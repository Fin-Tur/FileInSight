#ifndef ZSTDCOMPRESSOR_H
#define ZSTDCOMPRESSOR_H

#include <filesystem>

class zstdCompressor {
public:
    // Compresses the file at 'src' and writes compressed output to 'dst'
    // Returns true if successful, false otherwise
    static bool compress(std::filesystem::path src, std::filesystem::path dst, int compressionLevel = 3);

    // Decompresses the file at 'src' and writes the original content to 'dst'
    // Returns true if successful, false otherwise
    static bool decompress(std::filesystem::path src, std::filesystem::path dst);
};

#endif // ZSTDCOMPRESSOR_H
