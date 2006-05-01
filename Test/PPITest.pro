win32 {
   TEMPLATE	= vcapp
   LIBS       += ../PPI.lib
   LIBS       += ../../ColorBar/ColorBar.lib
   LIBS       += opengl32.lib
   LIBS       += glu32.lib
}

LANGUAGE	= C++

CONFIG += debug
CONFIG += qt 
CONFIG += thread
CONFIG += warn_on 
CONFIG += exceptions

INCLUDEPATH += ../
INCLUDEPATH += ../../
INCLUDEPATH += ../../ColorBar

HEADERS     += PPITest.h

SOURCES	+= main.cpp
SOURCES     += PPITest.cc

FORMS       += PPITestBase.ui
