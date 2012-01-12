# -*- python -*-

import os
import re
import eol_scons

tools = Split("""
prefixoptions
qt4
qtt_qtconfig
doxygen
""")

env = Environment(tools = ['default'] + tools)
env.AppendUnique(CPPFLAGS = ['-g', '-O0'])

qt4modules = ['QtCore','QtGui']
env.EnableQt4Modules(qt4modules)

# The directory where shared files (e.g., color tables) for QtBscan will be
# installed
bscanShareDir = env.subst(os.path.join(env['INSTALL_SHAREDIR'], 'qtt_bscan'))

# Create BscanShareDir.h, which holds a string with the path to installed 
# shared files for QtBscan.
f = open('BscanShareDir.h', 'w')
f.write('#include <string>\n')
f.write('// directory where shared files (e.g., color tables) are stored\n')
# Make sure we replace backslashes with double backslashes for C string syntax.
# This is important in the Windows world, where backslash is the path separator!
f.write('static const std::string BscanShareDir = "' + 
        bscanShareDir.replace('\\','\\\\') + '";\n')
f.close()

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
    env.AppendLibrary('qtbscan')
    env.AppendDoxref('qttoolbox_QtBscan')
    env.Require(tools)
    env.EnableQt4Modules(qt4modules)

Export('qtt_qtbscan')
