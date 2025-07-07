#include "UI/UI.h"

#include "finder/DuplicateFinder.h"
#include "core/FileCollecter.h"

int main() {

    UI interface;
    interface.start_ui();

    std::system("pause");

    return 0;

}