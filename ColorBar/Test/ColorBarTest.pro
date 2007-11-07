TEMPLATE = vcapp

LANGUAGE	= C++

CONFIG += qt 
CONFIG += thread
CONFIG += warn_on 
CONFIG += exceptions

CONFIG(release, debug|release) {
  LIBS += ../Release/ColorBar.lib
} else {
  LIBS += ../Debug/ColorBard.lib
}

INCLUDEPATH += ../
INCLUDEPATH += ../../

SOURCES += main.cpp
SOURCES += ColorBarTest.cpp

HEADERS += ColorBarTest.h

FORMS   += ColorBarTest.ui



