import re
import os

import SCons.Defaults
import SCons.Node
import SCons.Tool
import SCons.Util
from SCons.Options import PathOption
from SCons.Script import Options

options = None
USE_PKG_CONFIG = "Using pkg-config"
myKey = "HAS_TOOL_QT5"

class ToolQt5Warning(SCons.Warnings.Warning):
    pass
class GeneratedMocFileNotIncluded(ToolQt5Warning):
    pass
class Qt5ModuleIssue(ToolQt5Warning):
    pass
SCons.Warnings.enableWarningClass(ToolQt5Warning)

qrcinclude_re = re.compile(r'<file>([^<]*)</file>', re.M)


header_extensions = [".h", ".hxx", ".hpp", ".hh"]
if SCons.Util.case_sensitive_suffixes('.h', '.H'):
    header_extensions.append('.H')
cxx_suffixes = [".c", ".cxx", ".cpp", ".cc"]

def _checkMocIncluded(target, source, env):
    moc = target[0]
    cpp = source[0]
    # looks like cpp.includes is cleared before the build stage :-(
    # not really sure about the path transformations (moc.cwd? cpp.cwd?) :-/
    path = SCons.Defaults.CScan.path_function(env, moc.cwd)
    includes = SCons.Defaults.CScan(cpp, env, path)
    if not moc in includes:
        SCons.Warnings.warn(
            GeneratedMocFileNotIncluded,
            "Generated moc file '%s' is not included by '%s'" %
            (str(moc), str(cpp)))
        
def _find_file(filename, paths, node_factory):
    retval = None
    for dir in paths:
        node = node_factory(filename, dir)
        if node.rexists():
            return node
    return None

class _Automoc:
    """
    Callable class, which works as an emitter for Programs, SharedLibraries and
    StaticLibraries.
    """

    def __init__(self, objBuilderName):
        self.objBuilderName = objBuilderName
        
    def __call__(self, target, source, env):
        """
        Smart autoscan function. Gets the list of objects for the Program
        or Lib. Adds objects and builders for the special qt5 files.
        """
        try:
            if int(env.subst('$QT_AUTOSCAN')) == 0:
                return target, source
        except ValueError:
            pass
        try:
            debug = int(env.subst('$QT_DEBUG'))
        except ValueError:
            debug = 0
        # some shortcuts used in the scanner
        FS = SCons.Node.FS.default_fs
        objBuilder = getattr(env, self.objBuilderName)

        # some regular expressions:
        # Q_OBJECT detection
        q_object_search = re.compile(r'[^A-Za-z0-9]Q_OBJECT[^A-Za-z0-9]') 
        # cxx and c comment 'eater'
        #comment = re.compile(r'(//.*)|(/\*(([^*])|(\*[^/]))*\*/)')
        # CW: something must be wrong with the regexp. See also bug #998222
        #     CURRENTLY THERE IS NO TEST CASE FOR THAT
        
        # The following is kind of hacky to get builders working properly (FIXME)
        objBuilderEnv = objBuilder.env
        objBuilder.env = env
        mocBuilderEnv = env.Moc4.env
        env.Moc4.env = env
        
        # make a deep copy for the result; MocH objects will be appended
        out_sources = source[:]

        for obj in source:
            #
            # KLUGE: If the obj is not a SCons.Node.FS.Entry, it may be a list
            # containing only the entry.  If we get a list of length one, just
            # use its single entry as the obj.  Not sure why this has become
            # an issue now...
            #
            if (not isinstance(obj, SCons.Node.FS.Entry)):
                try:
                    if (len(obj) != 1):
                        raise Error
                    obj = obj[0]
                except:
                    errmsg = "qt5/_Automoc_ got a bad source object: "
                    errmsg += str(obj)
                    raise SCons.Errors.StopError, errmsg                    

            if not obj.has_builder():
                # binary obj file provided
                if debug:
                    print "scons: qt5: '%s' seems to be a binary. Discarded." % str(obj)
                continue

            cpp = obj.sources[0]
            if not SCons.Util.splitext(str(cpp))[1] in cxx_suffixes:
                if debug:
                    print "scons: qt5: '%s' is not a C++ file. Discarded." % str(cpp) 
                # c or fortran source
                continue
            #cpp_contents = comment.sub('', cpp.get_contents())
            cpp_contents = cpp.get_contents()
            h=None
            for h_ext in header_extensions:
                # try to find the header file in the corresponding source
                # directory
                hname = SCons.Util.splitext(cpp.name)[0] + h_ext
                h = _find_file(hname,
                              (cpp.get_dir(),),
                              FS.File)
                if h:
                    if debug:
                        print "scons: qt5: Scanning '%s' (header of '%s')" % (str(h), str(cpp))
                    #h_contents = comment.sub('', h.get_contents())
                    h_contents = h.get_contents()
                    break
            if not h and debug:
                print "scons: qt5: no header for '%s'." % (str(cpp))
            if h and q_object_search.search(h_contents):
                # h file with the Q_OBJECT macro found -> add moc_cpp
                moc_cpp = env.Moc4(h)
                moc_o = objBuilder(moc_cpp)
                out_sources.append(moc_o)
                #moc_cpp.target_scanner = SCons.Defaults.CScan
                if debug:
                    print "scons: qt5: found Q_OBJECT macro in '%s', moc'ing to '%s'" % (str(h), str(moc_cpp))
            if cpp and q_object_search.search(cpp_contents):
                # cpp file with Q_OBJECT macro found -> add moc
                # (to be included in cpp)
                moc = env.Moc4(cpp)
                env.Ignore(moc, moc)
                if debug:
                    print "scons: qt5: found Q_OBJECT macro in '%s', moc'ing to '%s'" % (str(cpp), str(moc))
                #moc.source_scanner = SCons.Defaults.CScan
        # restore the original env attributes (FIXME)
        objBuilder.env = objBuilderEnv
        env.Moc4.env = mocBuilderEnv

        return (target, out_sources)

