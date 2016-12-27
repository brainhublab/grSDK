cmake_minimum_required(VERSION 3.5)
project(gestusSDK)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    inc/ui/imgui/extra_fonts/binary_to_compressed_c.cpp
    inc/ui/imgui/imconfig.h
    inc/ui/imgui/imgui.cpp
    inc/ui/imgui/imgui.h
    inc/ui/imgui/imgui_demo.cpp
    inc/ui/imgui/imgui_draw.cpp
    inc/ui/imgui/imgui_internal.h
    inc/ui/imgui/stb_rect_pack.h
    inc/ui/imgui/stb_textedit.h
    inc/ui/imgui/stb_truetype.h
    inc/ui/imgui_impl_sdl.h
    inc/visualization/ui/imgui_impl_sdl.h
    inc/Arm.h
    inc/Dimensions.h
    inc/gestusAlgorithm.h
    inc/gestusConnection.h
    inc/gestusHand.h
    inc/gestusUtilites.h
    inc/gestusVisualization.h
    inc/Node.h
    inc/openGL.h
    inc/Renderer.h
    inc/VGui.h
    src/ui/imgui_impl_sdl.cpp
    src/Arm.cpp
    src/gestusAlgorithm.cpp
    src/gestusConnection.cpp
    src/gestusHand.cpp
    src/gestusUtilites.cpp
    src/gestusVisualization.cpp
    src/main.cpp
    src/Renderer.cpp
    src/VGui.cpp
    src/visualmain.cpp
    imgui.ini
    Makefile)

include_directories(./inc)

add_executable(gestusSDK ${SOURCE_FILES})