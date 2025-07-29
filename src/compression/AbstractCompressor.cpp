//
// Created by Admin on 26.07.2025.
//

#include "AbstractCompressor.h"

#include <deque>
#include <iostream>
#include <thread>

#include "core/FileCollecter.h"

bool AbstractCompressor::handleCompress(const std::filesystem::path &src, int compressionLevel) {
    //gather files
    auto files = FileCollector::collect(src.string());
    if (files.empty()) {
        std::cerr << "[Error] Could not extract files!\n";
        return false;
    }
    //Configure thread / work
    int maxThreads = std::max(1u, std::thread::hardware_concurrency());
    std::deque<std::thread> threads;
    auto compressionWorker = [&](const FileInfo& fi) {
        const std::filesystem::path dstPath = fi.path.string() +".fisc";
        if (this->compress(src, dstPath, compressionLevel)) {
            std::filesystem::remove(fi.path);
        }
    };

    //start threads
    for (auto& file : files) {
        threads.emplace_back(compressionWorker, file);
        if (threads.size() >= maxThreads) {
            if (threads[0].joinable()) {
                threads[0].join();
                threads.pop_front();
            }
        }
    }

    //wait for thread finish
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    return true;

}

bool AbstractCompressor::handleDecompress(const std::filesystem::path &src) {
    //Collect files
    auto files = FileCollector::collect(src.string());
    if (files.empty()) {
        std::cerr << "[Error] Could not extract files!\n";
        return false;
    }
    //Prepare threads
    int maxThreads = std::max(1u, std::thread::hardware_concurrency());
    std::deque<std::thread> threads;
    auto decompressionWorker = [&](const FileInfo& fi) {
        std::string srcStr = fi.path.string();
        if (srcStr.size() < 5 or srcStr.substr(srcStr.size() -5) != ".fisc") {
            std::cerr << "[Error] Unexpected file fomat for decompression : " << srcStr << "\n";
            return false;
        }else {
            const std::filesystem::path dstPath = srcStr.substr(0, srcStr.size() -5);
            if (this->decompress(srcStr, dstPath)) {
                std::filesystem::remove(srcStr);
            }else {
                std::cerr << "[Error] Could not decompress files!\n";
                return false;
            }
        }
        return true;
    };

    //Start threads
    for (const auto& file : files) {
        threads.emplace_back(decompressionWorker, file);
        if (threads.size() >= maxThreads) {
            if (threads[0].joinable()) {
                threads[0].join();
                threads.pop_front();
            }
        }
    }

    //Wait for threads
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    return true;
}
