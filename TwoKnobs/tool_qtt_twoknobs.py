tools = ['qt4', 'qwt']
# tools we need for the build here, but do not get passed as dependencies
# to those loading *this* tool
local_tools = ['qtt_common']
env = Environment(tools = ['default'] + tools + local_tools)

qt4modules = ['QtCore', 'QtGui', 'QtDesigner']
env.EnableQt4Modules(qt4modules)

tooldir = env.Dir('.')

# uic knob form
env.Uic4('TwoKnobs.ui')

# build knob shared library
sources = Split("""
TwoKnobs.cpp
TwoKnobsPlugin.cpp
""")
twoknobs = env.SharedLibrary('twoknobs', sources)

twoknobs_install = env.InstallLib(twoknobs)

Default(twoknobs)

def qtt_twoknobs(env):
    for t in tools:
        env.Tool(t)
    env.EnableQt4Modules(qt4modules)
    env.AppendUnique(CPPPATH = [tooldir])
    env.AppendUnique(LIBPATH = [tooldir])
    env.AppendUnique(RPATH=[tooldir])
    env.Append(LIBS = ['twoknobs'])
    
Export('qtt_twoknobs')


