cmake_minimum_required(VERSION 3.29)
project(02_huffman_encoding)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/driver)

add_executable(02_huffman_encoding
        # Main
        main.cpp
        driver/driver.h
        driver/driver.cpp

        # Frequency Hash Map
        src/huffman_tree/hash_map/FrequencyHashNode.h
        src/huffman_tree/hash_map/FrequencyHashMap.h
        src/huffman_tree/hash_map/FrequencyHashMap.cpp

        # Priority Queue
        src/huffman_tree/priority_queue/PriorityQueue.h
        src/huffman_tree/priority_queue/PriorityQueue.cpp

        # Huffman Tree and components
        src/huffman_tree/HuffmanNode.h
        src/huffman_tree/HuffmanTree.h
        src/huffman_tree/components/FileInformation.h
        src/huffman_tree/components/HuffmanHeader.h
        src/huffman_tree/HuffmanTree.cpp

        # Utilities
        src/utils/file/file_utils.h
        src/utils/file/file_utils.cpp
        src/utils/generate/generate_utils.h
        src/utils/generate/generate_utils.cpp
        src/utils/compression/compression_utils.h
        src/utils/compression/compression_utils.cpp
        src/utils/instantiate/instantiate_utils.h
        src/utils/instantiate/instantiate_utils.cpp
)
