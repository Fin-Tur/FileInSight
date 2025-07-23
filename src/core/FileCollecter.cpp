#include "FileCollecter.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <stack>
#include <chrono>

std::vector<FileInfo> FileCollector::collect(const std::string& path) {
    std::cout << "[Info] Collecting files...\n";

    std::vector<FileInfo> files;

    //Path is singular file
    std::error_code ec;
    std::filesystem::path inputPath(path);
    if (std::filesystem::is_regular_file(inputPath, ec)) {
        if (!isPathBlacklisted(path)) {
            uintmax_t size = std::filesystem::file_size(inputPath, ec);
            if (!ec) {
                FileInfo info{ inputPath.string(), size, "", -1 };

                std::error_code ec_time;
                auto ftime = std::filesystem::last_write_time(inputPath, ec_time);
                if (!ec_time) {
                    using namespace std::chrono;
                    auto now = std::filesystem::file_time_type::clock::now();
                    auto age_days = duration_cast<days>(now - ftime).count();
                    info.lastUsed = static_cast<int>(age_days);
                }

                files.emplace_back(info);
            }
        }

        return files; //return immediatly since singular file
    }


    std::stack<std::filesystem::path> dirs;
    dirs.emplace(path);

    while (!dirs.empty()) {
        std::filesystem::path current_dir = dirs.top();
        dirs.pop();

        std::error_code ec;
        std::filesystem::directory_iterator dir_it(current_dir, std::filesystem::directory_options::skip_permission_denied, ec);

        if (ec) {
            std::cerr << "[Dir error] " << current_dir << ": " << ec.message() << '\n';
            continue;
        }

        for (const auto& entry : dir_it) {
            std::error_code entry_ec;

            //Push Directorys on stack
            if (entry.is_directory(entry_ec)) {
                if (!entry_ec) dirs.push(entry.path());
                continue;
            }

            //Only work with regular files
            if (!entry.is_regular_file(entry_ec) || entry_ec)
                continue;

            const auto& pathCurr = entry.path();
            //Skip virtual files
            if (pathCurr.filename().string().starts_with("."))
                continue;

            uintmax_t size = std::filesystem::file_size(pathCurr, entry_ec);
            if (entry_ec)
                continue;

            if (!isPathBlacklisted(pathCurr.string())) {
                FileInfo info{pathCurr.string(), size, "", -1};

                std::error_code ec_time;
                auto ftime = std::filesystem::last_write_time(pathCurr, ec_time);
                if (!ec_time) {
                    using namespace std::chrono;
                    auto now = std::filesystem::file_time_type::clock::now();
                    auto age_days = duration_cast<days>(now - ftime).count();
                    info.lastUsed = static_cast<int>(age_days);
                }

                files.emplace_back(info);

                /*std::cout << "[OK] " << pathCurr
                          << " (" << size << " Bytes, ~"
                          << info.lastUsed << " days old)\n";
                          */

            }
        }
    }
    std::cout << "[Info] Collection succesful!\n";
    return files;
}

bool FileCollector::isPathBlacklisted(const std::string& pathStr) {
    std::filesystem::path path = std::filesystem::absolute(pathStr);
    path = path.lexically_normal();

    for (const auto& blStr : blacklist) {
        std::filesystem::path blPath = std::filesystem::absolute(blStr);
        blPath = blPath.lexically_normal();

        //Check if Path is blacklisted
        if (std::mismatch(blPath.begin(), blPath.end(), path.begin()).first == blPath.end()) {
            return true;
        }
    }
    return false;
}
