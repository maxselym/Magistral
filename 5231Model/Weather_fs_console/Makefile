#############################################################################
# Makefile for building: Weather_fs_console
# Generated by qmake (2.01a) (Qt 4.8.4) on: Mon Jun 17 16:20:51 2013
# Project:  Weather_fs_console.pro
# Template: app
# Command: /usr/bin/qmake -spec /usr/share/qt4/mkspecs/linux-g++-64 CONFIG+=debug -o Makefile Weather_fs_console.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_WEBKIT -DQT_SQL_LIB -DQT_XML_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -m64 -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -m64 -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++-64 -I. -I/usr/include/QtCore -I/usr/include/QtXml -I/usr/include/QtSql -I/usr/include -I/nfs/Magistral/NetCDF/include -I.
LINK          = g++
LFLAGS        = -m64
LIBS          = $(SUBLIBS)  -L/usr/lib64 -L/home/nfs/Magistral/5231Model/Weather_fs_console/../../../../../nfs/Magistral/NetCDF/lib64/ -lnetcdf_c++ -lQtSql -L/usr/lib64 -lQtXml -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		wpsrun_console.cpp \
		SharedFiles/xml_namelist.cpp \
		SharedFiles/wpsrun.cpp \
		SharedFiles/special_param.cpp \
		SharedFiles/specarr.cpp \
		SharedFiles/dbengine.cpp \
		SharedFiles/little_r_fm12.cpp moc_wpsrun_console.cpp \
		moc_xml_namelist.cpp \
		moc_wpsrun.cpp \
		moc_special_param.cpp \
		moc_specarr.cpp \
		moc_dbengine.cpp \
		moc_little_r_fm12.cpp
OBJECTS       = main.o \
		wpsrun_console.o \
		xml_namelist.o \
		wpsrun.o \
		special_param.o \
		specarr.o \
		dbengine.o \
		little_r_fm12.o \
		moc_wpsrun_console.o \
		moc_xml_namelist.o \
		moc_wpsrun.o \
		moc_special_param.o \
		moc_specarr.o \
		moc_dbengine.o \
		moc_little_r_fm12.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_phonon.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		Weather_fs_console.pro
QMAKE_TARGET  = Weather_fs_console
DESTDIR       = 
TARGET        = Weather_fs_console

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)
	{ test -n "$(DESTDIR)" && DESTDIR="$(DESTDIR)" || DESTDIR=.; } && test $$(gdb --version | sed -e 's,[^0-9]\+\([0-9]\)\.\([0-9]\).*,\1\2,;q') -gt 72 && gdb --nx --batch --quiet -ex 'set confirm off' -ex "save gdb-index $$DESTDIR" -ex quit '$(TARGET)' && test -f $(TARGET).gdb-index && objcopy --add-section '.gdb_index=$(TARGET).gdb-index' --set-section-flags '.gdb_index=readonly' '$(TARGET)' '$(TARGET)' && rm -f $(TARGET).gdb-index || true

Makefile: Weather_fs_console.pro  /usr/share/qt4/mkspecs/linux-g++-64/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_phonon.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib64/libQtSql.prl \
		/usr/lib64/libQtCore.prl \
		/usr/lib64/libQtXml.prl
	$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++-64 CONFIG+=debug -o Makefile Weather_fs_console.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/modules/qt_phonon.pri:
