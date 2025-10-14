//
// Created by f.willems on 14.10.2025.
//

#ifndef FILEINSIGHT_EXECUTOR_H
#define FILEINSIGHT_EXECUTOR_H
#include <string>
#include <vector>


class executor {
    public:

    static int compress(std::string& path, int compLvl);
    static int decompress(std::string& path);

    static int encrypt(const std::string& path, std::string& password, int iter);
    static int decrypt(const std::string& path, std::string& password, int iter);

    static int dupes_existing_for_file(const std::string& path, const std::string& file);



};


#endif //FILEINSIGHT_EXECUTOR_H