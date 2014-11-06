tools = ['qt4', 'doxygen']
# tools we need for the build here, but do not get passed as dependencies
# to those loading *this* tool
local_tools = ['qtt_common']
env = Environment(tools = ['default'] + tools + local_tools)

qt4modules = ['QtCore', 'QtGui', 'QtDesigner']
env.EnableQt4Modules(qt4modules)

tooldir = env.Dir('.').srcnode().abspath

# build colorbar shared library
sources = Split("""
ColorBar.cpp
ColorMap.cpp
ColorBarPlugin.cpp
""")

headers = Split("""
ColorBar.h
ColorMap.h
ColorBarPlugin.h
""")

colorbar = env.SharedLibrary('colorbar', sources)
colorbar_install = env.InstallLib(colorbar)

Default(colorbar)

env['DOXYFILE_DICT'].update({ "PROJECT_NAME" : "Qttoolbox Colorbar" })
html = env.Apidocs(sources + headers)
Default(html)

# The following function and Export actually define the qtt_colorbar tool
def qtt_colorbar(env):
    for t in tools:
        env.Tool(t)
    env.EnableQt4Modules(qt4modules)
    env.AppendUnique(CPPPATH = [tooldir])
    env.AppendUnique(LIBPATH = [tooldir])
    env.AppendUnique(RPATH=[tooldir])
    env.Append(LIBS = ['colorbar'])

Export('qtt_colorbar')