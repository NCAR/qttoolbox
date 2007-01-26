TEMPLATE	= vcapp
LANGUAGE	= C++

CONFIG += qt 
CONFIG += thread
CONFIG += warn_on 
CONFIG += exceptions
CONFIG += opengl

QT += opengl

CONFIG(release, debug|release) {
  LIBS  += ../release/PPI.lib
  LIBS  += ../../ColorBar/release/ColorBar.lib
} else {
  LIBS  += ../Debug/PPId.lib
  LIBS  += ../../ColorBar/debug/ColorBard.lib
}

INCLUDEPATH += ../
INCLUDEPATH += ../../
INCLUDEPATH += ../../ColorBar

FORMS       += PPITest.ui

HEADERS	+= PPITest.h

SOURCES	+= main.cpp
SOURCES     += PPITest.cc

