win32 {
   TEMPLATE	= vclib
   DLLDESTDIR += c:/Windows/System32
   DLLDESTDIR += $(QTDIR)/Plugins/Designer
   LIBS       += c:/Projects/lib/qwt500.lib
   LIBS       += ../Knob/Knob.lib
}

LANGUAGE	= C++

CONFIG += debug
CONFIG += qt 
CONFIG += thread
CONFIG += plugin
CONFIG += warn_on 
CONFIG += exceptions

INCLUDEPATH += ../
INCLUDEPATH += c:/Projects/qwt/include

HEADERS	+= TwoKnobsPlugin.h
HEADERS     += TwoKnobs.h

SOURCES	+= TwoKnobsPlugin.cpp
SOURCES	+= TwoKnobs.cpp

FORMS	= TwoKnobsBase.ui
