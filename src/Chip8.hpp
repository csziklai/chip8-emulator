#pragma once

#include <string>
#include <vector>

class Chip8
{
public:
    Chip8(); // initializes a new game by clearing memory, registers, screen

    bool drawFlag;

    unsigned short opcode; // current opcode
    unsigned short I; // index register
    unsigned short pc; // program counter

    unsigned char delay_timer;
    unsigned char sound_timer;

    unsigned short sp; //stack pointer

    unsigned char key[16]; // hex-based keypad

    void init();
    void loadGame(std::string name);
    void emulateCycle();
    void setKeys();
    void clear_display();

    private:
    std::array<unsigned char, 4096> memory{};  // Zero-initialized
    std::array<unsigned char, 16> V{}; // 16 registers
    std::array<unsigned char, 64 * 32> gfx{}; //graphics
    std::array<unsigned short, 16> stack{}; //stack to remember current location before jumping
    void fetch();


};