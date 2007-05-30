import os,os.path, sys
sys.path.insert(0, os.path.join(os.getcwd(),'#./'))

import SCons

env = Environment(tools=['default', 'qt4'], toolpath=['#'])
env['QWTDIR'] = os.environ.get('QWTDIR', None)
env.AppendUnique(CPPPATH=['$QWTDIR/include', '..', '$QTDIR/include/QtDesigner', '/usr/include/GL', '#/ColorBar'])
env.AppendUnique(QT4_MOCFROMHFLAGS=['-I', '$QTDIR/include/QtDesigner/'])
env.EnableQt4Modules(['QtCore','QtGui','QtOpenGL'])

Export('env')

SConscript('Knob/SConscript')
SConscript('TwoKnobs/SConscript')
SConscript('ScopePlot/SConscript')
SConscript('ColorBar/SConscript')
SConscript('PPI/SConscript')

