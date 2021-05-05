cmake_minimum_required(VERSION 3.17)
project(rasterizer)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/common
        ${PROJECT_SOURCE_DIR}/src/resterizer
        ${PROJECT_SOURCE_DIR}/src/math
)

file(GLOB SOURCES
        src/*.hpp
        src/*.h
        src/*.inl
        src/*.cpp
        src/common/*.hpp
        src/common/*.h
        src/common/*.cpp
        src/common/*.inl
        src/resterizer/*.hpp
        src/resterizer/*.h
        src/resterizer/*.cpp
        src/resterizer/*.inl
        src/math/*.hpp
        src/math/*.h
        src/math/*.cpp
        *.cpp
        )

add_executable(rasterizer main.cpp ${SOURCES})