tools = ['qt5', 'qwt', 'doxygen']
# tools we need for the build here, but do not get passed as dependencies
# to those loading *this* tool
local_tools = ['default', 'qtt_common', 'doxygen']
env = Environment(tools = local_tools + tools)

qtmodules = ['QtCore', 'QtWidgets', 'QtGui', 'QtOpenGL', 'QtXml']
env.EnableQtModules(qtmodules)

tooldir = env.Dir('.').srcnode().abspath

# uic knob form
env.Uic5('Knob.ui')

# build knob shared library
sources = Split("""
Knob.cpp
""")

headers = Split("""
Knob.h
""")

if False:
    env.Tool('qtdesigner')
    headers += "KnobPlugin.h"
    sources += "KnobPlugin.cpp"

knob = env.SharedLibrary('knob', sources)
knob_install = env.InstallLib(knob)

Default(knob)

env['DOXYFILE_DICT'].update({ "PROJECT_NAME" : "Qttoolbox Knob" })
html = env.Apidocs(sources + headers)


# Actually define and export the qtt_knob tool
def qtt_knob(env):
    for t in tools:
        env.Tool(t)
    env.EnableQtModules(qtmodules)
    env.AppendUnique(CPPPATH = [tooldir])
    env.AppendUnique(LIBPATH = [tooldir])
    env.AppendUnique(RPATH=[tooldir])
    env.Append(LIBS = ['knob'])
    
Export('qtt_knob')

