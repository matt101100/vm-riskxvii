#ifndef VM_METHODS_H
#define VM_METHODS_H

#include "vm_memory.h"

/*
 * Desc: initializes the virtual machine
 * Params: pointer to the virtual machine struct
 */
void initialize_virtual_machine(virtual_machine *vm);

/*
 * Desc: Extracts the opcode from the first 6 bits of a 32-bit instruction
 * Params: a 32-bit instruction binary
 * Return: a 32-bit opcode
 */
uint8_t get_opcode(uint32_t instruction);

#endif