TEMPLATE	= vcapp
LANGUAGE	= C++

CONFIG += debug
CONFIG += qt 
CONFIG += thread
CONFIG += warn_on 
CONFIG += exceptions

INCLUDEPATH += ../
INCLUDEPATH += ../../

SOURCES	+= main.cpp

FORMS += TwoKnobsTest.ui

LIBS  += ../Debug/TwoKnobs.lib



