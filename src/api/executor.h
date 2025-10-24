//
// Created by f.willems on 14.10.2025.
//

#ifndef FILEINSIGHT_EXECUTOR_H
#define FILEINSIGHT_EXECUTOR_H
#include <string>
#include <vector>


namespace executor {
     int compress(std::string& path, int compLvl);
     int decompress(std::string& path);
     int encrypt(const std::string& path, std::string& password, int iter);
     int decrypt(const std::string& path, std::string& password, int iter);
     int dupes_existing_for_file(const std::string& path, const std::string& file);
     double entropy_for_file(const std::string& path);



};


#endif //FILEINSIGHT_EXECUTOR_H