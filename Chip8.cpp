#include "Chip8.hpp"

#include <string>
#include <vector>
#include <cstdint>  // for uint8_t, uint16_t
#include <cstddef>  // for std::byte

Chip8::Chip8(){};

void Chip8::emulateCycle() {
    // fetch opcode
    opcode = memory[pc] << 8 | memory[pc+1];
    pc += 2;
    uint8_t first = static_cast<uint8_t>((opcode & 0xF000)>> 12);
    uint8_t X = static_cast<uint8_t>((opcode & 0x0F00)>> 8); // second nibble
    uint8_t Y = static_cast<uint8_t>((opcode & 0x00F0)>> 4); // 3rd nibble
    uint8_t N = static_cast<uint8_t>(opcode & 0x000F); // 4th nibble
    uint8_t NN = static_cast<uint8_t>(opcode & 0xFF00);
    uint16_t NNN = opcode & 0x0FFF;
    switch(first) {
        case 0x0:
            if (N == 0) {
                clear_display();
            } else {
                return;
            }
            break;

        case 0x1:
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
        case 0xA:
            I = NNN;
        case 0xB:
        case 0xC:
        case 0xD:
            int x = V[X]; // x-coord
            int y = V[Y]; // y-coord
            
        case 0xE:
        case 0xF:

    }
    
    // decode opcode
    // execute opcode

    // update timers
}

void Chip8::fetch() {

}

void Chip8::clear_display() {

}