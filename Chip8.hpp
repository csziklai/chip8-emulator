#pragma once

#include <string>
#include <vector>

class Chip8
{
public:
    Chip8(); // initializes a new game by clearing memory, registers, screen

    bool drawFlag;
    unsigned char memory[4096];
    unsigned short opcode; // current opcode
    unsigned char V[16]; // 16 registers

    unsigned short I; // index register
    unsigned short pc; // program counter
    unsigned char gfx[64 * 32]; // graphics

    unsigned char delay_timer;
    unsigned char sound_timer;

    unsigned short stack[16]; //stack to remember current location before jumping
    unsigned short sp; //stack pointer

    unsigned char key[16]; // hex-based keypad

    void init();
    void loadGame(string name);
    void emulateCycle();
    void setKeys();
    void clear_display();


}