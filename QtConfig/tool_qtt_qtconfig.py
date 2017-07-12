# build the config library
tools = ['qt5', 'doxygen']
# tools we need for the build here, but do not get passed as dependencies
# to those loading *this* tool
local_tools = ['qtt_common']
env = Environment(tools = ['default'] + tools + local_tools)

qt5modules = ['QtCore']
env.EnableQt5Modules(qt5modules)

tooldir = env.Dir('.').srcnode().abspath

sources = Split("""
QtConfig.cpp
""")

headers = Split("""
QtConfig.h
""")

qtconfig = env.Library('qtconfig', sources)

Default(qtconfig)

env['DOXYFILE_DICT'].update({ "PROJECT_NAME" : "Qttoolbox Config" })
html = env.Apidocs(sources + headers)
Default(html)

def qtt_qtconfig(env):
    for t in tools:
        env.Tool(t)
    env.EnableQt5Modules(qt5modules)
    env.AppendUnique(CPPPATH = [tooldir])
    env.AppendUnique(LIBPATH = [tooldir])
    env.Append(LIBS = ['qtconfig'])
    
Export('qtt_qtconfig')
