import os
import SCons
from SCons.Options import PathOption
from SCons.Script import Options

options = None
myKey = 'HAS_TOOL_QWT'
USE_PKG_CONFIG = 'Using pkg-config'

def generate(env):
    global options
    if not options:
        options = Options()
        options.AddOptions(PathOption('QWTDIR', 'Qwt installation root.', None))

    options.Update(env)
    #
    # See if pkg-config knows about Qwt on this system
    #
    try:
        pkgConfigKnowsQwt = (os.system('pkg-config --exists Qwt') == 0)
    except:
        pkgConfigKnowsQwt = 0

    #
    # One-time stuff if this tool hasn't been loaded yet
    #
    if (not env.has_key(myKey)):
        #
        # We should require Qt here, but which version?
        #
        #env.Require(['qt'])
        
        # 
        # Try to find the Qwt installation location, trying in order:
        #    o command line QWTDIR option
        #    o OS environment QWTDIR
        #    o installation defined via pkg-config (this is the preferred method)
        # At the end of checking, either env['QWTDIR'] will contain the path
        # to the top of the installation, or it will be set to USE_PKG_CONFIG, 
        # or we will raise an exception.
        #
        if (env.has_key('QWTDIR')):
            pass
        elif (os.environ.has_key('QWTDIR')):
            env['QWTDIR'] = os.environ['QWTDIR']
        elif pkgConfigKnowsQwt:
            env['QWTDIR'] = USE_PKG_CONFIG
        else:
            raise SCons.Errors.StopError, "Qwt not found"
        
        #
        # First-time-only stuff here: -I<>, -D<>, and -L<> options
        # The -l<> stuff we do later every time this tool is loaded
        #   
        if (env['QWTDIR'] is USE_PKG_CONFIG):
            env.ParseConfig('pkg-config --cflags Qwt')
        else:
            qwt_dir = env['QWTDIR']
            qwt_libdir = os.path.join(qwt_dir, 'lib')
            env.AppendUnique(LIBPATH = [qwt_libdir])
            env.AppendUnique(RPATH = [qwt_libdir])  
            env.Append(CPPPATH = [os.path.join(qwt_dir, 'include')])
            plugindir= os.path.join(qwt_dir, 'designer', 'plugins', 'designer')
            env.Append(QT_UICIMPLFLAGS=['-L', plugindir])
            env.Append(QT_UICDECLFLAGS=['-L', plugindir])
            env.Append(DEPLOY_SHARED_LIBS='qwt')
            
        env[myKey] = True
    #
    # Add -lqwt each time this tool is requested
    #
    if (env['QWTDIR'] is USE_PKG_CONFIG):
        env.ParseConfig('pkg-config --libs Qwt')
    else:
        env.Append(LIBS = ['qwt'])        

def exists(env):
    return True

