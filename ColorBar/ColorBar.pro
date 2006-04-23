TEMPLATE	= vclib
LANGUAGE	= C++

CONFIG	+= debug

INCLUDEPATH	+= ../

HEADERS     += ColorBar.h
HEADERS     += ColorMap.h
HEADERS	+= ColorBarPlugin.h

SOURCES	+= ColorBar.cpp
SOURCES     += ColorMap.cpp
SOURCES	+= ColorBarPlugin.cpp

FORMS	= ColorBarBase.ui

win32 {
      DLLDESTDIR += c:/Windows/System32
   DLLDESTDIR += $(QTDIR)/Plugins/Designer
}


CONFIG += qt 
CONFIG += thread
CONFIG += plugin
CONFIG += warn_on 
CONFIG += exceptions
