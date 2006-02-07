win32 {
   TEMPLATE	= vcapp
   LIBS       += c:/Projects/lib/qwt500.lib
   LIBS       += ../TwoKnobs.lib
}

LANGUAGE	= C++

CONFIG += debug
CONFIG += qt 
CONFIG += thread
CONFIG += warn_on 
CONFIG += exceptions

INCLUDEPATH += ../
INCLUDEPATH += ../../

SOURCES	+= main.cpp

FORMS	= TwoKnobsTest.ui 
