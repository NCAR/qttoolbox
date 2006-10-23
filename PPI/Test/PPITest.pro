TEMPLATE	= vcapp
LANGUAGE	= C++

CONFIG += debug
CONFIG += qt 
CONFIG += thread
CONFIG += warn_on 
CONFIG += exceptions

FORMS	= PPITestBase.ui

HEADERS	+= PPITest.h

SOURCES	+= main.cpp
SOURCES     += PPITest.cc

INCLUDEPATH	+= ../
INCLUDEPATH += ../../
INCLUDEPATH += ../../ColorBar


LIBS       += ../Debug/PPI.lib
LIBS       += ../../ColorBar/Debug/ColorBar.lib
LIBS       += opengl32.lib
LIBS       += glu32.lib



