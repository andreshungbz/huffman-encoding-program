TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle

SOURCES += main.cpp \
    src/hash_map/FrequencyHashMap.cpp \
    src/priority_queue/PriorityQueue.cpp \
    src/huffman_tree/HuffmanTree.cpp \
    src/FileUtils.cpp

HEADERS += header/hash_map/FrequencyHashNode.h \
    header/hash_map/FrequencyHashMap.h \
    header/priority_queue/PriorityQueue.h \
    header/huffman_tree/HuffmanNode.h \
    header/huffman_tree/HuffmanTree.h \
    header/FileInformation.h \
    header/FileUtils.h

INCLUDEPATH += header \
    src

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