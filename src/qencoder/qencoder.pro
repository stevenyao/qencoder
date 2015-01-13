TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ../../include
INCLUDEPATH += ../../libs/x264

QMAKE_LIBDIR += ../../libs/x264/libs/32bit

LIBS += libx264.lib

CONFIG += console

PRECOMPILED_HEADER = stable.h

CONFIG(debug, debug)
{
    DESTDIR = ../../bin/debug
}

CONFIG(release, release)
{
    DESTDIR = ../../bin/release
}

# Input
SOURCES += main.cpp
SOURCES += UIMainWindow.cpp
SOURCES += VideoEncoder.cpp
SOURCES += VideoEncoder_x264.cpp

HEADERS += UIMainWindow.h
HEADERS += VideoEncoder.h
HEADERS += VideoEncoder_x264.h
