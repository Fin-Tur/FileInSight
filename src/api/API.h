//
// Created by f.willems on 14.10.2025.
//
#pragma once

#if defined(_WIN32)
    #define FIS_API __declspec(dllexport)
#endif
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

    FIS_API int compress(const char* path, int compLvl);
    FIS_API int decompress(const char* path);
    FIS_API int encrypt(const char* path, const char* password, int iter);
    FIS_API int decrypt(const char* path, const char* password, int iter);
    //0->No Duplicates, 1->Dupes existing to file, 2->Dupes existing in general
    FIS_API int dupes_existing_for_file(const char* dir_path, const char* file_path);


#ifdef __cplusplus
}
#endif