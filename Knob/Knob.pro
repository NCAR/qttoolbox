TEMPLATE	= vclib
DLLDESTDIR += c:/Windows/System32
DLLDESTDIR += $(QTDIR)/Plugins/Designer
LIBS       += c:/Projects/qwt/lib/qwt500.lib

LANGUAGE	= C++

CONFIG += debug
CONFIG += qt 
CONFIG += thread
CONFIG += plugin
CONFIG += warn_on 
CONFIG += exceptions

INCLUDEPATH += ../
INCLUDEPATH += c:/Projects/qwt/include

HEADERS	+= KnobPlugin.h
HEADERS     += Knob.h

SOURCES	+= KnobPlugin.cpp
SOURCES	+= Knob.cpp

FORMS	      += KnobBase.ui
