win32 {
   TEMPLATE	= vcapp
   LIBS       += c:/Projects/lib/qwt500.lib
   LIBS       += ../ScopePlot.lib
}

LANGUAGE	= C++

CONFIG += debug
CONFIG += qt 
CONFIG += thread
CONFIG += warn_on 
CONFIG += exceptions

INCLUDEPATH += ../
INCLUDEPATH += c:/Projects/qwt/include

SOURCES	+= main.cpp

