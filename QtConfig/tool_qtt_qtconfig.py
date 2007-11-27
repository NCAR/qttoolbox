# build the config library
tools = ['qtt_common', 'qt4']
env = Environment(tools = ['default'] + tools)

qt4modules = ['QtCore']
env.EnableQt4Modules(qt4modules)

tooldir = env.Dir('.')

qtconfig = env.Library('qtconfig', ['QtConfig.cpp'])

Default(qtconfig)

def qtt_qtconfig(env):
    for t in tools:
        env.Tool(t)
    env.EnableQt4Modules(qt4modules)
    env.AppendUnique(CPPPATH = [tooldir])
    env.AppendUnique(LIBPATH = [tooldir])
    env.Append(LIBS = ['qtconfig'])
    
Export('qtt_qtconfig')
