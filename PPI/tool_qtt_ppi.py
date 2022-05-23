import os

tools = ['qtcore', 'qtopengl', 'qtt_colorbar', 'gl']

# tools we need for the build here, but do not get passed as dependencies
# to those loading *this* tool
local_tools = ['qtt_common', 'doxygen']
env = Environment(tools=['default'] + tools + local_tools)

tooldir = env.Dir('.').srcnode().abspath

# build knob shared library
sources = Split("""
PPI.cc
PPIPlugin.cpp
ScaledLabel.cpp
""")

headers = Split("""
PPI.h
PPIPlugin.h
ScaledLabel.h
""")

ppi = env.SharedLibrary('ppi', sources)
ppi_install = env.InstallLib(ppi)

Default(ppi)

env['DOXYFILE_DICT'].update({ "PROJECT_NAME" : "Qttoolbox PPI" })
html = env.Apidocs(sources + headers)

# The following function and export actually create the qtt_ppi tool
def qtt_ppi(env):
    env.Require(tools)

    # Deal with GLUT
    env['GLUTDIR'] = os.environ.get('GLUTDIR', None)
    if (env['GLUTDIR']):
        env.AppendUnique(CPPPATH = ['$GLUTDIR/include'])
        env.AppendUnique(LIBPATH = ['$GLUTDIR/lib'])
        
    env.AppendUnique(CPPPATH = [tooldir])
    env.AppendUnique(LIBPATH=[tooldir])
    env.AppendUnique(RPATH=[tooldir])
    env.Append(LIBS = ['ppi', 'glut'])
    
Export('qtt_ppi')
