TEMPLATE	= vclib
LANGUAGE	= C++

CONFIG += designer
CONFIG += plugin
CONFIG += thread
CONFIG += warn_on 
CONFIG += exceptions

win32 {
  CONFIG(debug, debug|release) {
    TARGET = ColorBard
    DLLDESTDIR += $(QTDIR)/lib
  } else {
    TARGET = ColorBar
    DLLDESTDIR += $(QTDIR)/lib
    DLLDESTDIR += $(QTDIR)/Plugins/Designer
  }
}

INCLUDEPATH += ../


HEADERS += ColorBar.h
HEADERS += ColorMap.h
HEADERS += ColorBarPlugin.h

SOURCES += ColorBar.cpp
SOURCES += ColorMap.cpp
SOURCES += ColorBarPlugin.cpp

