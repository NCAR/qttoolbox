TEMPLATE	= vclib
LANGUAGE	= C++

CONFIG += designer
CONFIG += plugin
CONFIG += thread
CONFIG += warn_on 
CONFIG += exceptions

win32 {
  CONFIG(debug, debug|release) {
    TARGET = StatusGauged
    DLLDESTDIR += $(QTDIR)/lib
  } else {
    TARGET = StatusGauge
    DLLDESTDIR += $(QTDIR)/lib
    DLLDESTDIR += $(QTDIR)/Plugins/Designer
  }
}

INCLUDEPATH += ../


HEADERS += StatusGauge.h
HEADERS += StatusGaugePlugin.h

SOURCES += StatusGauge.cpp
SOURCES += StatusGaugePlugin.cpp

