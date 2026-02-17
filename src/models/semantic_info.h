#pragma once
#include <cstddef>
#include <filesystem>
#include <vector>
#include <cmath>

namespace sem{

    struct sem_info_chunk {
        char* chunk;
        size_t chunk_size;
        float embedding[384];
    };

    struct sem_info_file {
        char* content; //allocated in embedding_generator.cpp
        std::filesystem::path path;
        size_t size;
        std::vector<sem_info_chunk> chunks;

        ~sem_info_file() {
            delete[] content;
        }
    };

    float dot(const float* vec1, const float* vec2, size_t size){
        float result = 0.0f;
        for(size_t i = 0; i < size; ++i) {
            result += vec1[i] * vec2[i];
        }
        return result;
    }

    float len(const float* vec, size_t size){
        float len_sq = dot(vec, vec, size);
        return std::sqrt(len_sq);
    }

}