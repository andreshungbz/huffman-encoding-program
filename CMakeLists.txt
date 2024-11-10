cmake_minimum_required(VERSION 3.29)
project(02_huffman_encoding)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/header ${PROJECT_SOURCE_DIR}/src)

add_executable(02_huffman_encoding main.cpp
        header/hash_map/FrequencyHashNode.h
        header/hash_map/FrequencyHashMap.h
        src/hash_map/FrequencyHashMap.cpp
        header/priority_queue/PriorityQueue.h
        header/huffman_tree/HuffmanNode.h
        src/priority_queue/PriorityQueue.cpp
        header/huffman_tree/HuffmanTree.h
        src/huffman_tree/HuffmanTree.cpp
        header/FileInformation.h
)
