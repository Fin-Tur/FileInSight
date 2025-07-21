//
// Created by Admin on 03.07.2025.
//


#ifndef UI_H
#define UI_H

#include <string>

class UI {

public:
    static std::string convertBytes(std::size_t bytes);
    static std::intmax_t bytesToKB(std::intmax_t bytes){return bytes/1024;}
    static std::intmax_t bytesToMB(std::intmax_t bytes){return (bytes/1024)/1024;}
};



#endif //UI_H
