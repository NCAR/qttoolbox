TEMPLATE	= vclib
LIBS       += c:/Projects/lib/qwt500.lib
LIBS       += ../Knob/Debug/Knob.lib

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
HEADERS += TwoKnobs.h

SOURCES	+= TwoKnobsPlugin.cpp
SOURCES	+= TwoKnobs.cpp

FORMS	= TwoKnobsBase.ui
DESTDIR     = Debug
DLLDESTDIR += c:/Windows/System32
DLLDESTDIR += $(QTDIR)/Plugins/Designer
