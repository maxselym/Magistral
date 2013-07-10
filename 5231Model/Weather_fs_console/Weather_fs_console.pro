QT       += core xml sql

QT       -= gui

TARGET = Weather_fs_console
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    wpsrun_console.cpp \
    SharedFiles/xml_namelist.cpp \
    SharedFiles/wpsrun.cpp \
    SharedFiles/special_param.cpp \
    SharedFiles/specarr.cpp \
    SharedFiles/dbengine.cpp \
    SharedFiles/little_r_fm12.cpp

HEADERS += \
    wpsrun_console.h \
    SharedFiles/xml_namelist.h \
    SharedFiles/wpsrun.h \
    SharedFiles/special_param.h \
    SharedFiles/specarr.h \
    SharedFiles/dbengine.h \
    SharedFiles/dbengine.h \
    SharedFiles/little_r_fm12.h


unix:!macx: LIBS += -L$$PWD/../../../../../nfs/Magistral/NetCDF/lib64/ -lnetcdf_c++

INCLUDEPATH += $$PWD/../../../../../nfs/Magistral/NetCDF/include
DEPENDPATH += $$PWD/../../../../../nfs/Magistral/NetCDF/include
