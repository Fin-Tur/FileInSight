//
// Created by Admin on 18.07.2025.
//

#ifndef RAWRECOVERYSCANNER_H
#define RAWRECOVERYSCANNER_H

#include "../../models/FileSignature.h"

const std::vector<FileSignature> knownSignatures = {
    // JPEG
    {{0xFF, 0xD8}, {0xFF, 0xD9}, "JPEG"},

    // PNG
    {{0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A}, {}, "PNG"},

    // GIF87a / GIF89a
    {{'G', 'I', 'F', '8', '7', 'a'}, {}, "GIF"},
    {{'G', 'I', 'F', '8', '9', 'a'}, {}, "GIF"},

    // PDF
    {{'%', 'P', 'D', 'F', '-'}, {'%', '%', 'E', 'O', 'F'}, "PDF"},

    // ZIP
    {{'P', 'K', 0x03, 0x04}, {}, "ZIP"},

    // RAR (v1.5–4.0)
    {{0x52, 0x61, 0x72, 0x21, 0x1A, 0x07, 0x00}, {}, "RAR"},

    // RAR (v5.0+)
    {{0x52, 0x61, 0x72, 0x21, 0x1A, 0x07, 0x01, 0x00}, {}, "RAR"},

    // 7-Zip
    {{0x37, 0x7A, 0xBC, 0xAF, 0x27, 0x1C}, {}, "7Z"},

    // EXE
    {{'M', 'Z'}, {}, "EXE"},

    // MP3 (ID3)
    {{'I', 'D', '3'}, {}, "MP3"},

    // MP4 / MOV
    {{0x00, 0x00, 0x00, 0x18, 'f', 't', 'y', 'p'}, {}, "MP4/MOV"},

    // BMP
    {{'B', 'M'}, {}, "BMP"}
};

class RawRecoveryScanner {
public:
    RawRecoveryScanner() = default;
    void scan(std::ifstream ifs);
    void printFiles() const;
private:
    std::vector<uint8_t> possibleFiles;
};



#endif //RAWRECOVERYSCANNER_H
