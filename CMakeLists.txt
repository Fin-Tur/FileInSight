cmake_minimum_required(VERSION 3.31)
project(FileInSight)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

# === Manually include zstd ===
include_directories("C:/vcpkg/installed/x64-windows/include")
link_directories("C:/vcpkg/installed/x64-windows/lib")
add_definitions(-DZSTD_STATIC_LINKING_ONLY)

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
        src/compression/zstdCompression.cpp
        src/compression/zstdCompression.h
        src/tests/TestCompression.cpp
        src/compression/AbstractCompressor.h
)

# === Link against the static zstd library ===
target_link_libraries(FileInSight PRIVATE zstd)
