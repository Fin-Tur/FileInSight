cmake_minimum_required(VERSION 3.16)
project(FileInSight)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

include(FetchContent)

# === ZSTD via FetchContent (as static library) ===
FetchContent_Declare(
        zstd
        GIT_REPOSITORY https://github.com/facebook/zstd.git
        GIT_TAG v1.5.5
)
FetchContent_MakeAvailable(zstd)

# === Haupt-Binary ===
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
        src/compression/AbstractCompressor.h
        src/hashing/FuzzyHasher.cpp
        src/hashing/FuzzyHasher.h
        src/finder/SimilarityFinder.cpp
        src/finder/SimilarityFinder.h
        src/analyzer/MagicByteAnalyzer.cpp
        src/analyzer/MagicByteAnalyzer.h
        src/cli/CLIParser.cpp
        src/cli/CLIParser.h
        src/steganography/XOREncryptor.cpp
        src/steganography/XOREncryptor.h
        src/analyzer/PathAnalyzer.cpp
        src/analyzer/PathAnalyzer.h
        src/analyzer/EntropyAnalyzer.cpp
        src/analyzer/EntropyAnalyzer.h
        src/analyzer/FileAnalyzer.cpp
        src/analyzer/FileAnalyzer.h
        src/steganography/forensics/RawRecoveryScanner.cpp
        src/steganography/forensics/RawRecoveryScanner.h
        src/models/FileSignature.h
        src/models/RecoveredFile.h
)

# === Test-Binary ===
add_executable(test_runner
        src/tests/test_main.cpp
        src/tests/TestEncryption.cpp
        src/tests/TestRecovery.cpp
        src/tests/TestCompression.cpp
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
        src/compression/AbstractCompressor.h
        src/hashing/FuzzyHasher.cpp
        src/hashing/FuzzyHasher.h
        src/finder/SimilarityFinder.cpp
        src/finder/SimilarityFinder.h
        src/analyzer/MagicByteAnalyzer.cpp
        src/analyzer/MagicByteAnalyzer.h
        src/cli/CLIParser.cpp
        src/cli/CLIParser.h
        src/steganography/XOREncryptor.cpp
        src/steganography/XOREncryptor.h
        src/analyzer/PathAnalyzer.cpp
        src/analyzer/PathAnalyzer.h
        src/analyzer/EntropyAnalyzer.cpp
        src/analyzer/EntropyAnalyzer.h
        src/analyzer/FileAnalyzer.cpp
        src/analyzer/FileAnalyzer.h
        src/steganography/forensics/RawRecoveryScanner.cpp
        src/steganography/forensics/RawRecoveryScanner.h
        src/models/FileSignature.h
        src/models/RecoveredFile.h
)

# === TLSH (lokal) ===
add_library(tlsh STATIC
        src/thirdparty/fuzzyhashLib/tlsh_impl.cpp
        src/thirdparty/fuzzyhashLib/tlsh_util.cpp
        src/thirdparty/fuzzyhashLib/tlsh.cpp
)

target_include_directories(tlsh PUBLIC src/thirdparty/fuzzyhashLib)

# === Include-Pfade ===
target_include_directories(FileInSight PRIVATE src ${zstd_SOURCE_DIR}/lib)
target_include_directories(test_runner PRIVATE src ${zstd_SOURCE_DIR}/lib)

# === Link Libraries ===
target_link_libraries(FileInSight PRIVATE libzstd_static tlsh)
target_link_libraries(test_runner PRIVATE libzstd_static tlsh)
