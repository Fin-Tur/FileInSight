#include <algorithm>

#include "cli/CLIParser.h"
#include "compression/zstdCompression.h"
#include "UI/UI.h"

#include "finder/DuplicateFinder.h"
#include "core/FileCollecter.h"

void testRecovery();
void testCompressionRoundtrip(const std::filesystem::path& inputFile);

void printHelper() {
    std::cout << "Wrong usage of commands! Usage: \nFileInSight -compress <path>\nFileInSight -decompress <path>";
}

int main(int argc, char* argv[]) {

    /*if (argc < 2) {
        UI interface;
        interface.start_ui();

        std::system("pause");
        return 0;

    }else{

        return CLIParser::run(argc, argv);

    }*/

    testRecovery();

    return 0;

}