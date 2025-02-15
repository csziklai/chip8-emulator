#include "Chip8.hpp"

#include <string>
#include <vector>

Chip8::Chip8() : drawFlag(false), memory([]) {}

Chip8::emulateCycle() {
    // fetch opcode
    opcode = memory[pc] << 8 | memory[pc+1];
    pc += 2;
    byte first = opcode & 0xF000;
    byte X = opcode & 0x0F00; // second nibble
    byte Y = opcode & 0x00F0; // 3rd nibble
    byte N = opcode & 0x000F; // 4th nibble
    byte NN = opcode & 0xFF00;
    byte NNN = opcide & 0x0FFF;
    switch(first) {
        case 0:
            if (N == 0) {
                clear_display();
            } else if (N == E) {
                return;
            }
            break;

        case 1:
            pc = NNN;
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            V[X] = NN;
        case 7:
            V[X] += NN;
        case 8:
        case 9:
        case A:
            I = NNN;
        case B:
        case C:
        case D:
            
        case E:
        case F:

    }
    
    // decode opcode
    // execute opcode

    // update timers
}

Chip8::fetch() {

}

Chip8::clear_display() {

}