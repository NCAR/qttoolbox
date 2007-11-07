TEMPLATE	= vclib

LANGUAGE	= C++

# do not specify CONFIG += release or CONFIG += debug 
# as this fouls up the library specifications
# for different configurations. 
# However, not adding one or the other means tha the
# project file name will take the name of the debug TARGET...

CONFIG += designer
CONFIG += plugin
CONFIG += thread
CONFIG += warn_on 
CONFIG += exceptions

CONFIG(release, debug|release) {
  TARGET = TwoKnobs
  LIBS  += $(QTDIR)/lib/qwt5.lib
  DLLDESTDIR += $(QTDIR)/lib
  DLLDESTDIR += $(QTDIR)/Plugins/Designer
} else {
  TARGET = TwoKnobsd
  LIBS  += $(QTDIR)/lib/qwt5d.lib
  DLLDESTDIR += $(QTDIR)/lib
}

INCLUDEPATH += ../
INCLUDEPATH += c:/Projects/qwt/src

HEADERS	+= TwoKnobsPlugin.h
HEADERS 	+= TwoKnobs.h

SOURCES	+= TwoKnobsPlugin.cpp
SOURCES	+= TwoKnobs.cpp

FORMS      += TwoKnobs.ui

