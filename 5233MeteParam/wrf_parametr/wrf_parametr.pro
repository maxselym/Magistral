#-------------------------------------------------
#
# Project created by QtCreator 2012-05-19T22:55:09
#
#-------------------------------------------------

QT       += core xml

QT       -= gui

TARGET = wrf_parametr
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    wpsrun.cpp \
    preferences.cpp \
    xml_namelist.cpp \
    wrffileread.cpp

HEADERS += \
    wpsrun.h \
    preferences.h \
    xml_namelist.h \
    wrffileread.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../NetCDF/lib64/release/ -lnetcdf_c++
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../NetCDF/lib64/debug/ -lnetcdf_c++
else:symbian: LIBS += -lnetcdf_c++
else:unix: LIBS += -L$$PWD/../../NetCDF/lib64/ -lnetcdf_c++

INCLUDEPATH += $$PWD/../../NetCDF/include
DEPENDPATH += $$PWD/../../NetCDF/include
