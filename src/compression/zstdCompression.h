#ifndef ZSTDCOMPRESSOR_H
#define ZSTDCOMPRESSOR_H

#include <filesystem>
#include "AbstractCompressor.h"
class zstdCompressor final : public AbstractCompressor{
public:

    bool compress(const std::filesystem::path& src, const std::filesystem::path& dst, const int compressionLevel) override;

    bool decompress(const std::filesystem::path& src, const std::filesystem::path& dst) override;
};

#endif // ZSTDCOMPRESSOR_H