AutomocShared = _Automoc('SharedObject')
AutomocStatic = _Automoc('StaticObject')

def generate(env):
    """Add Builders and construction variables for qt5 to an Environment."""
    if env.has_key(myKey):
        return

    def locateQt5Command(env, command) :
        commandQt5 = command + "-qt5"
        testenv = env.Copy()
        qt5BinDir = None
        #
        # If env['QT5DIR'] is defined, make sure we try its bin directory first
        #
        if (env.has_key('QT5DIR')):
            qt5BinDir = os.path.join(env['QT5DIR'], 'bin')
            testenv.PrependENVPath('PATH', qt5BinDir)
            
        whichCmd = testenv.Detect([commandQt5, command])
        if (not whichCmd):
            msg = "Qt5 command " + commandQt5 + " (" + command + ")"
            if (qt5BinDir):
                msg += " not in " + qt5BinDir + " or in $PATH"
            else:
                msg += " not in $PATH"
            raise(msg)

        return testenv.WhereIs(whichCmd) # return the full path to the cmd


    global options
    if not options:
        options = Options()
        options.AddOptions(PathOption('QT5DIR', 'Qt5 installation root.', None))
    options.Update(env)

    #
    # See if pkg-config knows about Qt5 on this system
    #
    try:
        pkgConfigKnowsQt5 = (os.system('pkg-config --exists Qt') == 0)
    except:
        pkgConfigKnowsQt5 = 0
    # 
    # Try to find the Qt5 installation location, trying in order:
    #    o command line QT5DIR option
    #    o OS environment QT5DIR
    #    o installation defined via pkg-config (this is the preferred method)
    #    o parent of directory holding moc-qt5 in the execution path
    #    o parent of directory holding moc in the execution path
    # At the end of checking, either env['QT5DIR'] will point to the
    # top of the installation, it will be set to USE_PKG_CONFIG, or 
    # we will raise an exception.
    #
    if (env.has_key('QT5DIR')):
        pass
    elif (os.environ.has_key('QT5DIR')):
        env['QT5DIR'] = os.environ['QT5DIR']
    elif pkgConfigKnowsQt5:
        env['QT5DIR'] = USE_PKG_CONFIG
    else:
        moc = env.WhereIs('moc-qt5') or env.WhereIs('moc')
        if moc:
            env['QT5DIR'] = os.path.dirname(os.path.dirname(moc))
        else:
            errmsg = "Qt5 not found"
            raise SCons.Errors.StopError, errmsg
        
    # the basics
    env['QT5_MOC'] = locateQt5Command(env, 'moc')
    env['QT5_UIC'] = locateQt5Command(env, 'uic')
    env['QT5_RCC'] = locateQt5Command(env, 'rcc')
    env['QT5_LUPDATE'] = locateQt5Command(env, 'lupdate')
    env['QT5_LRELEASE'] = locateQt5Command(env, 'lrelease')
    
    # Should the qt5 tool try to figure out which sources are to be moc'ed ?
    env['QT5_AUTOSCAN'] = 1
    
    # Some QT specific flags. I don't expect someone wants to
    # manipulate those ...
    env['QT5_UICDECLFLAGS'] = ''
    env['QT5_MOCFROMHFLAGS'] = ''
    env['QT5_MOCFROMCXXFLAGS'] = '-i'
    env['QT5_QRCFLAGS'] = ''
    
    # suffixes/prefixes for the headers / sources to generate
    env['QT5_MOCHPREFIX'] = 'moc_'
    env['QT5_MOCHSUFFIX'] = '$CXXFILESUFFIX'
    env['QT5_MOCCXXPREFIX'] = 'moc_'
    env['QT5_MOCCXXSUFFIX'] = '.moc'
    env['QT5_UISUFFIX'] = '.ui'
    env['QT5_UICDECLPREFIX'] = 'ui_'
    env['QT5_UICDECLSUFFIX'] = '.h'
    env['QT5_QRCSUFFIX'] = '.qrc',
    env['QT5_QRCCXXSUFFIX'] = '$CXXFILESUFFIX'
    env['QT5_QRCCXXPREFIX'] = 'qrc_'
    
    # Translation builder
    tsbuilder = SCons.Builder.Builder(action ='$QT5_LUPDATE $SOURCES -ts $TARGETS',
                                      multi=1)
    env.Append( BUILDERS = { 'Ts': tsbuilder } )
    qmbuilder = SCons.Builder.Builder(action =['$QT5_LRELEASE $SOURCE',    ],
                                      src_suffix = '.ts',
                                      suffix = '.qm',
                                      single_source = True)
    env.Append( BUILDERS = { 'Qm': qmbuilder } )
    
    # Resource builder
    def scanResources(node, env, path, arg):
        contents = node.get_contents()
        includes = qrcinclude_re.findall(contents)
        return includes
    qrcscanner = env.Scanner(name = 'qrcfile',
        function = scanResources,
        argument = None,
        skeys = ['.qrc'])
    qrcbuilder = SCons.Builder.Builder(action ='$QT5_RCC $QT5_QRCFLAGS $SOURCE -o $TARGET',
                                       source_scanner = qrcscanner,
                                       src_suffix = '$QT5_QRCSUFFIX',
                                       suffix = '$QT5_QRCCXXSUFFIX',
                                       prefix = '$QT5_QRCCXXPREFIX',
                                       single_source = True)
    env.Append( BUILDERS = { 'Qrc': qrcbuilder } )
    
    # Interface builder
    env['QT5_UIC4CMD'] = [
        SCons.Util.CLVar('$QT5_UIC $QT5_UICDECLFLAGS -o ${TARGETS[0]} $SOURCE'),
        ]
    uic4builder = SCons.Builder.Builder(action='$QT5_UIC4CMD',
                                        src_suffix='$QT5_UISUFFIX',
                                        suffix='$QT5_UICDECLSUFFIX',
                                        prefix='$QT5_UICDECLPREFIX',
                                        single_source = True)
    env.Append( BUILDERS = { 'Uic4': uic4builder } )
    
    # Metaobject builder
    env['QT5_MOCFROMHCMD'] = (
        '$QT5_MOC $QT5_MOCFROMHFLAGS -o ${TARGETS[0]} $SOURCE')
    env['QT5_MOCFROMCXXCMD'] = [
        SCons.Util.CLVar('$QT5_MOC $QT5_MOCFROMCXXFLAGS -o ${TARGETS[0]} $SOURCE'),
        SCons.Action.Action(_checkMocIncluded,None)]
    mocBld = SCons.Builder.Builder(action={}, prefix={}, suffix={})
    for h in header_extensions:
        mocBld.add_action(h, '$QT5_MOCFROMHCMD')
        mocBld.prefix[h] = '$QT5_MOCHPREFIX'
        mocBld.suffix[h] = '$QT5_MOCHSUFFIX'
    for cxx in cxx_suffixes:
        mocBld.add_action(cxx, '$QT5_MOCFROMCXXCMD')
        mocBld.prefix[cxx] = '$QT5_MOCCXXPREFIX'
        mocBld.suffix[cxx] = '$QT5_MOCCXXSUFFIX'
    env.Append( BUILDERS = { 'Moc4': mocBld } )
    
    # er... no idea what that was for
    static_obj, shared_obj = SCons.Tool.createObjBuilders(env)
    static_obj.src_builder.append('Uic4')
    shared_obj.src_builder.append('Uic4')
    
    # We use the emitters of Program / StaticLibrary / SharedLibrary
    # to scan for moc'able files
    # We can't refer to the builders directly, we have to fetch them
    # as Environment attributes because that sets them up to be called
    # correctly later by our emitter.
    #env.AppendUnique(PROGEMITTER =[AutomocStatic],
    #                 SHLIBEMITTER=[AutomocShared],
    #                 LIBEMITTER  =[AutomocStatic],
    #                 # Of course, we need to link against the qt5 libraries
    #                 CPPPATH=[os.path.join('$QT5DIR', 'include')],
    #                 LIBPATH=[os.path.join('$QT5DIR', 'lib')],
    env.AppendUnique(PROGEMITTER =[AutomocStatic],
                     SHLIBEMITTER=[AutomocShared],
                     LIBEMITTER  =[AutomocStatic])
    
    import new
    env.EnableQt5Modules = new.instancemethod(enable_modules, env, type(env))
    env[myKey] = True

