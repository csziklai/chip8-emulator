#include "Chip8.hpp"
#include <string>
#include <vector>
#include <array>
#include <cstdint>  // for uint8_t, uint16_t
#include <cstddef>  // for std::byte
#include <iostream>
#include <fstream>
#include <cstdlib>

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
    uint8_t NN = static_cast<uint8_t>(opcode & 0x00FF);
    uint16_t NNN = opcode & 0x0FFF;
    switch(first) {
        case 0x0:
            if (N == 0) {
                clear_display();
            } else if (N == 0xE) {
                unsigned short top = stack.top();
                stack.pop();
                pc = top;
            } else {
                return;
            }
            break;

        case 0x1:
            pc = NNN;
            break;
        case 2:
            stack.push(pc-2);
            pc = NNN;
            break;
        case 3:
            if (V[X] == NN) {
                pc += 2;
            }
            break;
        case 4:
            if (V[X] != NN) {
                pc += 2;
            }
            break;
        case 5:
            if (V[X] == V[Y]) {
                pc += 2;
            }
            break;
        case 6:
            V[X] = NN;
            break;
        case 7:
            V[X] += NN;
            break;
        case 8:
            switch(N) { // least significant nibble
                case 0:
                    V[X] = V[Y];
                    break;
                case 1:
                    V[X] = V[X] | V[Y];
                    break;
                case 2:
                    V[X] = V[X] & V[Y];
                    break;
                case 3:
                    V[X] = V[X] ^ V[Y]; //XOR
                    break;
                case 4:
                    V[X] += V[Y];
                    if (V[X] > 255) {
                        V[0xF] = 1; //this is how you do it?
                    } else {
                        V[0xF] = 0;
                    }
                    break;
                case 5:
                    V[X] -= V[Y];
                    if (V[X] >= V[Y]) { //ambiguous on >= or >?
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }
                    break;
                case 6: {
                    unsigned char lsb = V[Y] & 1;
                    V[X] = V[Y] >> 1;
                    V[0xF] = lsb;
                    break;  
                }                  
                case 7:
                    V[X] = V[Y] - V[X];
                    if (V[Y] >= V[X]) { //ambiguous on >= or >?
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }
                    break;
                case 8: {
                    unsigned char msb = V[Y] >> 31; //does this work?
                    V[X] = V[Y] << 1;
                    V[0xF] = msb;
                    break; 
                }
            }
            break;
        case 9:
            if (V[X] != V[Y]) {
                pc += 2;
            }
            break;
        case 0xA:
            I = NNN;
            break;
        case 0xB:
            pc = NNN + V[0];
            break;
        case 0xC: {
            int randNum = rand() % (NN + 1); //do we want a diff random number every time the program runs?
            V[X] = randNum & NN;
            break;
        }
        case 0xD: {
            unsigned short x = V[X] & 63;
            unsigned short y = V[Y] & 31;
            unsigned short height = N;
            std::cout << "height: " << height << std::endl;
            unsigned short pixel;
            V[0xF] = 0;
            //std::cout << "Sprite row data: " << std::bitset<8>(pixel) << std::endl;

            for (int yline = 0; yline < height; yline++) {
                pixel = memory[I + yline];
                std::cout << "Row " << yline << ": " << std::bitset<8>(pixel) << std::endl;

                for (int xline = 0; xline < 8; xline++) {
                    unsigned short mask = 0x80 >> xline;
                    std::cout << (pixel & mask) << std::endl;
                    if ((pixel & mask) != 0) { // it is on
                        int idx = x + xline + ((y + yline) * 64);
                        std::cout << "Drawing at (" << x + xline << ", " << y + yline << ") Index: " 
          << (x + xline + ((y + yline) * 64)) << std::endl;
                        if (gfx[idx] == 1) {
                            V[0xF] = 1;
                            std::cout << "Collision detected at: " << (x + xline) << "," << (y + yline) << std::endl;
                        }
                        gfx[idx] ^= 1;

                    }
                }

            }
            drawFlag = true;
            break;
        }   
        case 0xE: {

            if (N == 0xE) {
                if (key[V[X]] != 0) {
                    pc += 2;

                }
            } else {
                // N == 0x1
                if (key[V[X]] == 0) {
                    pc += 2;
                }
            }
            break;
        }

        case 0xF: {
            switch(NN) {
                case 0x07:
                    V[X] = delay_timer;
                    break;
                case 0x15:
                    delay_timer = V[X];
                    break;
                case 0x18:
                    sound_timer = V[X];
                    break;
                case 0x1E:
                    I += V[X];
                    break;
                case 0x0A:
                    if (key[V[X]] == 0) {
                        pc -= 2;
                    }
                    break; // double check
                case 0x29:
                    I = V[X]; //LEFT OFF HERE
                    break;
                case 0x33: {
                    int x = static_cast<int>(V[X]);
                    memory[I] = x / 100;
                    memory[I + 1] = (x / 10) % 10;
                    memory[I + 2] = x % 10;
                    break;
                }
                case 0x55: {
                    unsigned short j = I;
                    for (uint8_t i = 0; i < X; i++) {
                        memory[j] = V[i];
                        j += 1;
                    }
                    break;
                }
                case 0x65: {
                    unsigned short k = I;
                    for (uint8_t i = 0; i < X; i++) {
                        V[i] = memory[k]; // not switch i and k as well?
                        k += 1;
                    }
                    break;
                }

            }
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