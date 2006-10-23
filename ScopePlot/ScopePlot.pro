TEMPLATE	= vclib
LANGUAGE	= C++

CONFIG	+= debug
CONFIG += qt 
CONFIG += thread
CONFIG += plugin
CONFIG += warn_on 
CONFIG += exceptions

INCLUDEPATH	+= ../
INCLUDEPATH += c:/Projects/qwt/include

HEADERS     += ScopePlot.h
HEADERS     += ScopePlotPlugin.h
HEADERS	+= ScrollZoomer.h
HEADERS     += ScrollBar.h

SOURCES	+= ScopePlot.cpp
SOURCES	+= ScopePlotPlugin.cpp
SOURCES	+= ScrollZoomer.cpp
SOURCES     += ScrollBar.cpp

FORMS	= ScopePlotBase.ui 

DESTDIR     = Debug
DLLDESTDIR += c:/Windows/System32
DLLDESTDIR += $(QTDIR)/Plugins/Designer
LIBS       += c:/Projects/qwt/lib/qwt500.lib




