//
// Created by f.willems on 14.10.2025.
//

#include "API.h"

#include "executor.h"

//
int compress(const char* path, int compLvl) {
    if (!path || compLvl == 0) return 2;
    std::string strPath = path;
    try {
        return executor::compress(strPath, compLvl);
    }catch (...) {
        return 1;
    }

}

int decompress(const char* path) {
    if (!path) return 2;
    std::string strPath = path;
    try {
        return executor::decompress(strPath);
    }catch (...) {
        return 1;
    }

}

int encrypt(const char* path, const char* password, int iter) {
    if (!path || !password || iter == 0) return 2;
    std::string strPath = path;
    std::string strPassword = password;
    try {
        return executor::encrypt(strPath, strPassword, iter);
    }catch (...) {
        return 1;
    }
}

int decrypt(const char* path, const char* password, int iter) {
    if (!path || !password || iter == 0) return 2;
    std::string strPath = path;
    std::string strPassword = password;
    try {
        return executor::decrypt(strPath, strPassword, iter);
    }catch (...) {
        return 1;
    }
}

int dupes_existing_for_file(const char *dir_path, const char *file_path) {
    if (!dir_path || !file_path) return 3;
    std::string strDir_path = dir_path;
    std::string strFile_path = file_path;
    try {
        return executor::dupes_existing_for_file(strDir_path, strFile_path);
    }catch (...) {
        return 4;
    }
}

