TEMPLATE = vcapp

LANGUAGE	= C++

CONFIG += debug
CONFIG += qt 
CONFIG += thread
CONFIG += warn_on 
CONFIG += exceptions

INCLUDEPATH += ../
INCLUDEPATH += ../../

HEADERS += ColorBarTest.h

SOURCES	+= main.cpp
SOURCES += ColorBarTest.cpp

FORMS   += ColorBarTestBase.ui

LIBS     += ../Debug/ColorBar.lib
