import os

tools = ['qt4', 'qtt_colorbar']
env = Environment(tools=['default'] + tools)

qt4modules = ['QtCore', 'QtOpenGL']
env.EnableQt4Modules(qt4modules)

tooldir = env.Dir('.')

# build knob shared library
sources = Split("""
PPI.cc
PPIPlugin.cpp
ScaledLabel.cpp
""")

ppi = env.SharedLibrary('ppi', sources)
ppi_install = env.InstallLib(ppi)

Default(ppi)

# The following function and export actually create the qtt_ppi tool
def qtt_ppi(env):
    for t in tools:
        env.Tool(t)
    env.EnableQt4Modules(qt4modules)
    
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
