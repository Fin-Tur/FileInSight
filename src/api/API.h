
#pragma once

#if defined(_WIN32)
    #define FIS_API __declspec(dllexport)
#endif
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        size_t size;
        char type[64];
        double entropy;
        char tlsh_hash[256];
        int flagged;
        int last_used_days;
    } FIS_ExtFileInfo;

    FIS_API int fis_compress(const char* path, int compLvl);
    FIS_API int fis_decompress(const char* path);
    FIS_API int fis_encrypt(const char* path, const char* password, int iter);
    FIS_API int fis_decrypt(const char* path, const char* password, int iter);
    FIS_API int fis_dupes_existing_for_file(const char* dir_path, const char* file_path);
    FIS_API double fis_entropy_for_file(const char* path);
    FIS_API int fis_analyze_extended(const char* path, FIS_ExtFileInfo* file_info);
    FIS_API int fis_file_check_flag_bytes(const unsigned char* data, size_t data_size, const char* expected_type);
    


#ifdef __cplusplus
}
#endif