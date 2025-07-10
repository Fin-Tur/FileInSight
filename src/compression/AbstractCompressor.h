//
// Created by Admin on 10.07.2025.
//

#ifndef ABSTRACTCOMPRESSOR_H
#define ABSTRACTCOMPRESSOR_H
#include <filesystem>

class AbstractCompressor {
public:
    virtual ~AbstractCompressor() = default;

    // Compresses the file at 'src' and writes compressed output to 'dst'
    // Returns true if successful, false otherwise
    virtual bool compress(const std::filesystem::path& src, const std::filesystem::path& dst) = 0;

    // Decompresses the file at 'src' and writes the original content to 'dst'
    // Returns true if successful, false otherwise
    virtual bool decompress(const std::filesystem::path& src, const std::filesystem::path& dst) = 0;
};

#endif //ABSTRACTCOMPRESSOR_H
