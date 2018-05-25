tools = ['qwt', 'qt4']
# tools we need for the build here, but do not get passed as dependencies
# to those loading *this* tool
local_tools = ['default', 'qtt_common', 'doxygen']
env = Environment(tools = local_tools + tools)

qt4modules = ['QtCore', 'QtGui', 'QtDesigner']
env.EnableQt4Modules(qt4modules)

tooldir = env.Dir('.').srcnode().abspath

# uic scopeplot form
env.Uic4('ScopePlot.ui')

# build knob shared library
sources = Split("""
ScopePlot.cpp
ScopePlotPlugin.cpp
ScrollBar.cpp
ScrollZoomer.cpp
""")

headers = Split("""
ScopePlot.h
ScopePlotPlugin.h
ScrollBar.h
ScrollZoomer.h
""")

scopeplot = env.SharedLibrary('scopeplot', sources)
scopeplot_install = env.InstallLib(scopeplot)

Default(scopeplot)

env['DOXYFILE_DICT'].update({ "PROJECT_NAME" : "Qttoolbox Scopeplot" })
html = env.Apidocs(sources + headers)
Default(html)

def qtt_scopeplot(env):
    for t in tools:
        env.Tool(t)
    env.EnableQt4Modules(qt4modules)
    env.AppendUnique(CPPPATH = [tooldir])
    env.AppendUnique(LIBPATH = [tooldir])
    env.AppendUnique(RPATH = [tooldir])
    env.Append(LIBS = ['scopeplot'])
    
Export('qtt_scopeplot')


