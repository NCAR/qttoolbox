# -*- python -*-

import os
import re
import eol_scons

tools = Split("""
qt4
qtt_qtconfig
hcrdds
doxygen
boost_program_options
""")

env = Environment(tools = ['default'] + tools)

qt4modules = ['QtCore','QtGui']
env.EnableQt4Modules(qt4modules)

# Shared files (e.g., colortable files) will be installed in bscanShareDir,
# which is <INSTALL_DIR>/share/bscan
bscanShareDir = os.path.join(env['INSTALL_SHAREDIR'], 'bscan')

# Put in a preprocessor macro defining BSCAN_SHAREDIR
env.AppendUnique(CPPDEFINES = ['BSCAN_SHAREDIR=\\"' + bscanShareDir + '\\"'])

# This will create the ui_*.h files
uifiles = Split("""
Bscan.ui
DisplayLimitDialog.ui
GateLimitDialog.ui
TimeSpanDialog.ui
""")
env.Uic4(uifiles)

sources = Split("""
BscanMainWindow.cpp
BscanGraphicsScene.cpp
BscanGraphicsView.cpp
BscanRay.cpp
BscanWidget.cpp
ColorTable.cpp
DisplayLimitDialog.cpp
GateLimitDialog.cpp
RayGraphicsItem.cpp
TimeSpanDialog.cpp
""")

headers = Split("""
BscanGraphicsScene.h
BscanGraphicsView.h
BscanMainWindow.h
BscanRay.h
BscanWidget.h
ColorTable.h
DisplayLimitDialog.h
FakeDataThread.h
GateLimitDialog.h
RayGraphicsItem.h
TimeSpanDialog.h
""")

env['DOXYFILE_DICT'].update({ "PROJECT_NAME" : "qttoolbox bscan" })
html = env.Apidocs(sources + headers)
Default(html)

qtbscanlib = env.Library('qtbscan', sources)
Default(qtbscanlib)

# Build a list of all colortables/*.ct files
colortables = []
for file in os.listdir('colortables'):
    if (re.compile('.*\.ct').match(file)):
        colortables.append(os.path.join('colortables', file))

# Make install targets to put our color table files into
# INSTALL_DIR/share/bscan
ct_targets = []
for file in colortables:
    ct_targets += env.InstallShare('bscan', file)
Default(ct_targets)

# Finally, the tool section...
tooldir = env.Dir('.').srcnode().abspath    # this directory

def qtt_qtbscan(env):
    env.AppendUnique(CPPPATH = [tooldir])
    # add -DBSCAN_SHAREDIR=<share_dir> to compiles so that source knows where 
    # to look for shared files (e.g., colortables)
    env.AppendUnique(CPPDEFINES = ['BSCAN_SHAREDIR=\\"' + bscanShareDir + '\\"'])
    env.AppendLibrary('qtbscan')
    env.AppendDoxref('qttoolbox_QtBscan')
    env.Require(tools)
    env.EnableQt4Modules(qt4modules)

Export('qtt_qtbscan')
