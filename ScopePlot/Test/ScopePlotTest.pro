TEMPLATE	= vcapp

LANGUAGE	= C++

CONFIG += qt 
CONFIG += thread
CONFIG += warn_on 
CONFIG += exceptions

CONFIG(release, debug|release) {
  LIBS  += ../release/ScopePlot.lib
  LIBS  += $(QTDIR)/lib/qwt5.lib
} else {
  LIBS  += ../Debug/ScopePlotd.lib
  LIBS  += $(QTDIR)/lib/qwt5d.lib
}

INCLUDEPATH += ../
INCLUDEPATH += ../../
INCLUDEPATH += c:/Projects/qwt/src


HEADERS     += ScopePoke.h
HEADERS     += ScopePlotTest.h

SOURCES	+= main.cpp
SOURCES     += ScopePoke.cpp
SOURCES     += ScopePlotTest.cpp

FORMS       += ScopePlotTest.ui

