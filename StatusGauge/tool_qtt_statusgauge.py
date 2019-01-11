tools = ['qt4', 'doxygen']
# tools we need for the build here, but do not get passed as dependencies
# to those loading *this* tool
local_tools = ['qtt_common']
env = Environment(tools = ['default'] + tools + local_tools)

qt4modules = ['QtCore', 'QtGui', 'QtDesigner']
env.EnableQt4Modules(qt4modules)

tooldir = env.Dir('.').srcnode().abspath

# build status guage shared library
sources = Split("""
StatusGauge.cpp
StatusGaugePlugin.cpp
""")

headers = Split("""
StatusGauge.h
StatusGaugePlugin.h
""")

statusgauge = env.SharedLibrary('statusgauge', sources)
statusgauge_install = env.InstallLib(statusgauge)

Default(statusgauge)

env['DOXYFILE_DICT'].update({ "PROJECT_NAME" : "Qttoolbox StatusGuage" })
html = env.Apidocs(sources + headers)

# The following function and Export actually define the qtt_statusgauge tool
def qtt_statusgauge(env):
    for t in tools:
        env.Tool(t)
    env.EnableQt4Modules(qt4modules)
    env.AppendUnique(CPPPATH = [tooldir])
    env.AppendUnique(LIBPATH = [tooldir])
    env.AppendUnique(RPATH=[tooldir])
    env.Append(LIBS = ['statusgauge'])

Export('qtt_statusgauge')
