tools = ['qtt_common', 'qwt', 'qt4']
env = Environment(tools=['default'] + tools)

qt4modules = ['QtCore', 'QtGui', 'QtOpenGL', 'QtXml', 'QtDesigner']
env.EnableQt4Modules(qt4modules)

tooldir = env.Dir('.')

# uic knob form
env.Uic4('Knob.ui')

# build knob shared library
sources = Split("""
Knob.cpp
KnobPlugin.cpp
""")

knob = env.SharedLibrary('knob', sources)
knob_install = env.InstallLib(knob)

Default(knob)

# Actually define and export the qtt_knob tool
def qtt_knob(env):
    for t in tools:
        env.Tool(t)
    env.EnableQt4Modules(qt4modules)
    env.AppendUnique(CPPPATH = [tooldir])
    env.AppendUnique(LIBPATH = [tooldir])
    env.AppendUnique(RPATH=[tooldir])
    env.Append(LIBS = ['knob'])
    
Export('qtt_knob')
    