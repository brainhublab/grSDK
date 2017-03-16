#############################################################################
# Makefile for building: bin/gestusVisualization
# Generated by qmake (2.01a) (Qt 4.8.6) on: Thu Mar 16 13:22:53 2017
# Project:  config/gestusVisualisation.pro
# Template: app
# Command: /usr/bin/qmake -o Makefile config/gestusVisualisation.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -D_REENTRANT -Wall -W $(DEFINES)
CXXFLAGS      = -pipe -g -std=c++11 -O2 -D_REENTRANT -Wall -W $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/default -Iconfig -I/usr/include/QtCore -I/usr/include/QtGui -I/usr/include/QtOpenGL -I/usr/include -I/usr/include/dbus-1.0 -I/usr/lib64/dbus-1.0/include -I./inc -I./externAssets/qcustomplot -I./externAssets/ -Iconfig -I/usr/X11R6/include -Ibuild/generated_files -I. -Iconfig -I.
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib64 -L/usr/X11R6/lib -L/usr/lib64 -ldbus-1 -lglut -lGLU -lQtOpenGL -L/usr/X11R6/lib -lQtGui -lQtCore -lGL -lpthread 
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

OBJECTS_DIR   = build/

####### Files

SOURCES       = src/main.cpp \
		src/gestusVisualization.cpp \
		src/gestusGLWidget.cpp \
		src/gestusHand.cpp \
		src/gestusRender.cpp \
		externAssets/qcustomplot/qcustomplot.cpp \
		src/gestusDataPlotter.cpp \
		src/gestusConnection.cpp build/generated_files/moc_gestusVisualization.cpp \
		build/generated_files/moc_gestusGLWidget.cpp \
		build/generated_files/moc_qcustomplot.cpp \
		build/generated_files/moc_gestusDataPlotter.cpp
OBJECTS       = build/main.o \
		build/gestusVisualization.o \
		build/gestusGLWidget.o \
		build/gestusHand.o \
		build/gestusRender.o \
		build/qcustomplot.o \
		build/gestusDataPlotter.o \
		build/gestusConnection.o \
		build/moc_gestusVisualization.o \
		build/moc_gestusGLWidget.o \
		build/moc_qcustomplot.o \
		build/moc_gestusDataPlotter.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/opengl.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/shared.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		config/gestusVisualisation.pro
QMAKE_TARGET  = gestusVisualization
DESTDIR       = bin/
TARGET        = bin/gestusVisualization

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

$(TARGET): ui_gestusVisualisation.h $(OBJECTS)  
	@$(CHK_DIR_EXISTS) bin/ || $(MKDIR) bin/ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: config/gestusVisualisation.pro  /usr/share/qt4/mkspecs/default/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/opengl.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/shared.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib64/libQtOpenGL.prl \
		/usr/lib64/libQtGui.prl \
		/usr/lib64/libQtCore.prl
	$(QMAKE) -o Makefile config/gestusVisualisation.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/opengl.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/shared.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib64/libQtOpenGL.prl:
/usr/lib64/libQtGui.prl:
/usr/lib64/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile config/gestusVisualisation.pro

dist: 
	@$(CHK_DIR_EXISTS) build/gestusVisualization1.0.0 || $(MKDIR) build/gestusVisualization1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) build/gestusVisualization1.0.0/ && $(COPY_FILE) --parents inc/gestusVisualization.h inc/ui_gestusVisualization.h inc/gestusGLWidget.h inc/gestusHandNode.h inc/gestusHand.h inc/dimensions.h inc/gestusRender.h externAssets/qcustomplot/qcustomplot.h inc/gestusDataPlotter.h inc/gestusConnection.h build/gestusVisualization1.0.0/ && $(COPY_FILE) --parents src/main.cpp src/gestusVisualization.cpp src/gestusGLWidget.cpp src/gestusHand.cpp src/gestusRender.cpp externAssets/qcustomplot/qcustomplot.cpp src/gestusDataPlotter.cpp src/gestusConnection.cpp build/gestusVisualization1.0.0/ && $(COPY_FILE) --parents config/gestusVisualisation.ui build/gestusVisualization1.0.0/ && (cd `dirname build/gestusVisualization1.0.0` && $(TAR) gestusVisualization1.0.0.tar gestusVisualization1.0.0 && $(COMPRESS) gestusVisualization1.0.0.tar) && $(MOVE) `dirname build/gestusVisualization1.0.0`/gestusVisualization1.0.0.tar.gz . && $(DEL_FILE) -r build/gestusVisualization1.0.0


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

