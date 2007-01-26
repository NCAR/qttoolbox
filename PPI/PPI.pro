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

//LIBS += opengl32.lib
//LIBS += glu32.lib

CONFIG(release, debug|release) {
  TARGET = PPI
  LIBS       += ../ColorBar/Release/ColorBar.lib
  DLLDESTDIR += $(QTDIR)/lib
  DLLDESTDIR += $(QTDIR)/Plugins/Designer
} else {
  TARGET = PPId
  LIBS       += ../ColorBar/Debug/ColorBard.lib
  LIBS  += $(QTDIR)/lib/qwt5d.lib
  DLLDESTDIR += $(QTDIR)/lib
}

INCLUDEPATH += ../

HEADERS	+= PPI.h
HEADERS     += PPIPlugin.h

SOURCES	+= PPI.cc
SOURCES     += PPIPlugin.cpp

INCLUDEPATH += ../
INCLUDEPATH += ../../
