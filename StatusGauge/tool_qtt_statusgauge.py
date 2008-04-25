tools = ['qtt_common', 'qt4']
env = Environment(tools = ['default'] + tools)

qt4modules = ['QtCore', 'QtGui', 'QtDesigner']
env.EnableQt4Modules(qt4modules)

tooldir = env.Dir('.')

# build knob shared library
sources = Split("""
StatusGauge.cpp
StatusGaugePlugin.cpp
""")

statusgauge = env.SharedLibrary('statusgauge', sources)
statusgauge = env.InstallLib(statusgauge)

Default(statusgauge)

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
