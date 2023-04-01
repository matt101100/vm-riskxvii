#ifndef VM_MEMORY_H
#define VM_MEMORY_H

#include <stdint.h>

#define MEMORY_SIZE 2048 // 2^11 bytes of addressable memory
#define BLOCK_SIZE 64 // size of a heap block
#define NUM_REGISTERS 32
#define HEAP_SIZE (1 << 13) // 8192 bytes of allocatable memory

/*
 * A block of memory to be desginated for use once the instructions ask for
   memory. Has a linked list structure, with a pointer to he next block in the
   sequence.
 */
typedef struct block block;
struct block {
    uint32_t usable_mem_size; // the exact size of requested memory
    uint32_t total_mem_size; // the total size of memory, multiple of 64
    uint32_t mem_base_address; // the base address for this block
    block *next; // pointer to the head of the next block
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
    uint32_t pc; // program counter
    uint8_t heap[HEAP_SIZE];
    block *head;
    uint32_t total_allocated_memory; // amount of heap bytes allocated
};

/*
 * Desc: Checks if a write to mem_address is to a currently allocated heap block
 * Params: the memory address to write to / store from, a pointer to the vm
           struct and an 8-bit integer representing the amount of data to write
           / store.
  Return: 0: if memory access to mem_address is invalid, 1 if access is valid
 */
uint8_t check_heap_memory_access(uint32_t mem_address,
                                       virtual_machine *vm, uint8_t data_size);


#endif