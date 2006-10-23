TEMPLATE	= vclib
LANGUAGE	= C++

CONFIG	+= debug

INCLUDEPATH	+= ../

HEADERS	+= ScrollZoomer.h

SOURCES	+= ScrollZoomer.cpp

FORMS	= ScopePlotBase.ui \
	.\ScopePlotBase.ui

DLLDESTDIR += c:/Windows/System32
DLLDESTDIR += $(QTDIR)/Plugins/Designer
LIBS       += c:/Projects/qwt/lib/qwt500.lib

CONFIG += qt 
CONFIG += thread
CONFIG += plugin
CONFIG += warn_on 
CONFIG += exceptions

INCLUDEPATH += c:/Projects/qwt/include

HEADERS     += ScrollBar.h
HEADERS     += ScopePlot.h
HEADERS     += ScopePlotPlugin.h

SOURCES     += ScrollBar.cpp
SOURCES	+= ScopePlotPlugin.cpp
SOURCES	+= ScopePlot.cpp

