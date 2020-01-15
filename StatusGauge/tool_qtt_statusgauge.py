tools = ['qt5', 'qtcore', 'qtwidgets', 'qtdesigner']
# tools we need for the build here, but do not get passed as dependencies
# to those loading *this* tool
local_tools = ['qtt_common', 'doxygen']
env = Environment(tools = ['default'] + tools + local_tools)

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
    env.Require(tools)
    env.AppendUnique(CPPPATH = [tooldir])
    env.AppendUnique(LIBPATH = [tooldir])
    env.AppendUnique(RPATH=[tooldir])
    env.Append(LIBS = ['statusgauge'])

Export('qtt_statusgauge')
