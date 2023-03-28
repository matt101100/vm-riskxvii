#ifndef VM_MEMORY_H
#define VM_MEMORY_H

#include <stdint.h>

#define MEMORY_SIZE 2048 // 2^11 bytes of addressable memory
#define BLOCK_SIZE 64 // size of a heap block
#define NUM_REGISTERS 32

/*
 * A block of memory to be desginated for use once the instructions ask for
   memory. Has a linked list structure, with a pointer to he next block in the
   sequence.
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
   program counter and the head of the heap.
 */
typedef struct virtual_machine virtual_machine;
struct virtual_machine {
    uint8_t memory[MEMORY_SIZE];
    uint32_t registers[NUM_REGISTERS];
    int pc; // program counter
    block *head;
    
};

#endif