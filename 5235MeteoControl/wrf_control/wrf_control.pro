#-------------------------------------------------
#
# Project created by QtCreator 2012-05-21T15:32:01
#
#-------------------------------------------------

QT       += core gui xml

TARGET = wrf_control
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    wrffileread.cpp \
    xml_namelist.cpp

HEADERS  += dialog.h \
    wrffileread.h \
    xml_namelist.h

FORMS    += dialog.ui






win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../NetCDF/lib64/release/ -lnetcdf_c++
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../NetCDF/lib64/debug/ -lnetcdf_c++
else:symbian: LIBS += -lnetcdf_c++
else:unix: LIBS += -L$$PWD/../../NetCDF/lib64/ -lnetcdf_c++

INCLUDEPATH += $$PWD/../../NetCDF/include
DEPENDPATH += $$PWD/../../NetCDF/include
