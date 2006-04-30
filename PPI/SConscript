import os

Import('env')
env = env.Create("qttoolbox.ppi", tools = ['default', 'qt'])

my_tools = env.Require(['PKG_QT','PKG_GL'])

mydir=Dir('.')

def PKG_QTTOOLBOX_PPI(env):
        env.Append(CPPPATH=[mydir,])
	env.Append(LIBPATH=[mydir,])
	env.Append(LIBS=['ppi',])
	env.Append(QT_UICIMPLFLAGS=['-L', str(mydir)])
	env.Apply(my_tools)

Export('PKG_QTTOOLBOX_PPI')

lib = env.SharedLibrary('ppi', 
Split("""
 PPIBase.ui
 PPI.cc
 PPIPlugin.cpp
"""))

SConscript('Test/SConscript')

Default(lib)

env.Install(os.path.join(env['QTDIR'],'plugins','designer'), lib)
