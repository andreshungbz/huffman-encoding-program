cmake_minimum_required(VERSION 3.29)
project(02_huffman_encoding)

set(CMAKE_CXX_STANDARD 20)

include_directories(${PROJECT_SOURCE_DIR}/header ${PROJECT_SOURCE_DIR}/src)

add_executable(02_huffman_encoding main.cpp
    header/FrequencyHashNode.h
    header/FrequencyHashMap.h
    src/FrequencyHashMap.cpp
)