compiler_moc_header_make_all: build/generated_files/moc_gestusVisualization.cpp build/generated_files/moc_gestusGLWidget.cpp build/generated_files/moc_qcustomplot.cpp build/generated_files/moc_gestusDataPlotter.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) build/generated_files/moc_gestusVisualization.cpp build/generated_files/moc_gestusGLWidget.cpp build/generated_files/moc_qcustomplot.cpp build/generated_files/moc_gestusDataPlotter.cpp
build/generated_files/moc_gestusVisualization.cpp: inc/gestusDataPlotter.h \
		inc/gestusVisualization.h
	/usr/bin/moc $(DEFINES) $(INCPATH) inc/gestusVisualization.h -o build/generated_files/moc_gestusVisualization.cpp

build/generated_files/moc_gestusGLWidget.cpp: inc/gestusRender.h \
		inc/gestusHand.h \
		inc/gestusHandNode.h \
		inc/dimensions.h \
		inc/gestusGLWidget.h
	/usr/bin/moc $(DEFINES) $(INCPATH) inc/gestusGLWidget.h -o build/generated_files/moc_gestusGLWidget.cpp

build/generated_files/moc_qcustomplot.cpp: externAssets/qcustomplot/qcustomplot.h
	/usr/bin/moc $(DEFINES) $(INCPATH) externAssets/qcustomplot/qcustomplot.h -o build/generated_files/moc_qcustomplot.cpp

build/generated_files/moc_gestusDataPlotter.cpp: inc/gestusDataPlotter.h
	/usr/bin/moc $(DEFINES) $(INCPATH) inc/gestusDataPlotter.h -o build/generated_files/moc_gestusDataPlotter.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_gestusVisualisation.h
compiler_uic_clean:
	-$(DEL_FILE) ui_gestusVisualisation.h
ui_gestusVisualisation.h: config/gestusVisualisation.ui
	/usr/bin/uic config/gestusVisualisation.ui -o ui_gestusVisualisation.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 

####### Compile

build/main.o: src/main.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/main.o src/main.cpp

build/gestusVisualization.o: src/gestusVisualization.cpp ui_gestusVisualisation.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/gestusVisualization.o src/gestusVisualization.cpp

build/gestusGLWidget.o: src/gestusGLWidget.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/gestusGLWidget.o src/gestusGLWidget.cpp

build/gestusHand.o: src/gestusHand.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/gestusHand.o src/gestusHand.cpp

build/gestusRender.o: src/gestusRender.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/gestusRender.o src/gestusRender.cpp

build/qcustomplot.o: externAssets/qcustomplot/qcustomplot.cpp externAssets/qcustomplot/qcustomplot.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/qcustomplot.o externAssets/qcustomplot/qcustomplot.cpp

build/gestusDataPlotter.o: src/gestusDataPlotter.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/gestusDataPlotter.o src/gestusDataPlotter.cpp

build/gestusConnection.o: src/gestusConnection.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/gestusConnection.o src/gestusConnection.cpp

build/moc_gestusVisualization.o: build/generated_files/moc_gestusVisualization.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_gestusVisualization.o build/generated_files/moc_gestusVisualization.cpp

build/moc_gestusGLWidget.o: build/generated_files/moc_gestusGLWidget.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_gestusGLWidget.o build/generated_files/moc_gestusGLWidget.cpp

build/moc_qcustomplot.o: build/generated_files/moc_qcustomplot.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_qcustomplot.o build/generated_files/moc_qcustomplot.cpp

build/moc_gestusDataPlotter.o: build/generated_files/moc_gestusDataPlotter.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_gestusDataPlotter.o build/generated_files/moc_gestusDataPlotter.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