/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/debug.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib64/libQtSql.prl:
/usr/lib64/libQtCore.prl:
/usr/lib64/libQtXml.prl:
qmake:  FORCE
	@$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++-64 CONFIG+=debug -o Makefile Weather_fs_console.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/Weather_fs_console1.0.0 || $(MKDIR) .tmp/Weather_fs_console1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/Weather_fs_console1.0.0/ && $(COPY_FILE) --parents wpsrun_console.h SharedFiles/xml_namelist.h SharedFiles/wpsrun.h SharedFiles/special_param.h SharedFiles/specarr.h SharedFiles/dbengine.h SharedFiles/dbengine.h SharedFiles/little_r_fm12.h .tmp/Weather_fs_console1.0.0/ && $(COPY_FILE) --parents main.cpp wpsrun_console.cpp SharedFiles/xml_namelist.cpp SharedFiles/wpsrun.cpp SharedFiles/special_param.cpp SharedFiles/specarr.cpp SharedFiles/dbengine.cpp SharedFiles/little_r_fm12.cpp .tmp/Weather_fs_console1.0.0/ && (cd `dirname .tmp/Weather_fs_console1.0.0` && $(TAR) Weather_fs_console1.0.0.tar Weather_fs_console1.0.0 && $(COMPRESS) Weather_fs_console1.0.0.tar) && $(MOVE) `dirname .tmp/Weather_fs_console1.0.0`/Weather_fs_console1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/Weather_fs_console1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_wpsrun_console.cpp moc_xml_namelist.cpp moc_wpsrun.cpp moc_special_param.cpp moc_specarr.cpp moc_dbengine.cpp moc_dbengine.cpp moc_little_r_fm12.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_wpsrun_console.cpp moc_xml_namelist.cpp moc_wpsrun.cpp moc_special_param.cpp moc_specarr.cpp moc_dbengine.cpp moc_dbengine.cpp moc_little_r_fm12.cpp
moc_wpsrun_console.cpp: ../Weather_fs/SharedFiles/wpsrun.h \
		../Weather_fs/SharedFiles/xml_namelist.h \
		../Weather_fs/SharedFiles/special_param.h \
		/nfs/Magistral/NetCDF/include/netcdfcpp.h \
		../../NetCDF/include/ncvalues.h \
		../../NetCDF/include/netcdf.h \
		../Weather_fs/SharedFiles/specarr.h \
		../Weather_fs/SharedFiles/dbengine.h \
		wpsrun_console.h
	/usr/bin/moc $(DEFINES) $(INCPATH) wpsrun_console.h -o moc_wpsrun_console.cpp

moc_xml_namelist.cpp: SharedFiles/xml_namelist.h
	/usr/bin/moc $(DEFINES) $(INCPATH) SharedFiles/xml_namelist.h -o moc_xml_namelist.cpp

moc_wpsrun.cpp: ../Weather_fs/SharedFiles/xml_namelist.h \
		../Weather_fs/SharedFiles/special_param.h \
		/nfs/Magistral/NetCDF/include/netcdfcpp.h \
		../../NetCDF/include/ncvalues.h \
		../../NetCDF/include/netcdf.h \
		../Weather_fs/SharedFiles/specarr.h \
		../Weather_fs/SharedFiles/dbengine.h \
		SharedFiles/wpsrun.h
	/usr/bin/moc $(DEFINES) $(INCPATH) SharedFiles/wpsrun.h -o moc_wpsrun.cpp

moc_special_param.cpp: /nfs/Magistral/NetCDF/include/netcdfcpp.h \
		../../NetCDF/include/ncvalues.h \
		../../NetCDF/include/netcdf.h \
		../Weather_fs/SharedFiles/specarr.h \
		../Weather_fs/SharedFiles/dbengine.h \
		SharedFiles/special_param.h
	/usr/bin/moc $(DEFINES) $(INCPATH) SharedFiles/special_param.h -o moc_special_param.cpp

moc_specarr.cpp: SharedFiles/specarr.h
	/usr/bin/moc $(DEFINES) $(INCPATH) SharedFiles/specarr.h -o moc_specarr.cpp

moc_dbengine.cpp: SharedFiles/dbengine.h
	/usr/bin/moc $(DEFINES) $(INCPATH) SharedFiles/dbengine.h -o moc_dbengine.cpp

moc_dbengine.cpp: SharedFiles/dbengine.h
	/usr/bin/moc $(DEFINES) $(INCPATH) SharedFiles/dbengine.h -o moc_dbengine.cpp

