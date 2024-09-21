// main.cpp
#include "Shell.h"

int main() {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;
    Shell shell;
    shell.run();
    return 0;
}
