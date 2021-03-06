
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
plugindefs = ['_REENTRANT', 'QT_PLUGIN', 'QDESIGNER_EXPORT_WIDGETS','QT_SHARED','QT_NO_DEBUG']

def qtt_common(env):
    env.AddMethod(_InstallLib, "InstallLib")
    env.AddMethod(_InstallBin, "InstallBin")
    env.AppendUnique(CPPDEFINES = plugindefs)
    env.Append(CCFLAGS='-g')
    env.Append(CCFLAGS='-O2')
    
Export('qtt_common')
    
