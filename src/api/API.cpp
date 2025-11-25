
#include "API.h"

#include <cstring>

#include "executor.h"
#include "../analyzer/FileAnalyzer.h"
#include <analyzer/MagicByteAnalyzer.h>


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

// 0 -> Sucess, 1 -> Fail, 2 -> File not found
int fis_analyze_extended(const char* path, FIS_ExtFileInfo* file_info){
    if(!path || !file_info) return 1;
    std::string pathStr = std::string(path);
    try{
        auto ext_inf_opt = FileAnalyzer::analyzeExtended(pathStr);
        if (!ext_inf_opt.has_value()) return 2;
        auto ext_inf = ext_inf_opt.value();
        file_info->size = ext_inf.size;
        file_info->entropy = ext_inf.entropy;
        file_info->flagged = ext_inf.flagged ? 1 : 0;
        file_info->last_used_days = ext_inf.last_used_days;
        std::strncpy(file_info->tlsh_hash, ext_inf.tlsh_hash.c_str(), 255);
        std::strncpy(file_info->type, ext_inf.type.c_str(), 63);
        return 0;
    }catch(...){
        return 1;
    }
}

// -1 -> Invalid input, 0 -> Not flagged, 1 -> Flagged, 2 -> Error  
int fis_file_check_flag_bytes(const unsigned char* data, size_t data_size, const char* expected_type) {
    if (!data || data_size == 0 || !expected_type) return -1;
    try {
        return MagicByteAnalyzer::checkFlagBytes(data, data_size, expected_type);
    } catch (...) {
        return 2;
    }
}


