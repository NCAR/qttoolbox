TEMPLATE	= vcapp
LANGUAGE	= C++

CONFIG += qt 
CONFIG += thread
CONFIG += warn_on 
CONFIG += exceptions

CONFIG(release, debug|release) {
  LIBS  += ../release/Knob.lib
  LIBS  += $(QTDIR)/lib/qwt5.lib
} else {
  LIBS  += ../Debug/Knobd.lib
  LIBS  += $(QTDIR)/lib/qwt5d.lib
}

INCLUDEPATH += ../
INCLUDEPATH += ../../
INCLUDEPATH += c:/Projects/qwt/src

SOURCES	+= main.cpp

FORMS += KnobTest.ui
