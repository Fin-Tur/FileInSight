cmake_minimum_required(VERSION 3.31)
project(FileInSight)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

# Manuelles Einbinden von ZSTD
include_directories("C:/vcpkg/installed/x64-windows/include")
link_directories("C:/vcpkg/installed/x64-windows/lib")

add_executable(FileInSight
        src/main.cpp
        src/core/FileCollecter.cpp
        src/core/FileCollecter.h
        src/finder/DuplicateFinder.cpp
        src/finder/DuplicateFinder.h
        src/hashing/Hasher.cpp
        src/hashing/Hasher.h
        src/hashing/picosha2.h
        src/UI/UI.cpp
        src/UI/UI.h
        src/finder/RegExFinder.cpp
        src/finder/RegExFinder.h
        src/finder/AbstractFinder.h
        src/finder/AgingFileFinder.cpp
        src/finder/AgingFileFinder.h
        src/models/FileInfo.h
        src/compression/HuffmanCompressor.cpp
        src/compression/HuffmanCompressor.h
        src/compression/HuffmanTree.cpp
        src/compression/HuffmanTree.h
        src/tests/TestCompression.cpp
        src/compression/zstdCompressor.cpp
        src/compression/zstdCompressor.h
)

# ZSTD statisch linken
target_link_libraries(FileInSight PRIVATE zstd)
