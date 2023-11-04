cmake_minimum_required(VERSION 3.22)
project("ParametrPack")
file(GLOB SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
file(GLOB HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/MyMetaFunctions/*.hpp
)
set(CMAKE_CXX_STANDARD 20)
add_executable(${PROJECT_NAME} main.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

