#-------------------------------------------------
#
# Project created by QtCreator 2016-02-22T16:19:17
#
#-------------------------------------------------

QT       += core gui network
CONFIG  += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server
TEMPLATE = app


SOURCES +=\
    sconnect.cpp \
    sutils.cpp \
    swgt.cpp \
    smain.cpp \
    sserver.cpp \
    sclients.cpp

HEADERS  += \
		sconnect.h \
    sdefines.h \
    sutils.h \
    swgt.h \
    sserver.h \
    sclients.h
