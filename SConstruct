import os,os.path, sys
sys.path.insert(0, os.path.join(os.getcwd(),'config'))

import atd_scons
import SCons

atd_scons.SConsignFile()

def QtToolboxSetup(env):

    env.Require(['QTDIR'])
    # TAO IDL-generated code triggers the char subscript warning, so
    # disable it.  Likewise g++ 4.0 warns on lots of 
    # non-virtual destructors
    # in Xerces and ACE, so disable that warning.
    env.AppendUnique (CCFLAGS=['-Wall','-Wno-char-subscripts'])
    env.AppendUnique (CXXFLAGS=['-Wno-non-virtual-dtor'])
    env.Apply ([atd_scons.DebugSetup])
    atd_scons.OptPrefixSetup (env)
    return env

options = atd_scons.Pkg_Options()

env = atd_scons.Pkg_Environment()
env.GlobalSetup ([QtToolboxSetup])
Export('env')

SConscript('Knob/SConscript')
SConscript('TwoKnobs/SConscript')
SConscript('ScopePlot/SConscript')
SConscript('ColorBar/SConscript')
SConscript('PPI/SConscript')

options.Update(env)

options = atd_scons.Pkg_Options()
options.Update(env)
Help(options.GenerateHelpText(env))
