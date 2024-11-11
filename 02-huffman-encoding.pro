TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle

SOURCES += main.cpp \
    src/hash_map/FrequencyHashMap.cpp \
    src/priority_queue/PriorityQueue.cpp \
    src/huffman_tree/HuffmanTree.cpp

HEADERS += header/hash_map/FrequencyHashNode.h \
    header/hash_map/FrequencyHashMap.h \
    header/priority_queue/PriorityQueue.h \
    header/huffman_tree/HuffmanNode.h \
    header/huffman_tree/HuffmanTree.h \
    header/FileInformation.h

INCLUDEPATH += header \
    src