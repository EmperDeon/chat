#-------------------------------------------------
#
# Project created by QtCreator 2016-02-22T16:18:57
#
#-------------------------------------------------

QT       += core gui network
QT += multimedia
CONFIG  += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES +=\
    cmain.cpp \
    cserver.cpp \
    cutils.cpp \
    cwgt.cpp \
    cclient.cpp

HEADERS  += \
    cdefines.h \
    cserver.h \
    cutils.h \
    cwgt.h \
    cclient.h

DISTFILES += \
    Commands
RC_FILE = res.rc

RESOURCES += \
    res.qrc
