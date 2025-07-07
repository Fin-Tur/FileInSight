//
// Created by Admin on 03.07.2025.
//

#include "UI.h"

#include <algorithm>

#include "../finder/AgingFileFinder.h"
#include "../hashing/picosha2.h"
#include "../finder/RegExFinder.h"

bool UI::verifyPassword() {
    std::string input;
    std::cin >> input;
    return picosha2::hash256_hex_string(input) == passwordHashed;
}


void UI::start_ui() {

    std::cout << "Please enter password to continue:\n";
    if (!verifyPassword()) return;

    //main-loop

    while (true) {
        this->freedMemory = 0;
        this->duplicateDataAmount = 0;
        std::system("cls");

        std::cout << "\nWhich Program would you like to run? \n[D : DuplicateSearch]\n[R : RegEx Search]\n[A: Aged Search]\n[E : Exit]\n";
        std::string input;
        std::cin >> input;
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);
        if (input == "d") {
            start_duplicateSearch();
        }else if (input == "r") {
            start_regexSearch();
        }else if (input == "a") {
            start_agingSearch();
        }else if (input == "e") {
            break;
        }
    }



}

void UI::start_duplicateSearch() {

    std::cout << "Please enter path for duplicate search: \n";
    std::string filePath;
    std::cin >> filePath;


    try {
        std::vector<FileInfo> files = FileCollector::collect(filePath);
        DuplicateFinder finder(files);
        std::vector<std::vector<FileInfo>> duplicates = finder.find();

        std::system("cls");

        if (duplicates.empty()){std::cout <<"No duplicates found!\n";}

        for (auto& filesD : duplicates) {
            std::cout << "\nFile Duplicate found: \n";
            for (auto& file: filesD) {
                std::cout << file.path << " size: " << file.size/1024.0 << "KB" << std::endl;
            }
            this->duplicateDataAmount += filesD[0].size*(filesD.size()-1);
            std::cout << "\nWould you like to delete following file/s? [Y : Yes], [N: NO] \n";

            for (int i = 1; i < filesD.size(); i++) {
                std::cout << filesD[i].path << " size: " << filesD[i].size/1024.0 << "KB" << std::endl;
            }
            std::cout << "\n";
            std::string deleteAnswer;
            std::cin >> deleteAnswer;
            std::transform(deleteAnswer.begin(), deleteAnswer.end(), deleteAnswer.begin(), ::tolower);

            if (deleteAnswer == "y") {
                this->freedMemory += filesD[0].size*(filesD.size()-1);
                finder.deleteDuplicates(filesD);
            }

        }

        std::cout << "Total Duplicate Data: " << this->duplicateDataAmount/1024.0 << "kB" << std::endl;
        std::cout << "Freed Memory: " << this->freedMemory/1024.0 << "kB" << std::endl;


    }catch (std::exception& e) {
        std::cout << "\nError :" << e.what() << std::endl;
    }

}

void UI::start_regexSearch() {
    std::cout << "Please enter path for Regex-Title search: \n";
    std::string filePath;
    std::cin >> filePath;

    std::cout << "Please enter pattern to look out for:\n";
    std::string pattern;
    std::cin >> pattern;

    try {
        std::vector<FileInfo> files = FileCollector::collect(filePath);
        RegExFinder finder(files, pattern);


        std::vector<FileInfo> results = finder.find()[0];

        if (results.empty()) {std::cout <<"\nNo matches found!";}

        for (auto& file: results) {
            std::cout << "Match! : " << file.path.string() << "\n";
        }

    } catch (std::exception& e) {
        std::cout << "\nError :" << e.what() << std::endl;
    }

}

void UI::start_agingSearch() {
    std::cout << "Please enter path for AgingFilter search: \n";
    std::string filePath;
    std::cin >> filePath;

    std::cout <<"Please enter aging criteria (last used) in days: \n";
    int lastUsed;
    std::cin >> lastUsed;

    try {
        std::vector<FileInfo> files = FileCollector::collect(filePath);
        AgingFileFinder finder(files, lastUsed);
        std::vector<FileInfo> results = finder.find()[0];
        if (results.empty()) {std::cout <<"\nNo matches found!";}

        for (auto& file: results) {
            std::cout << "Match! : " << file.path.string() << " Last Used: " << file.lastUsed << " days ago!\n";
        }

    } catch (std::exception& e) {
        std::cout << "\nError :" << e.what() << std::endl;
    }
}

