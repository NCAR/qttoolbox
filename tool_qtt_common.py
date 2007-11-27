import new

# install library method
def _InstallLib(self, lib):
    installdir = '#/designer'
    self.Install(installdir, lib)
    self.Alias('install', installdir)

# install binary method
def _InstallBin(self, bin):
    installdir = '#/bin'
    self.Install(installdir, bin)
    self.Alias('install', installdir)

# preprocessor defs needed for Qt plugins
plugindefs = ['REENTRANT']

def qtt_common(env):
    env.InstallLib = new.instancemethod(_InstallLib, env, type(env))
    env.InstallBin = new.instancemethod(_InstallBin, env, type(env))
    env.AppendUnique(CPPDEFINES = plugindefs)
    
Export('qtt_common')
    