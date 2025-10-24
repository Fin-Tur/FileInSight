//
// Created by f.willems on 14.10.2025.
//

#include "API.h"

#include <cstring>

#include "executor.h"

//
int fis_compress(const char* path, int compLvl) {
    if (!path || compLvl == 0) return 2;
    std::string strPath = path;
    try {
        return executor::compress(strPath, compLvl);
    }catch (...) {
        return 1;
    }

}

int fis_decompress(const char* path) {
    if (!path) return 2;
    std::string strPath = path;
    try {
        return executor::decompress(strPath);
    }catch (...) {
        return 1;
    }

}

int fis_encrypt(const char* path, const char* password, int iter) {
    if (!path || !password || iter == 0) return 2;
    std::string strPath = path;
    std::string strPassword = password;
    try {
        return executor::encrypt(strPath, strPassword, iter);
    }catch (...) {
        return 1;
    }
}

int fis_decrypt(const char* path, const char* password, int iter) {
    if (!path || !password || iter == 0) return 2;
    std::string strPath = path;
    std::string strPassword = password;
    try {
        return executor::decrypt(strPath, strPassword, iter);
    }catch (...) {
        return 1;
    }
}

int fis_dupes_existing_for_file(const char *dir_path, const char *file_path) {
    if (!dir_path || !file_path) return 3;
    std::string strDir_path = std::string(dir_path);
    std::string strFile_path = std::string(file_path);
    try {
        return executor::dupes_existing_for_file(strDir_path, strFile_path);
    }catch (...) {
        return 4;
    }
}

double fis_entropy_for_file(const char *path) {
    if (!path || strlen(path) == 0) return -1;
    std::string strPath = std::string(path);
    try {
        return executor::entropy_for_file(strPath);
    }catch (...) {
        return -2;
    }
}


