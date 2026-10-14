#ifndef CHIP_H
#define CHIP_H

// heart of the Chip-8
#include <stdint.h>

typedef struct Chip8 {
    uint8_t registers[16];
    uint8_t memory[4096];
    uint16_t index;
    uint16_t pc;
    uint16_t stack[16];
    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t keypad[16];
    uint32_t video[64 * 32];
    uint16_t opcode;
} Chip8;

void chip8_init(struct Chip8 *c);

void chip8_loadrom(char const* filename, struct Chip8 *c);

void OP_00E0(struct Chip8 *c);

void OP_00EE(struct Chip8 *c);

void OP_1nnn(struct Chip8 *c);

void OP_2nnn(struct Chip8 *c);

#endif // CHIP_H