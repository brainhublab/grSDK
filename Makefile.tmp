SRC_DIR=./src
INC_DIR=./inc
LIB_DIR=./lib
BUILD_DIR=./build
BIN_DIR=./bin

CPP=g++
LIB_FLAGS=-ldbus-1 -lconfig++
CPP_FLAGS= -g -std=c++11 -pthread #-Wall #-pedantic

DBUS_INC=-I/usr/include/dbus-1.0 -I/usr/lib64/dbus-1.0/include
DBUS_LIB=-L/usr/lib64
INCLUDE=-I$(INC_DIR) $(DBUS_INC)


main: $(BIN_DIR)/main

BINARIES=main

all:$(BINARIES)



$(BIN_DIR)/%: $(BUILD_DIR)/gestusConnection.o $(BUILD_DIR)/%.o 
	$(CPP) $(CPP_FLAGS) $(INCLUDE) $^ -o $@ $(LIB_FLAGS)


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPP) $(CPP_FLAGS) $(INCLUDE) -c $^ -o $@ $(LIB_FLAGS)
#clean rules

#.PHONY: clean cleanobj cleanbin

clean:
	rm -vf $(BIN_DIR)/*
	rm -vf $(BUILD_DIR)/*

cleanobj:
	rm -vf $(BUILD_DIR)/*
	
cleanbin:
	rm -vf $(BIN_DIR)/*
