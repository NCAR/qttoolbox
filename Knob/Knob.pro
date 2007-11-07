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
  TARGET = Knob
  LIBS  += $(QTDIR)/lib/qwt5.lib
  DLLDESTDIR += $(QTDIR)/lib
  DLLDESTDIR += $(QTDIR)/Plugins/Designer
} else {
  TARGET = Knobd
  LIBS  += $(QTDIR)/lib/qwt5d.lib
  DLLDESTDIR += $(QTDIR)/lib
}

INCLUDEPATH += ../
INCLUDEPATH += c:/Projects/qwt/src

HEADERS	+= KnobPlugin.h
HEADERS 	+= Knob.h

SOURCES	+= KnobPlugin.cpp
SOURCES	+= Knob.cpp

FORMS      += Knob.ui


