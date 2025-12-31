cmake_minimum_required(VERSION 3.16)
project(FileInSight)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#===Sources===
set(FIS_SOURCES
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
        src/models/ExtFileInfo.h
        src/config/Settings.cpp
        src/config/Settings.h
        src/steganography/AESEncryptor.cpp
        src/steganography/AESEncryptor.h
        src/thirdparty/aesEnc/aes.c
        src/thirdparty/aesEnc/aes.h
        src/thirdparty/aesEnc/aes.hpp
        src/steganography/AbstractEncryptor.h
        src/steganography/AbstractEncryptor.cpp
        src/compression/AbstractCompressor.cpp
        src/container/VaultManager.cpp
        src/container/VaultManager.h
        src/cli/CLIParser.cpp
)

# === Haupt-Binary ===
add_executable(FileInSight
        src/main.cpp
        ${FIS_SOURCES}
)

# === DLL - Target ===
add_library(FISDLL SHARED
        src/UI/UI.cpp
        src/UI/UI.h
        src/core/FileCollecter.cpp
        src/core/FileCollecter.h
        src/finder/DuplicateFinder.cpp
        src/finder/DuplicateFinder.h
        src/hashing/Hasher.cpp
        src/hashing/Hasher.h
        src/hashing/FuzzyHasher.cpp
        src/hashing/FuzzyHasher.h
        src/hashing/picosha2.h
        src/finder/AbstractFinder.h
        src/models/FileInfo.h
        src/compression/zstdCompression.cpp
        src/compression/zstdCompression.h
        src/compression/AbstractCompressor.h
        src/steganography/AESEncryptor.cpp
        src/steganography/AESEncryptor.h
        src/thirdparty/aesEnc/aes.c
        src/thirdparty/aesEnc/aes.h
        src/thirdparty/aesEnc/aes.hpp
        src/steganography/AbstractEncryptor.h
        src/steganography/AbstractEncryptor.cpp
        src/compression/AbstractCompressor.cpp
        src/analyzer/EntropyAnalyzer.cpp
        src/analyzer/EntropyAnalyzer.h
        src/analyzer/FileAnalyzer.cpp
        src/analyzer/FileAnalyzer.h
        src/analyzer/MagicByteAnalyzer.cpp
        src/analyzer/MagicByteAnalyzer.h
        src/hashing/Hasher.cpp
        src/hashing/Hasher.h

        src/api/executor.cpp
        src/api/executor.h
        src/api/API.cpp
        src/api/API.h
)
target_compile_definitions(FISDLL PRIVATE FIS_BUILD_DLL)
#smaller dll (9->2MB) not reached sections thrown away
target_link_options(FISDLL PRIVATE -Wl,--gc-sections -s)

# === Test-Binary ===
add_executable(test_runner
        src/thirdparty/testing/catch_amalgamated.cpp
        src/tests/TestEncryption.cpp
        src/tests/TestRecovery.cpp
        src/tests/TestCompression.cpp
        ${FIS_SOURCES}
)

# === TLSH (local) ===
add_library(tlsh STATIC
        src/thirdparty/fuzzyhashLib/tlsh_impl.cpp
        src/thirdparty/fuzzyhashLib/tlsh_util.cpp
        src/thirdparty/fuzzyhashLib/tlsh.cpp
)

target_include_directories(tlsh PUBLIC src/thirdparty/fuzzyhashLib)
set_target_properties(tlsh PROPERTIES POSITION_INDEPENDENT_CODE ON)


# === ZSTD aus Source bauen ===
file(GLOB ZSTD_COMMON
        ${CMAKE_SOURCE_DIR}/src/thirdparty/zstd/zstd-dev/lib/common/*.c
)
file(GLOB ZSTD_COMPRESS
        ${CMAKE_SOURCE_DIR}/src/thirdparty/zstd/zstd-dev/lib/compress/*.c
)
file(GLOB ZSTD_DECOMPRESS
        ${CMAKE_SOURCE_DIR}/src/thirdparty/zstd/zstd-dev/lib/decompress/*.c
)
file(GLOB ZSTD_DICT
        ${CMAKE_SOURCE_DIR}/src/thirdparty/zstd/zstd-dev/lib/dictBuilder/*.c
)

add_library(zstd_static STATIC
        ${ZSTD_COMMON}
        ${ZSTD_COMPRESS}
        ${ZSTD_DECOMPRESS}
        ${ZSTD_DICT}
)

target_compile_definitions(zstd_static PUBLIC ZSTD_DISABLE_ASM)
set_target_properties(zstd_static PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(zstd_static PUBLIC
        ${CMAKE_SOURCE_DIR}/src/thirdparty/zstd/zstd-dev/lib
        ${CMAKE_SOURCE_DIR}/src/thirdparty/zstd/zstd-dev/lib/common
        ${CMAKE_SOURCE_DIR}/src/thirdparty/zstd/zstd-dev/lib/dictBuilder
)



target_include_directories(FileInSight PRIVATE src src/thirdparty/zstd/zstd-dev/lib)
target_include_directories(test_runner PRIVATE src src/thirdparty/zstd/zstd-dev/lib)
target_include_directories(FISDLL PRIVATE src src/thirdparty/zstd/zstd-dev/lib)


# === Link Libraries ===
target_link_libraries(FileInSight PRIVATE zstd_static tlsh)
target_link_libraries(test_runner PRIVATE zstd_static tlsh)
target_link_libraries(FISDLL PRIVATE zstd_static tlsh)



