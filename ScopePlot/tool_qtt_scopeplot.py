tools = ['qtt_common', 'qwt', 'qt4']
env = Environment(tools = ['default'] + tools)

qt4modules = ['QtCore', 'QtGui', 'QtDesigner']
env.EnableQt4Modules(qt4modules)

tooldir = env.Dir('.')

# uic scopeplot form
env.Uic4('ScopePlot.ui')

# build knob shared library
sources = Split("""
ScopePlot.cpp
ScopePlotPlugin.cpp
ScrollBar.cpp
ScrollZoomer.cpp
""")

scopeplot = env.SharedLibrary('scopeplot', sources)
scopeplot_install = env.InstallLib(scopeplot)

Default(scopeplot)

def qtt_scopeplot(env):
    for t in tools:
        env.Tool(t)
    env.EnableQt4Modules(qt4modules)
    env.AppendUnique(CPPPATH = [tooldir])
    env.AppendUnique(LIBPATH = [tooldir])
    env.AppendUnique(RPATH = [tooldir])
    env.Append(LIBS = ['scopeplot'])
    
Export('qtt_scopeplot')


