#-------------------------------------------------
#
# Project created by QtCreator 2012-01-18T14:13:19
#
#-------------------------------------------------

QT       += core gui sql xml


TARGET = Weather_fs
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    preference.cpp \
    xmlltree.cpp \
    wps_set_dlg.cpp \
    wpsrun_dlg.cpp \
    dlg_namelist_edit.cpp \
    special_param_dlg.cpp \
    SharedFiles/xml_namelist.cpp \
    SharedFiles/wpsrun.cpp \
    SharedFiles/special_param.cpp \
    SharedFiles/specarr.cpp \
    SharedFiles/dbengine.cpp \
    SharedFiles/little_r_fm12.cpp \
    raw_pgp_dlg.cpp

HEADERS  += mainwindow.h \
    Preferences/preference.h \
    preference.h \
    xmlltree.h \
    wps_set_dlg.h \
    wpsrun_dlg.h \
    dlg_namelist_edit.h \
    special_param_dlg.h \
    SharedFiles/xml_namelist.h \
    SharedFiles/wpsrun.h \
    SharedFiles/special_param.h \
    SharedFiles/specarr.h \
    SharedFiles/dbengine.h \
    SharedFiles/little_r_fm12.h \
    raw_pgp_dlg.h

FORMS    += mainwindow.ui \
    wps_set_dlg.ui \
    wpsrun_dlg.ui \
    special_param_dlg.ui \
    raw_pgp_dlg.ui

RESOURCES += \
    main_res.qrc


unix:!macx: LIBS += -L$$PWD/../../../../../nfs/Magistral/NetCDF/lib64/ -lnetcdf_c++

INCLUDEPATH += $$PWD/../../../../../nfs/Magistral/NetCDF/include
DEPENDPATH += $$PWD/../../../../../nfs/Magistral/NetCDF/include
