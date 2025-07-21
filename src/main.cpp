#include <algorithm>

#include "cli/CLIParser.h"
#include "compression/zstdCompression.h"
#include "UI/UI.h"

#include "finder/DuplicateFinder.h"
#include "core/FileCollecter.h"

void testRecovery();
void testCompressionRoundtrip(const std::filesystem::path& inputFile);


int main(int argc, char* argv[]) {

        return CLIParser::run(argc, argv);


    return 0;

}