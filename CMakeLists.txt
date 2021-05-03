cmake_minimum_required(VERSION 3.17)
project(rasterizer)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        ${PROJECT_SOURCE_DIR}/src
)

file(GLOB SOURCES
        src/*.hpp
        src/*.h
        src/*.inl
        src/*.cpp
        *.cpp
        )

add_executable(rasterizer main.cpp ${SOURCES} src/commonDefine.h)