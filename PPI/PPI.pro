TEMPLATE	= vclib
LANGUAGE	= C++

# do not specify CONFIG += release or CONFIG += debug 
# as this fouls up the library specifications
# for different configurations. 
# However, not adding one or the other means that the
# generated project file name will take the name 
# of the debug TARGET...

CONFIG += opengl
CONFIG += designer
CONFIG += plugin
CONFIG += thread
CONFIG += warn_on 
CONFIG += exceptions

QT += opengl

CONFIG(release, debug|release) {
  TARGET = PPI
  LIBS       += ../ColorBar/Release/ColorBar.lib
  LIBS       += ../../glut-3.7.6/lib/glut/release/glut32.lib
  DLLDESTDIR += $(QTDIR)/lib
  DLLDESTDIR += $(QTDIR)/Plugins/Designer
} else {
  TARGET = PPId
  LIBS       += ../ColorBar/Debug/ColorBard.lib
  LIBS       += ../../glut-3.7.6/lib/glut/debug/glut32.lib
  DLLDESTDIR += $(QTDIR)/lib
}

INCLUDEPATH += ../
INCLUDEPATH += ../ColorBar
INCLUDEPATH += ../../
INCLUDEPATH += ../../glut-3.7.6/include/GL

HEADERS	+= PPI.h
HEADERS     += PPIPlugin.h
HEADERS     += ScaledLabel.h

SOURCES	+= PPI.cc
SOURCES     += PPIPlugin.cpp
SOURCES     += ScaledLabel.cpp
