#ifndef DUPLICATEFINDER_H
#define DUPLICATEFINDER_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

#include "AbstractFinder.h"
#include "../core/FileCollecter.h"

// List of protected folders (won't delete from these)
const std::vector<std::string> protectedPaths = {
    "C:/Windows",
    "C:/Program Files",
    "C:/Program Files (x86)",
    "C:/System32",
    "C:/boot",
    "C:/EFI"
};

// List of protected file extensions
const std::vector<std::string> protectedFiles = {
    ".dll",
    ".sys",
    ".exe"
};

struct FileInfo; // Forward declaration

// Finds and manages duplicate files
class DuplicateFinder : public AbstractFinder{
    std::unordered_map<std::uintmax_t, std::vector<FileInfo>> fileSizeMap; // files grouped by size
    std::unordered_map<std::string, std::vector<FileInfo>> duplicates;     // files grouped by hash
    std::pmr::unordered_set<std::filesystem::path> deletedFiles;           // deleted file log

public:
    explicit DuplicateFinder(const std::vector<FileInfo>& files);

    // Finds duplicate files
    std::vector<std::vector<FileInfo>> find() override;

    // Byte-by-byte file comparison
    static bool areFilesEqual(const FileInfo& f1, const FileInfo& f2);

    // Deletes duplicates (except one per group)
    void deleteDuplicates(std::vector<FileInfo>& duplicates);

    // Checks if file is protected (by path or extension)
    static bool isProtected(const std::filesystem::path& pathStr);

    // Computes hashes in parallel
    void computeHashesParallel(std::vector<FileInfo>& files);
};

#endif // DUPLICATEFINDER_H