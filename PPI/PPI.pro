TEMPLATE	= vclib
LANGUAGE	= C++

CONFIG += debug
CONFIG += qt 
CONFIG += thread
CONFIG += plugin
CONFIG += warn_on 
CONFIG += exceptions

HEADERS	+= PPI.h
HEADERS     += PPIPlugin.h

SOURCES	+= PPI.cc
SOURCES     += PPIPlugin.cpp

FORMS	= PPIBase.ui
INCLUDEPATH += ../
INCLUDEPATH += ../../

DLLDESTDIR += c:/Windows/System32
DLLDESTDIR += $(QTDIR)/Plugins/Designer
LIBS           += ../ColorBar/Debug/ColorBar.lib
LIBS           += opengl32.lib
LIBS           += glu32.lib







