cmake_minimum_required(VERSION 3.10)
get_filename_component(PROJECT_NAME ${CMAKE_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
    src/Ball-Game.cpp
    src/BFS.cpp
    src/Condition.cpp
    src/Structures.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})