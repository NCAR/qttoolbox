TEMPLATE	= vcapp
LANGUAGE	= C++

CONFIG	+= debug

INCLUDEPATH	+= ../

HEADERS	+= PPITest.h

SOURCES	+= main.cpp

FORMS	= PPITestBase.ui

win32 {
      LIBS       += ../PPI.lib
   LIBS       += ../../ColorBar/ColorBar.lib
   LIBS       += opengl32.lib
   LIBS       += glu32.lib
}


CONFIG += qt 
CONFIG += thread
CONFIG += warn_on 
CONFIG += exceptions

INCLUDEPATH += ../../
INCLUDEPATH += ../../ColorBar


SOURCES     += PPITest.cc

