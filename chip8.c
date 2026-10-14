// initialize opcodes and chip8 function

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <time.h>
#include <string.h>
#include "chip8.h"

// constants
const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_SIZE = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50;

uint8_t fontset[FONTSET_SIZE] =
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

// temp opcode define so vscode isnt mad at me 
int opcode = 0;

void chip8_init(Chip8 *c) {
    // set start to ROM when initializing
    c->pc = START_ADDRESS; 

    // load fonts into memory
    for (unsigned int i = 0; i < FONTSET_SIZE; i++) {
        c->memory[FONTSET_START_ADDRESS] = fontset[i];
    }

    // unsigned to not return time type t
    srand((unsigned)time(NULL));
    // initialize the random bytes
    uint8_t randbyte = rand() % 256;
}

void chip8_loadrom(char const* filename, Chip8 *c) {
    // read file as binary
    FILE *file = fopen(filename, "rb");

    if (!file) {
        printf("Error with opening file.\n");
    }

    // set ptr to eof
    fseek(file, 0, SEEK_END);

    // get file size and put in mem
    fpos_t size = ftell(file);
    char *buffer = malloc(size);

    // read from beginning and put in buffer
    fseek(file, 0, SEEK_SET);
    size_t fread(buffer, size, file);
    fclose(file);

    // load ROM into mem starting at 0x200
    for (long i = 0; i < size; i++) {
        c->memory[START_ADDRESS + i] = buffer[i];
    }

    free(buffer);
}

// CLS opcode, clears display
void OP_00E0(Chip8 *c) {
    memset(c->video, 0, sizeof(c->video));
}

// RET opcode, return from subroutine
void OP_00EE(Chip8 *c) {
    --c->sp;
    c->pc = c->stack[c->sp];
}

// JP opcode, jump PC to nnn
void OP_1nnn(Chip8 *c) {
    uint16_t address = opcode & 0x0FFFu;

    c->pc = address;
}

// CALL opcode, call subroutine at nnn
void OP_2nnn(Chip8 *c) {
    uint16_t address = opcode & 0x0FFFu;

    c->stack[c->sp] = c->pc;
    c->sp++;
    c->pc = address;
}

// call everything here
int main(int argc, char *argv[]) {
    Chip8 c;

    if (argc != 4) {
        printf(">> Usage: ./chip8.c [Scale] [Delay] [ROM]");
    }

    int videoscale = argv[1];
    int cycledelay = argv[2];
    char const* romfile = argv[3];

    chip8_loadrom(romfile, &c);

    return 0;
}