moc_little_r_fm12.cpp: ../Weather_fs/SharedFiles/dbengine.h \
		SharedFiles/little_r_fm12.h
	/usr/bin/moc $(DEFINES) $(INCPATH) SharedFiles/little_r_fm12.h -o moc_little_r_fm12.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

main.o: main.cpp wpsrun_console.h \
		../Weather_fs/SharedFiles/wpsrun.h \
		../Weather_fs/SharedFiles/xml_namelist.h \
		../Weather_fs/SharedFiles/special_param.h \
		/nfs/Magistral/NetCDF/include/netcdfcpp.h \
		../../NetCDF/include/ncvalues.h \
		../../NetCDF/include/netcdf.h \
		../Weather_fs/SharedFiles/specarr.h \
		../Weather_fs/SharedFiles/dbengine.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

wpsrun_console.o: wpsrun_console.cpp wpsrun_console.h \
		../Weather_fs/SharedFiles/wpsrun.h \
		../Weather_fs/SharedFiles/xml_namelist.h \
		../Weather_fs/SharedFiles/special_param.h \
		/nfs/Magistral/NetCDF/include/netcdfcpp.h \
		../../NetCDF/include/ncvalues.h \
		../../NetCDF/include/netcdf.h \
		../Weather_fs/SharedFiles/specarr.h \
		../Weather_fs/SharedFiles/dbengine.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o wpsrun_console.o wpsrun_console.cpp

xml_namelist.o: SharedFiles/xml_namelist.cpp ../Weather_fs/SharedFiles/xml_namelist.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o xml_namelist.o SharedFiles/xml_namelist.cpp

wpsrun.o: SharedFiles/wpsrun.cpp ../Weather_fs/SharedFiles/wpsrun.h \
		../Weather_fs/SharedFiles/xml_namelist.h \
		../Weather_fs/SharedFiles/special_param.h \
		/nfs/Magistral/NetCDF/include/netcdfcpp.h \
		../../NetCDF/include/ncvalues.h \
		../../NetCDF/include/netcdf.h \
		../Weather_fs/SharedFiles/specarr.h \
		../Weather_fs/SharedFiles/dbengine.h \
		../Weather_fs/SharedFiles/little_r_fm12.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o wpsrun.o SharedFiles/wpsrun.cpp

special_param.o: SharedFiles/special_param.cpp ../Weather_fs/SharedFiles/special_param.h \
		/nfs/Magistral/NetCDF/include/netcdfcpp.h \
		../../NetCDF/include/ncvalues.h \
		../../NetCDF/include/netcdf.h \
		../Weather_fs/SharedFiles/specarr.h \
		../Weather_fs/SharedFiles/dbengine.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o special_param.o SharedFiles/special_param.cpp

specarr.o: SharedFiles/specarr.cpp ../Weather_fs/SharedFiles/specarr.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o specarr.o SharedFiles/specarr.cpp

dbengine.o: SharedFiles/dbengine.cpp ../Weather_fs/SharedFiles/dbengine.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o dbengine.o SharedFiles/dbengine.cpp

little_r_fm12.o: SharedFiles/little_r_fm12.cpp ../Weather_fs/SharedFiles/little_r_fm12.h \
		../Weather_fs/SharedFiles/dbengine.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o little_r_fm12.o SharedFiles/little_r_fm12.cpp

moc_wpsrun_console.o: moc_wpsrun_console.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_wpsrun_console.o moc_wpsrun_console.cpp

moc_xml_namelist.o: moc_xml_namelist.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_xml_namelist.o moc_xml_namelist.cpp

moc_wpsrun.o: moc_wpsrun.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_wpsrun.o moc_wpsrun.cpp

moc_special_param.o: moc_special_param.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_special_param.o moc_special_param.cpp

moc_specarr.o: moc_specarr.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_specarr.o moc_specarr.cpp

moc_dbengine.o: moc_dbengine.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_dbengine.o moc_dbengine.cpp

moc_little_r_fm12.o: moc_little_r_fm12.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_little_r_fm12.o moc_little_r_fm12.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

