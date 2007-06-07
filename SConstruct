# The following environment variables are expected:
# QTDIR        - The location of Qt4
# QWTDIR       - The location of the qwt installation
# GLUTDIR      - The location of glut.
#
# if not specified by QWTDIR and GLUTDIR, then qwt and glut 
# will be assumed to be located in standard install locations. 

import os,os.path, sys
sys.path.insert(0, os.path.join(os.getcwd(),'#./'))

import SCons

# Create our environment. We are expecting to find 
# qt4.py in the top level directory.
env = Environment(tools=['default', 'qt4'], toolpath=['#./'])

# add qwt and glut to the paths
env['QWTDIR'] = os.environ.get('QWTDIR', None)
env['GLUTDIR'] = os.environ.get('GLUTDIR', None)
env.AppendUnique(CPPPATH=['../', '$QWTDIR/include', 
	'$QTDIR/include/QtDesigner', '$GLUTDIR/include/GL', 
	'/usr/include/GL', '#/ColorBar'])


# Fix moc needs
env.AppendUnique(QT4_MOCFROMHFLAGS=['-I', '$QTDIR/include/QtDesigner/'])


# enable the qt4 modules
env.EnableQt4Modules(['QtCore','QtGui','QtOpenGL'])

# add install library method
def InstallLib(self, lib):
	installdir = '#/lib'
	self.Install(installdir, lib)
	self.Alias('install', installdir)

Environment.InstallLib = InstallLib

# add install binary method
def InstallBin(self, bin):
	installdir = '#/bin'
	self.Install(installdir, bin)
	self.Alias('install', installdir)

Environment.InstallBin = InstallBin

# export this environment
Export('env')

SConscript('Knob/SConscript')
SConscript('TwoKnobs/SConscript')
SConscript('ScopePlot/SConscript')
SConscript('ColorBar/SConscript')
SConscript('PPI/SConscript')

