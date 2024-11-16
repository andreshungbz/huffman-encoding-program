cmake_minimum_required(VERSION 3.29)
project(02_huffman_encoding)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(02_huffman_encoding main.cpp
        src/huffman_tree/hash_map/FrequencyHashNode.h
        src/huffman_tree/hash_map/FrequencyHashMap.h
        src/huffman_tree/hash_map/FrequencyHashMap.cpp

        src/huffman_tree/priority_queue/PriorityQueue.h
        src/huffman_tree/priority_queue/PriorityQueue.cpp

        src/huffman_tree/HuffmanNode.h
        src/huffman_tree/HuffmanTree.h
        src/huffman_tree/FileInformation.h
        src/huffman_tree/HuffmanFileHeader.h
        src/huffman_tree/HuffmanTree.cpp

        src/utils/FileUtils.h
        src/utils/FileUtils.cpp
        src/utils/GenerateUtils.h
        src/utils/GenerateUtils.cpp
        src/utils/CompressionUtils.h
        src/utils/CompressionUtils.cpp
)
