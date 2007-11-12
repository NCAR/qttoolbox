# The following environment variables are expected:
# QTDIR        - The location of Qt4
# QWTDIR       - The location of the qwt installation
# GLUTDIR      - The location of glut.
#
# if not specified by QWTDIR and GLUTDIR, then qwt and glut 
# will be assumed to be located in standard install locations. 

import os

# Create our environment. 
qttbenv = Environment(tools=['default', 'qt4'],
	toolpath=['.',],  ENV=os.environ)

# Export the qttoolbox aplication environment
Export('qttbenv')

# add qwt and glut to the paths
qttbenv['QTDIR'] = os.environ.get('QTDIR', None)
qttbenv['QWTDIR'] = os.environ.get('QWTDIR', None)
qttbenv['GLUTDIR'] = os.environ.get('GLUTDIR', None)
qttbenv.AppendUnique(CPPPATH=[
	'../', 
	'$QWTDIR/include', 
	'$QTDIR/include/QtDesigner', 
	'$GLUTDIR/include/GL', 
	'/usr/include/GL',
	])

qttbenv.AppendUnique(CPPFLAGS=['-O2', ])

# Fix moc needs
qttbenv.AppendUnique(QT4_MOCFROMHFLAGS=['-I', '$QTDIR/include/QtDesigner/'])

# enable the qt4 modules
qttbenv.EnableQt4Modules(['QtCore', 'QtGui', 'QtOpenGL', 'QtXml', ])

# create an environment for building the plugins
# The cxx and moc flags must match EXACTLY the
# qt system build flags, or designer will silently 
# reject your plugin. How the heck are you supposed to know
# what these flags are? I sleuthed them out by creating
# a Linux .pro file and using qmake to generate a makefile,
# then running the makefile to see how it built the plugin,
# then verifying that designer could see the plugin generated
# in this fashion. Geez, there has to be a better way.

pluginenv = qttbenv.Clone()

flags=[
'-D_REENTRANT', 
'-DQT_NO_DEBUG', 
'-DQT_PLUGIN', 
'-DQT_XML_LIB', 
'-DQT_GUI_LIB', 
'-DQT_CORE_LIB', 
'-DQDESIGNER_EXPORT_WIDGETS', 
'-DQT_SHARED', 
]

# add install library method
def InstallLib(self, lib):
	installdir = '#/designer'
	self.Install(installdir, lib)
	self.Alias('install', installdir)

Environment.InstallLib = InstallLib

# add install binary method
def InstallBin(self, bin):
	installdir = '#/bin'
	self.Install(installdir, bin)
	self.Alias('install', installdir)

Environment.InstallBin = InstallBin

# Export the QtToolbox plugin environment.
Export('pluginenv')

SConscript('Knob/SConscript')
SConscript('TwoKnobs/SConscript')
SConscript('ScopePlot/SConscript')
SConscript('ColorBar/SConscript')
SConscript('PPI/SConscript')
