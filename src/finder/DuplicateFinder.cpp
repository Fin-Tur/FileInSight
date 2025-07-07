#include "DuplicateFinder.h"
#include <iostream>
#include <ranges>
#include <unordered_set>
#include "../hashing/Hasher.h"

// Constructor: group files by size
DuplicateFinder::DuplicateFinder(const std::vector<FileInfo> &files) {
    for (auto& file : files) {
        this->fileSizeMap[file.size].emplace_back(file);
    }
}

// Find duplicates by size, then hash, then byte-by-byte
std::vector<std::vector<FileInfo>> DuplicateFinder::find() {
    std::vector<std::vector<FileInfo>> result;

    // Step 1: hash files with same size
    for (auto &group : this->fileSizeMap | std::views::values) {
        if (group.size() < 2) continue;

        computeHashesParallel(group);
    }

    // Step 2: confirm duplicates by byte comparison
    std::vector<FileInfo> filesToAdd;
    for (const auto& files : duplicates | std::views::values) {
        if (files.size() < 2) continue;
        filesToAdd.emplace_back(files[0]);
        for (int i = 0; i < files.size() - 1; ++i) {
            if (areFilesEqual(files[i], files[i + 1])) {
                filesToAdd.emplace_back(files[i + 1]);
            }
        }
        if (filesToAdd.size() > 1) {
            result.emplace_back(filesToAdd);
        }
        filesToAdd.clear();
    }
    return result;
}

// Compare two files byte-by-byte
bool DuplicateFinder::areFilesEqual(const FileInfo &f1, const FileInfo &f2) {
    std::ifstream ifs1(f1.path, std::ios::binary);
    if (!ifs1) throw std::runtime_error("Cannot open file!");
    std::ifstream ifs2(f2.path, std::ios::binary);
    if (!ifs2) throw std::runtime_error("Cannot open file!");

    char byte1, byte2;
    while (ifs1.get(byte1) && ifs2.get(byte2)) {
        if (byte1 != byte2) return false;
    }
    return true;
}

// Delete duplicates (except one) unless protected
void DuplicateFinder::deleteDuplicates(std::vector<FileInfo> &duplicates) {
        for (int i = 1; i < duplicates.size(); ++i) {
            try {
                if (!isProtected(duplicates[i].path)) {
                    std::filesystem::remove(duplicates[i].path);
                    this->deletedFiles.insert(duplicates[i].path);
                    std::cout << "Removed file @ " << duplicates[i].path << std::endl;
                } else {
                    std::cout << "[Error] File cannot be removed (protected)\n";
                }
            } catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "[Error] " << e.what() << '\n';
            }
        }

}

// Check if file is protected by path or extension
bool DuplicateFinder::isProtected(const std::filesystem::path& pathStr){
    std::filesystem::path path = std::filesystem::weakly_canonical(std::filesystem::absolute(pathStr));
    path = path.lexically_normal();

    for (const auto& protectedRootStr : protectedPaths) {
        std::filesystem::path protectedPath = std::filesystem::weakly_canonical(std::filesystem::absolute(protectedRootStr));
        protectedPath = protectedPath.lexically_normal();

        if (std::mismatch(protectedPath.begin(), protectedPath.end(), path.begin()).first == protectedPath.end()) {
            return true;
        }
    }

    auto ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    for (const auto& protectedExt : protectedFiles) {
        if (ext == protectedExt) {
            return true;
        }
    }
    return false;

}


// Compute hashes in parallel using std::async
void DuplicateFinder::computeHashesParallel(std::vector<FileInfo> &files) {
    std::vector<std::future<std::pair<std::string, FileInfo>>> futures;

    for (auto& file : files) {
        futures.emplace_back(std::async(std::launch::async, [&file]() {
            std::string hash = Hasher::computeHash(file, false);
            FileInfo copy = file;
            copy.hash = hash;
            return std::make_pair(hash, copy);
        }));
    }

    int success = 0, failed = 0;

    for (auto& f : futures) {
        try {
            auto [hash, file] = f.get();
            if (!hash.empty()) {
                this->duplicates[hash].emplace_back(file);
                ++success;
            } else {
                ++failed;
            }
        } catch (const std::exception& e) {
            std::cerr << "[Hash error] " << e.what() << '\n';
            ++failed;
        }
    }

    std::cout << "[Hashing Summary] Success: " << success << ", Failed: " << failed << "\n";
}
