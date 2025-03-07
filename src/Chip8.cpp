#include "Chip8.hpp"
#include <string>
#include <vector>
#include <array>
#include <cstdint>  // for uint8_t, uint16_t
#include <cstddef>  // for std::byte
#include <iostream>
#include <fstream>

unsigned char chip8_fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8(){};

void Chip8::init() {
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;

    drawFlag = true;
    std::array<unsigned char, 64 * 32> gfx = {}; // Ensures all elements are 0

    // Load fontset
	for(int i = 0; i < 80; ++i) {
        memory[i] = chip8_fontset[i];
    }
    for (int i = 0; i < 80; i++) {
        std::cout << "Font[" << i << "] = " << std::hex << (int)memory[i] << std::endl;
    }

    
}

void Chip8::loadGame(std::string filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size > 3584) {  // Ensure it fits within available memory (4096 - 0x200)
        std::cerr << "ROM too large to fit in memory" << std::endl;
        return;
    }

    file.read(reinterpret_cast<char*>(&memory[0x200]), size);

    for (int i = 0x200; i < 0x210; i++) {
        std::cout << "Memory[" << i << "] = " << std::hex << (int)memory[i] << std::endl;
    }
    
    file.close();



}

void Chip8::emulateCycle() {
    // fetch opcode
    //std::cout << "Emulating cycle..." << std::endl;
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
            break;
        case 7:
            V[X] += NN;
            break;
        case 8:
        case 9:
        case 0xA:
            I = NNN;
            break;
        case 0xB:
        case 0xC:
        case 0xD: {
            std::cout << "Executing draw opcode!" << std::endl;

            unsigned short x = V[X]; // & 63?
            unsigned short y = V[Y]; // & 31
            unsigned short height = NN;
            unsigned short pixel;
            V[0xF] = 0;
            //std::cout << "Sprite row data: " << std::bitset<8>(pixel) << std::endl;

            for (int yline = 0; yline < height; yline++) {
                pixel = memory[I + yline];
                for (int xline = 0; xline < 8; xline++) {
                    unsigned short mask = 0x80 >> xline;
                    if ((pixel & mask) == 1) { // it is on
                        int idx = ((x + xline)%64) + (((y + yline)%32)*64);
                        std::cout << "Drawing at (" << x + xline << ", " << y + yline << ") Index: " 
          << (x + xline + ((y + yline) * 64)) << std::endl;
                        if (gfx[idx] == 1) {
                            V[0xF] = 1;
                            std::cout << "Collision detected at: " << (x + xline) << "," << (y + yline) << std::endl;
                        }
                        gfx[idx] = 1;

                    }
                }

            }
            drawFlag = true;
            break;
        }   
        case 0xE: {
            break;
        }

        case 0xF: {
            break;
        }

    }
    
    // decode opcode
    // execute opcode

    // update timers
}


void Chip8::clear_display() {
    gfx.fill(0);


}