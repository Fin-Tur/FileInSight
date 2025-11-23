
#pragma once

#if defined(_WIN32)
    #define FIS_API __declspec(dllexport)
#endif
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

    FIS_API int fis_compress(const char* path, int compLvl);
    FIS_API int fis_decompress(const char* path);
    FIS_API int fis_encrypt(const char* path, const char* password, int iter);
    FIS_API int fis_decrypt(const char* path, const char* password, int iter);
    FIS_API int fis_dupes_existing_for_file(const char* dir_path, const char* file_path);
    FIS_API double fis_entropy_for_file(const char* path);


#ifdef __cplusplus
}
#endif