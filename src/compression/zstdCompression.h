#pragma once

#include <filesystem>
#include "AbstractCompressor.h"
class zstdCompressor final : public AbstractCompressor{
public:

    bool compress(const std::filesystem::path& src, const std::filesystem::path& dst, int compressionLevel) override;

    bool decompress(const std::filesystem::path& src, const std::filesystem::path& dst) override;
};

