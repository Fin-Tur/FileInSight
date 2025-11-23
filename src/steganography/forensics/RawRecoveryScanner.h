#pragma once

#include "../../models/FileSignature.h"
#include "../../models/RecoveredFile.h"

inline std::vector<FileSignature> knownSignatures = {
    // JPEG
    {{0xFF, 0xD8}, {0xFF, 0xD9}, "jpeg"},

    // PNG
    {{0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A}, {}, "png"},

    // GIF87a / GIF89a
    {{'G', 'I', 'F', '8', '7', 'a'}, {}, "gif"},
    {{'G', 'I', 'F', '8', '9', 'a'}, {}, "gif"},

    // PDF
    {{'%', 'P', 'D', 'F', '-'}, {'%', '%', 'E', 'O', 'F'}, "pdf"},

    // ZIP
    {{'P', 'K', 0x03, 0x04}, {}, "zip"},

    // RAR (v1.5–4.0)
    {{0x52, 0x61, 0x72, 0x21, 0x1A, 0x07, 0x00}, {}, "rar"},

    // RAR (v5.0+)
    {{0x52, 0x61, 0x72, 0x21, 0x1A, 0x07, 0x01, 0x00}, {}, "rar"},

    // 7-Zip
    {{0x37, 0x7A, 0xBC, 0xAF, 0x27, 0x1C}, {}, "7z"},

    // EXE
    {{'M', 'Z'}, {}, "exe"},

    // MP3 (ID3)
    {{'I', 'D', '3'}, {}, "mp3"},

    // MP4 / MOV
    {{0x00, 0x00, 0x00, 0x18, 'f', 't', 'y', 'p'}, {}, "mp4"},

    // BMP
    {{'B', 'M'}, {}, "bmp"}
};



class RawRecoveryScanner {
public:
    explicit RawRecoveryScanner(const bool& enable_UTF16LE);
    void scan(std::ifstream& ifs);
    void extractFiles(std::ifstream& ifs, const std::string& outputDir) const;
    static void addUTF16LESignatures();
private:
    const int maxWindowLength = 16;
    std::vector<RecoveredFile> extractedFiles;
};

