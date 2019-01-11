# build the config library
tools = ['qt4', 'doxygen']
# tools we need for the build here, but do not get passed as dependencies
# to those loading *this* tool
local_tools = ['qtt_common']
env = Environment(tools = ['default'] + tools + local_tools)

qt4modules = ['QtCore']
env.EnableQt4Modules(qt4modules)

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

def qtt_qtconfig(env):
    for t in tools:
        env.Tool(t)
    env.EnableQt4Modules(qt4modules)
    env.AppendUnique(CPPPATH = [tooldir])
    env.AppendUnique(LIBPATH = [tooldir])
    env.Append(LIBS = ['qtconfig'])
    
Export('qtt_qtconfig')
