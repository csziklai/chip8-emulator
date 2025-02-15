#include "Chip8.hpp"


int main(int argc, char **argv) {

    // set up graphics
    // set up input

    Chip8 prog;
    prog.init();
    prog.loadGame(argv[1]);

    std::string line;

    while (std::getline(std::cin, line)) {
        prog.emulateCycle();

        prog.setKeys();

    }
    return 0;
}