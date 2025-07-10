cmake_minimum_required(VERSION 3.31)
project(FileInSight)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

# ZSTD einbinden
find_package(ZSTD REQUIRED)

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
)

# ZSTD an dein Executable binden
target_link_libraries(FileInSight PRIVATE ZSTD::ZSTD)
