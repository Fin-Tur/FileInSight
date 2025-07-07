#include "UI/UI.h"

#include "finder/DuplicateFinder.h"
#include "core/FileCollecter.h"
void testCompressionRoundtrip(const std::filesystem::path& inputFile);

int main() {

    UI interface;
    interface.start_ui();

    std::system("pause");


    return 0;

}