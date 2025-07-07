//
// Created by Admin on 07.07.2025.
//
#include "../compression/HuffmanCompressor.h"
#include <filesystem>
#include <fstream>
#include <iostream>

void testCompressionRoundtrip(const std::filesystem::path& inputFile) {
    HuffmanCompressor compressor;

    std::filesystem::path compressedFile = "C:/Users/Admin/OneDrive/Desktop/temp_compressed.huff";
    std::filesystem::path decompressedFile = "C:/Users/Admin/OneDrive/Desktop/temp_decompressed.txt";

    compressor.compress(inputFile, compressedFile);
    compressor.decompress(compressedFile, decompressedFile);

    // Vergleich der Dateien
    std::ifstream original(inputFile, std::ios::binary);
    std::ifstream decompressed(decompressedFile, std::ios::binary);

    if (!original || !decompressed) {
        std::cerr << "Fehler beim Öffnen der Dateien für den Vergleich." << std::endl;
        return;
    }

    char ch1, ch2;
    int position = 0;
    while (original.get(ch1) && decompressed.get(ch2)) {
        if (ch1 != ch2) {
            std::cerr << "Fehler: Unterschied an Byte " << position << ": "
                      << (int)(unsigned char)ch1 << " != " << (int)(unsigned char)ch2 << std::endl;
            return;
        }
        position++;
    }

    if (original.get(ch1) || decompressed.get(ch2)) {
        std::cerr << "Fehler: Dateigrößen stimmen nicht überein." << std::endl;
        return;
    }

    std::cout << "✅ Test erfolgreich: Komprimierte und dekomprimierte Datei sind identisch!" << std::endl;
}
