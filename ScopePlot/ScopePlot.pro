win32 {
   TEMPLATE	= vclib
#   DLLDESTDIR += c:/Windows/System32
#   DLLDESTDIR += $(QTDIR)/Plugins/Designer
   LIBS       += c:/Projects/lib/qwt500.lib
}

LANGUAGE	= C++

CONFIG += debug
CONFIG += qt 
CONFIG += thread
#CONFIG += plugin
CONFIG += warn_on 
CONFIG += exceptions

INCLUDEPATH += ../
INCLUDEPATH += c:/Projects/qwt/include

#HEADERS	+= KnobPlugin.h
HEADERS     += ScrollZoomer.h
HEADERS     += ScrollBar.h
HEADERS     += ScopePlot.h

SOURCES     += ScrollZoomer.cpp
SOURCES     += ScrollBar.cpp
SOURCES     += IncrementalPlot.cpp
SOURCES	+= ScopePlot.cpp

FORMS	      += ScopePlotBase.ui
