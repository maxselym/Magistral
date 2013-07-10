#-------------------------------------------------
#
# Project created by QtCreator 2012-05-14T22:31:28
#
#-------------------------------------------------

QT       += core xml network sql

QT       -= gui

TARGET = meteoCentre
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    preferences.cpp \
    wpsrun.cpp \
    ftpclient.cpp \
    psqldriver.cpp \
    csvtopsql.cpp \
    psqltocsv.cpp

HEADERS += \
    preferences.h \
    wpsrun.h \
    ftpclient.h \
    psqldriver.h \
    csvtopsql.h \
    psqltocsv.h






