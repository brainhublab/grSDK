BUILD_DIR = ./bin
INC_DIR = ./inc
SRC_DIR = ./src
UI_DIR = ./inc/ui

IMGUI_FILES = $(UI_DIR)/imgui/imgui.cpp $(UI_DIR)/imgui/imgui_draw.cpp $(SRC_DIR)/ui/imgui_impl_sdl.cpp

OBJS = $(IMGUI_FILES) $(SRC_DIR)/Arm.cpp $(SRC_DIR)/VGui.cpp $(SRC_DIR)/Renderer.cpp $(SRC_DIR)/gestusVisualization.cpp $(SRC_DIR)/main.cpp

CC = g++

COMPILER_FLAGS = -w -std=c++11

LINKER_FLAGS = -lSDL2 -lGL -lGLU

OBJ_NAME = $(BUILD_DIR)/visualize

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME) -I$(INC_DIR)

clean :
	rm -v $(OBJ_NAME)
# done: ok move from visualization/ to inc all my written files;
# done: voids to bool
# learn stuff about makefiles
