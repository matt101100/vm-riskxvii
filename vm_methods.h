#ifndef VM_METHODS_H
#define VM_METHODS_H

#include "vm_memory.h"

enum instruction_type {
    R = 0,
    I,
    U,
    S,
    SB,
    UJ
};

/*
 * Desc: initializes the virtual machine
 * Params: pointer to the virtual machine struct
 */
void initialize_virtual_machine(virtual_machine *vm);

/*
 * Desc: Extracts the opcode from the first 6 bits of a 32-bit instruction
 * Params: a 32-bit instruction binary
 * Return: an 8-bit opcode
 */
uint8_t get_opcode(uint32_t instruction);

/*
 * Desc: Extracts the func3 or func7 bits from a 32-bit instruction
 *       and stores in an array. Note that only R-type instructions have a 
 *       func7 and so the only time two values are read into opcode_arr is when
 *       we have a R-type instruction.
 * Params: a 32-bit instruction binary, an instruction type int enumerated above
 *         and a pointer to the opcode array to update
 */
void get_additional_opcode(uint32_t instruction, int instruction_type, 
                              uint8_t aditional_opcodes[]);

#endif