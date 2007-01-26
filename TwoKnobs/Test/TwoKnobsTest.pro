TEMPLATE	= vcapp
LANGUAGE	= C++

CONFIG += debug_and_release
CONFIG += qt 
CONFIG += thread
CONFIG += warn_on 
CONFIG += exceptions

CONFIG(release, debug|release) {
  LIBS  += ../release/TwoKnobs.lib
  LIBS  += $(QTDIR)/lib/qwt5.lib
} else {
  LIBS  += ../Debug/TwoKnobsd.lib
  LIBS  += $(QTDIR)/lib/qwt5d.lib
}

INCLUDEPATH += ../
INCLUDEPATH += ../../
INCLUDEPATH += c:/Projects/qwt/src

SOURCES	+= main.cpp

FORMS += TwoKnobsTest.ui



