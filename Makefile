BUILD_DIR = ./bin
INC_DIR = ./inc
SRC_DIR = ./src
UI_DIR = ./inc/visualization/ui

IMGUI_FILES = $(UI_DIR)/imgui/imgui.cpp $(UI_DIR)/imgui/imgui_draw.cpp $(SRC_DIR)/visualization/ui/imgui_impl_sdl.cpp

OBJS = $(IMGUI_FILES) $(SRC_DIR)/visualization/Arm.cpp $(SRC_DIR)/visualization/Renderer.cpp $(SRC_DIR)/gestusVisualization.cpp $(SRC_DIR)/main.cpp

CC = g++

COMPILER_FLAGS = -w -std=c++11

LINKER_FLAGS = -lSDL2 -lGL -lGLU

OBJ_NAME = $(BUILD_DIR)/visualization

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME) -I$(INC_DIR)

clean :
	rm -v $(OBJ_NAME)
