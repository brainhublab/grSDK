SRC_DIR=./src
INC_DIR=./inc
LIB_DIR=./lib
BUILD_DIR=./build
BIN_DIR=./bin

CPP=g++
LIB_FLAGS=-lconfig++
CPP_FLAGS= -g -std=c++11 -pthread -lgrt -ltinfo -lstdc++ -lbluetooth#-Wall #-pedantic

EXTERN = ./externAssets
GRT_LIB = /grt/lib/ 
GRT_INC = /grt/include

DBUS_INC=-I/usr/include/dbus-1.0 -I/usr/lib64/dbus-1.0/include
LIB=-L/usr/lib64 -L$(EXTERN)$(GRT_LIB) 
INCLUDE=-I$(INC_DIR) -I$(EXTERN)$(GRT_INC)


main: $(BIN_DIR)/main

BINARIES=main

all:$(BINARIES)


$(BIN_DIR)/%: $(BUILD_DIR)/grConnection.o $(BUILD_DIR)/grAlgorithm.o $(BUILD_DIR)/grUtilities.o $(BUILD_DIR)/grGrt.o $(BUILD_DIR)/grMadgwick.o $(BUILD_DIR)/%.o 
		$(CPP) $(CPP_FLAGS) $(LIB) $(INCLUDE) $^ -o $@ $(LIB_FLAGS)


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
		$(CPP) $(CPP_FLAGS) $(LIB) $(INCLUDE) -c $^ -o $@ $(LIB_FLAGS)
#clean rules

#.PHONY: clean cleanobj cleanbin

clean:
		rm -vf $(BIN_DIR)/*
			rm -vf $(BUILD_DIR)/*

cleanobj:
		rm -vf $(BUILD_DIR)/*
			
cleanbin:
		rm -vf $(BIN_DIR)/*

