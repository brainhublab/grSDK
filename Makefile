#############################################################################
# Makefile for building: bin/GRVisualization
# Generated by qmake (2.01a) (Qt 4.8.6) on: Sun Apr 2 14:46:21 2017
# Project:  config/GRVisualisation.pro
# Template: app
# Command: /usr/bin/qmake -o Makefile config/GRVisualisation.pro
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
		src/grVisualization.cpp \
		src/grBufferManager.cpp \
		src/GRGLWidget.cpp \
		src/grHand.cpp \
		src/grRender.cpp \
		externAssets/qcustomplot/qcustomplot.cpp \
		src/grDataPlotter.cpp \
		src/grConnection.cpp build/generated_files/moc_grVisualization.cpp \
		build/generated_files/moc_grBufferManager.cpp \
		build/generated_files/moc_GRGLWidget.cpp \
		build/generated_files/moc_qcustomplot.cpp \
		build/generated_files/moc_grDataPlotter.cpp
OBJECTS       = build/main.o \
		build/grVisualization.o \
		build/grBufferManager.o \
		build/GRGLWidget.o \
		build/grHand.o \
		build/grRender.o \
		build/qcustomplot.o \
		build/grDataPlotter.o \
		build/grConnection.o \
		build/moc_grVisualization.o \
		build/moc_grBufferManager.o \
		build/moc_GRGLWidget.o \
		build/moc_qcustomplot.o \
		build/moc_grDataPlotter.o
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
		config/GRVisualisation.pro
QMAKE_TARGET  = GRVisualization
DESTDIR       = bin/
TARGET        = bin/GRVisualization

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

$(TARGET): ui_grVisualisation.h $(OBJECTS)  
	@$(CHK_DIR_EXISTS) bin/ || $(MKDIR) bin/ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: config/GRVisualisation.pro  /usr/share/qt4/mkspecs/default/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
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
	$(QMAKE) -o Makefile config/GRVisualisation.pro
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
	@$(QMAKE) -o Makefile config/GRVisualisation.pro

dist: 
	@$(CHK_DIR_EXISTS) build/GRVisualization1.0.0 || $(MKDIR) build/GRVisualization1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) build/GRVisualization1.0.0/ && $(COPY_FILE) --parents inc/grVisualization.h inc/ui_GRVisualization.h inc/grBufferManager.h inc/GRGLWidget.h inc/grHandNode.h inc/grHand.h inc/renderConfig.h inc/grRender.h externAssets/qcustomplot/qcustomplot.h inc/grDataPlotter.h inc/grConnection.h build/GRVisualization1.0.0/ && $(COPY_FILE) --parents src/main.cpp src/grVisualization.cpp src/grBufferManager.cpp src/GRGLWidget.cpp src/grHand.cpp src/grRender.cpp externAssets/qcustomplot/qcustomplot.cpp src/grDataPlotter.cpp src/grConnection.cpp build/GRVisualization1.0.0/ && $(COPY_FILE) --parents config/grVisualisation.ui build/GRVisualization1.0.0/ && (cd `dirname build/GRVisualization1.0.0` && $(TAR) GRVisualization1.0.0.tar GRVisualization1.0.0 && $(COMPRESS) GRVisualization1.0.0.tar) && $(MOVE) `dirname build/GRVisualization1.0.0`/GRVisualization1.0.0.tar.gz . && $(DEL_FILE) -r build/GRVisualization1.0.0


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

compiler_moc_header_make_all: build/generated_files/moc_grVisualization.cpp build/generated_files/moc_grBufferManager.cpp build/generated_files/moc_GRGLWidget.cpp build/generated_files/moc_qcustomplot.cpp build/generated_files/moc_grDataPlotter.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) build/generated_files/moc_grVisualization.cpp build/generated_files/moc_grBufferManager.cpp build/generated_files/moc_GRGLWidget.cpp build/generated_files/moc_qcustomplot.cpp build/generated_files/moc_grDataPlotter.cpp
build/generated_files/moc_grVisualization.cpp: inc/grDataPlotter.h \
		inc/GRGLWidget.h \
		inc/grRender.h \
		inc/grHand.h \
		inc/grHandNode.h \
		inc/renderConfig.h \
		inc/grBufferManager.h \
		inc/grVisualization.h
	/usr/bin/moc $(DEFINES) $(INCPATH) inc/grVisualization.h -o build/generated_files/moc_grVisualization.cpp

build/generated_files/moc_grBufferManager.cpp: inc/GRGLWidget.h \
		inc/grRender.h \
		inc/grHand.h \
		inc/grHandNode.h \
		inc/renderConfig.h \
		inc/grBufferManager.h
	/usr/bin/moc $(DEFINES) $(INCPATH) inc/grBufferManager.h -o build/generated_files/moc_grBufferManager.cpp

build/generated_files/moc_GRGLWidget.cpp: inc/grRender.h \
		inc/grHand.h \
		inc/grHandNode.h \
		inc/renderConfig.h \
		inc/GRGLWidget.h
	/usr/bin/moc $(DEFINES) $(INCPATH) inc/GRGLWidget.h -o build/generated_files/moc_GRGLWidget.cpp

build/generated_files/moc_qcustomplot.cpp: externAssets/qcustomplot/qcustomplot.h
	/usr/bin/moc $(DEFINES) $(INCPATH) externAssets/qcustomplot/qcustomplot.h -o build/generated_files/moc_qcustomplot.cpp

build/generated_files/moc_grDataPlotter.cpp: inc/grDataPlotter.h
	/usr/bin/moc $(DEFINES) $(INCPATH) inc/grDataPlotter.h -o build/generated_files/moc_grDataPlotter.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_grVisualisation.h
compiler_uic_clean:
	-$(DEL_FILE) ui_grVisualisation.h
ui_grVisualisation.h: config/grVisualisation.ui
	/usr/bin/uic config/grVisualisation.ui -o ui_grVisualisation.h

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

build/grVisualization.o: src/grVisualization.cpp ui_grVisualisation.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/grVisualization.o src/grVisualization.cpp

build/grBufferManager.o: src/grBufferManager.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/grBufferManager.o src/grBufferManager.cpp

build/GRGLWidget.o: src/GRGLWidget.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/GRGLWidget.o src/GRGLWidget.cpp

build/grHand.o: src/grHand.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/grHand.o src/grHand.cpp

build/grRender.o: src/grRender.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/grRender.o src/grRender.cpp

build/qcustomplot.o: externAssets/qcustomplot/qcustomplot.cpp externAssets/qcustomplot/qcustomplot.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/qcustomplot.o externAssets/qcustomplot/qcustomplot.cpp

build/grDataPlotter.o: src/grDataPlotter.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/grDataPlotter.o src/grDataPlotter.cpp

build/grConnection.o: src/grConnection.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/grConnection.o src/grConnection.cpp

build/moc_grVisualization.o: build/generated_files/moc_grVisualization.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_grVisualization.o build/generated_files/moc_grVisualization.cpp

build/moc_grBufferManager.o: build/generated_files/moc_grBufferManager.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_grBufferManager.o build/generated_files/moc_grBufferManager.cpp

build/moc_GRGLWidget.o: build/generated_files/moc_GRGLWidget.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_GRGLWidget.o build/generated_files/moc_GRGLWidget.cpp

build/moc_qcustomplot.o: build/generated_files/moc_qcustomplot.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_qcustomplot.o build/generated_files/moc_qcustomplot.cpp

build/moc_grDataPlotter.o: build/generated_files/moc_grDataPlotter.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_grDataPlotter.o build/generated_files/moc_grDataPlotter.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

