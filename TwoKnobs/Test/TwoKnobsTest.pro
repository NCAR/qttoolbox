TEMPLATE	= vcapp
LANGUAGE	= C++

CONFIG	+= debug

INCLUDEPATH	+= ../

SOURCES	+= main.cpp

FORMS	= ScopePlot.pro \
	c:\Projects\QtToolbox\ScopePlot\ScopePlotBase.ui

win32 {
         LIBS       += ../TwoKnobs.lib
}


CONFIG += qt 
CONFIG += thread
CONFIG += warn_on 
CONFIG += exceptions

INCLUDEPATH += ../../


