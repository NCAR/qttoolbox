TEMPLATE	= vcapp
LIBS       += c:/Projects/qwt/lib/qwt500.lib
LIBS       += ../Debug/ScopePlot.lib

LANGUAGE	= C++

CONFIG += debug
CONFIG += qt 
CONFIG += thread
CONFIG += warn_on 
CONFIG += exceptions

INCLUDEPATH += ../
INCLUDEPATH += ../../
INCLUDEPATH += c:/Projects/qwt/include

HEADERS     += ScopePoke.h
HEADERS     += ScopePlotTest.h

SOURCES	+= main.cpp
SOURCES     += ScopePoke.cpp
SOURCES     += ScopePlotTest.cpp

FORMS       += ScopePlotTestBase.ui
