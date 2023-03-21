#ifndef VM_MEMORY_H
#define VM_MEMORY_H

#include <stdint.h>

#define MEMORY_SIZE 1024 // number of 4-byte instruction lines in 1024 bytes
#define BLOCK_SIZE 64 // size of a heap block
#define NUM_REGISTERS 32 // 32 registers

/*
 * 32 registers in total
 * all general-purpose for now, NEED SOME FOR FLAGS SO CHANGE THE NAME AROUND
 * AND LABEL THEM AS PER THEIR USE
 */
enum registers {
    ZERO = 0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15,
    R16,
    R17,
    R18,
    R19,
    R20,
    R21,
    R22,
    R23,
    R24,
    R25,
    R26,
    R27,
    R28,
    R29,
    R30,
    R31,
    R32,
    REGISTER_COUNT
};


/*
 * A block of memory to be desginated for use once the instructions ask for
 * memory. Has a linked list structure, with a pointer to he next block in the
 * sequence.
 */
typedef struct block block;
struct block {
    uint8_t block_memory[BLOCK_SIZE]; // 64 byte block for usage as memory
    int in_use; // 1 = block in use, 0 = free
    block *next; // pointer to the next block
};

/*
 * Represents the virtual machine
 * contains arrays that store the instruction and data memory as well as the
 * program counter and the head of the heap.
 */
typedef struct virtual_machine virtual_machine;
struct virtual_machine {
    uint32_t instruction_memory[MEMORY_SIZE];
    uint32_t data_memory[MEMORY_SIZE];
    uint32_t registers[REGISTER_COUNT];
    int pc; // program counter
    block *head;
    
};

#endif