# list of modules for which we've issued "No pkgconfig for <module>" warnings
no_pkgconfig_warned = []
def enable_modules(self, modules, debug=False) :
    import sys

    if sys.platform == "linux2" :
        if debug : modules = [module + "_debug" for module in modules]
        for module in modules:
            if (self['QT5DIR'] == USE_PKG_CONFIG):
                # Starting directory for headers
                hdir = os.popen('pkg-config --variable=headerdir Qt').read()
                hdir = hdir.strip()
                
                if (os.system('pkg-config --exists ' + module) == 0):
                    # Don't try here to make things unique in LIBS and
                    # CFLAGS; just do a simple append
                    self.ParseConfig('pkg-config --libs --cflags ' + module,
                                     unique = False)
                else:
                    # warn if we haven't already
                    if not (module in no_pkgconfig_warned):
                        print("Warning: No pkgconfig for Qt5/" + module + 
                              ", doing what I can...")
                        no_pkgconfig_warned.append(module)
                    # Add -l<module>
                    self.AppendUnique(LIBS = [module])
                    # Add -I<Qt5HeaderDir>/<module>
                    self.AppendUnique(CPPPATH = [os.path.join(hdir, module)])
            else:
                # Module library directory can apparently be either 
                # <QT5DIR>/lib/<module> or just <QT5DIR>/lib.  Use the longer
                # one if the directory exists, otherwise the shorter one...
                libpath = os.path.join(self['QT5DIR'], 'lib')
                longpath = os.path.join(libpath, module)
                if os.path.isdir(longpath):
                    libpath = longpath
                self.AppendUnique(LIBPATH = [libpath])

                hdir = os.path.join(self['QT5DIR'], 'include')
                self.AppendUnique(CPPPATH = [hdir])
                self.AppendUnique(CPPPATH = [os.path.join(hdir, module)])
                self.Append(LIBS = [module])

            # Kluge(?) so that moc can find the QtDesigner headers, necessary
            # at least for Fedora 7
            if module == "QtDesigner":
                self.AppendUnique(QT5_MOCFROMHFLAGS =
                                  ['-I', os.path.join(hdir, module)])

        return
    if sys.platform == "win32" :
        if debug : debugSuffix = 'd'
        else : debugSuffix = ''
        self.AppendUnique(LIBS=[lib+'4'+debugSuffix for lib in modules])
        if 'QtOpenGL' in modules:
            self.AppendUnique(LIBS=['opengl32'])
        self.AppendUnique(CPPPATH=[ '$QT5DIR/include/'+module
            for module in modules])
        self.AppendUnique(LIBPATH=['$QT5DIR/lib'])


def exists(env):
    return _detect(env)
