SRC_DIR=./src
INC_DIR=./inc
LIB_DIR=./lib
BUILD_DIR=./build
BIN_DIR=./bin
UI_DIR = ./inc/ui
EXTERN_ASSETS_DIR=$(LIB_DIR)/externAssets
ATTITUDE_LIB = $(EXTERN_ASSETS_DIR)/attitude_estimator/lib/
ATTITUDE_INC = $(EXTERN_ASSETS_DIR)/attitude_estimator/src/

IMGUI_FILES = $(UI_DIR)/imgui/imgui.cpp $(UI_DIR)/imgui/imgui_draw.cpp $(SRC_DIR)/ui/imgui_impl_sdl.cpp

KALMAN_DIR=$(LIB_DIR)$(EXTERN_ASSETS_DIR)/kalman

CPP=g++
LIB_FLAGS=-ldbus-1 -lconfig++ -lattitude_estimator
CPP_FLAGS= -g -std=c++11 -pthread #-Wall #-pedantic



DBUS_INC=-I/usr/include/dbus-1.0 -I/usr/lib64/dbus-1.0/include
DBUS_LIB=-L/usr/lib64 -L$(ATTITUDE_LIB)
INCLUDE=-I$(INC_DIR) $(DBUS_INC) -I$(ATTITUDE_INC)

EXTERN_ASSETS := extern_assets

main: $(BIN_DIR)/main

BINARIES=main

all:$(BINARIES) 


$(BIN_DIR)/%: $(BUILD_DIR)/gestusConnection.o  $(BUILD_DIR)/%.o 
	$(CPP) $(CPP_FLAGS) $(DBUS_LIB) $(INCLUDE) $^ -o $@ $(LIB_FLAGS)


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPP) $(CPP_FLAGS) $(DBUS_LIB)  $(INCLUDE) -c $^ -o $@ $(LIB_FLAGS)

#extern_assets:
#	$(MAKE) -f $(EXTERN_ASSETS_DIR)/Makefile	
#clean rules

#.PHONY: clean cleanobj cleanbin

clean:
	rm -vf $(BIN_DIR)/*
	rm -vf $(BUILD_DIR)/*

cleanobj:
	rm -vf $(BUILD_DIR)/*
	
cleanbin:
	rm -vf $(BIN_DIR)/*
