cmake_minimum_required(VERSION 3.10)

# Project name
project(portalFactory)



# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/box2d/include)

# Add executable
add_executable(portalFactory game/main.cpp)

# Include all source files from the game directory
file(GLOB_RECURSE GAME_SOURCES ${CMAKE_SOURCE_DIR}/game/*.cpp)
target_sources(portalFactory PRIVATE ${GAME_SOURCES})

# Add OpenGL and GLUT libraries
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Link Box2D library
find_library(Box2D_LIB box2d PATHS ${CMAKE_SOURCE_DIR}/box2d/build/src NO_DEFAULT_PATH REQUIRED)
if (NOT Box2D_LIB)
    message(FATAL_ERROR "Box2D library not found. Ensure it is located in ${CMAKE_SOURCE_DIR}/box2d/build/src.")
endif()
target_link_libraries(portalFactory ${Box2D_LIB})

target_link_libraries(portalFactory OpenGL::GL GLUT::GLUT)
