#requirements for cmake version
cmake_minimum_required (VERSION 3.5)

project(GR_SDK)

set(PROJECT GR_SDK)

set(EXTERN_ASSETS /externAssets)

set ("${PROJECT}_BINARY_DIR" bin)
set("${PROJECT}_BUILD_DIR" build)
set("${PROJECT}_SOURCE_DIR" src:inc)


cmake_policy(SET CMP0011 NEW)
cmake_policy(SET CMP0001 OLD)

OPTION(WITH_DEBUG_MODE "Build with debug mode" ON)

if(NOT UNIX)
    message (FATAL ERROR "Selected OS is not UNIX based")
endif()

add_executable(main main.cpp)

