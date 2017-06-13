#requirements for cmake version
cmake_minimum_required (VERSION 3.5)

project(GR_SDK)

set(PROJECT grSDK)

set(EXTERN_ASSETS "externAssets")

set(BINARY_DIR "bin")
set(BUILD_DIR "build")
set(SOURCE_DIR "src")
set(HEADER_DIR "inc")

set(GRT_DIR "${EXTERN_ASSETS}/grt")
set(GRT_INCLUDE "${GRT_DIR}/include")
set(GRT_LIB "${GRT_DIR}/lib/libgrt.so")
#set(GRT_SOURCES "${EXTERN_ASSETS}/sources/grt_source")
set(EIGEN_DIR "${EXTERN_ASSETS}/eigen")
set(PLOG_DIR "${EXTERN_ASSETS}/plog")

set(SOURCES "${SOURCE_DIR}/*.cpp")
file(GLOB CPPS ${SOURCES})
#TODO add entries for samples maeby
set(CXX_COMPILE_FLAGS "-g -std=c++11 -pthread" )
set(CXX_LINK_FLAGS " -lncurses -lstdc++ -lbluetooth")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILE_FLAGS} ${CXX_LINK_FLAGS}")

include_directories(${HEADER_DIR})
include_directories(${GRT_INCLUDE})
include_directories(${EIGEN_DIR})

#cmake_policy(SET CMP0011 NEW)
#cmake_policy(SET CMP0001 OLD)
#option(BUILD_SHARED_LIB " " OFF)
#add_subdirectory("${GRT_SOURCES}/build" "../${GRT_DIR}")

OPTION(WITH_DEBUG_MODE "Build with debug mode" ON)

if(NOT UNIX)
    message (FATAL ERROR "Selected OS is not UNIX based")
endif()

add_executable(main ${SOURCE_DIR}/main.cpp ${CPPS})
target_link_libraries(main ${CMAKE_SOURCE_DIR}/externAssets/grt/lib/libgrt.so)
