//
// Created by Admin on 21.07.2025.
//
#include "UI.h"


std::string UI::convertBytes(std::size_t bytes) {
    if (bytes < 1024) {
        return std::to_string(bytes)+" bytes";
    }else if (bytes < 1024*1024) {
        return std::to_string(bytes/1024)+"KB";
    }else if (bytes < 1024*1024*1024) {
        return std::to_string(bytes/(1024*1024))+"MB";
    }
}


