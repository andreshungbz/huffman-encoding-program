TEMPLATE = app
CONFIG += console c++17

SOURCES += main.cpp \
    driver/driver.cpp \
    src/huffman_tree/hash_map/FrequencyHashMap.cpp \
    src/huffman_tree/priority_queue/PriorityQueue.cpp \
    src/huffman_tree/HuffmanTree.cpp \
    src/utils/file/file_utils.cpp \
    src/utils/generate/generate_utils.cpp \
    src/utils/compression/compression_utils.cpp \
    src/utils/instantiate/instantiate_utils.cpp

HEADERS += driver/driver.h \
    src/huffman_tree/hash_map/FrequencyHashNode.h \
    src/huffman_tree/hash_map/FrequencyHashMap.h \
    src/huffman_tree/priority_queue/PriorityQueue.h \
    src/huffman_tree/HuffmanNode.h \
    src/huffman_tree/HuffmanTree.h \
    src/huffman_tree/components/FileInformation.h \
    src/huffman_tree/components/HuffmanHeader.h \
    src/utils/file/file_utils.h \
    src/utils/generate/generate_utils.h \
    src/utils/compression/compression_utils.h \
    src/utils/instantiate/instantiate_utils.h

INCLUDEPATH += src \
    driver

# Unix/Linux build folders
unix {
    CONFIG(debug, debug|release) {
        DESTDIR = $$PWD/qmake-build-debug
    }

    CONFIG(release, debug|release) {
        DESTDIR = $$PWD/qmake-build-release
    }

    OBJECTS_DIR = $$DESTDIR/
    MOC_DIR = $$DESTDIR/
    RCC_DIR = $$DESTDIR/
    UI_DIR = $$DESTDIR/
}

# Windows build folders
win32 {
    CONFIG(debug, debug|release) {
        DESTDIR = $$PWD\\qmake-build-debug
    }

    CONFIG(release, debug|release) {
        DESTDIR = $$PWD\\qmake-build-release
    }

    OBJECTS_DIR = $$DESTDIR\\
    MOC_DIR = $$DESTDIR\\
    RCC_DIR = $$DESTDIR\\
    UI_DIR = $$DESTDIR\\
}