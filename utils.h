#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#include "vm_memory.h"

enum instruction_type {
    R = 0,
    I,
    U,
    S,
    SB,
    UJ
};

enum instruction_label {
    add = 0,
    addi,
    sub,
    lui,
    xor,
    xori,
    or,
    ori,
    and,
    andi,
    sll,
    srl,
    sra,
    lb,
    lh,
    lw,
    lbu,
    lhu,
    sb,
    sh,
    sw,
    slt,
    slti,
    sltu,
    sltiu,
    beq,
    bne,
    blt,
    bltu,
    bge,
    bgeu,
    jal,
    jalr
};

/*
 * Desc: Opens the file containing machine instructions with binary read flags
 * Params: path to the file, pointer to the virtual machine struct
 * Return: pointer to the beginning of the file
 */
FILE *open_machine_instructions(char filename[], virtual_machine *vm);

/*
 *Desc: Reads the instructions / data into corresponding memory
 * Params: pointer to an open file, pointer to the virtual machine
 * Return: the number of bytes read
 */
size_t load_image_into_memory(FILE *fp, uint32_t memory[]);

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

/*
 * Desc: determines the specific instruction based on the opcodes provided
 * Params: the 8-bit opcode and addtional func3 and func7 opcodes
 * Return: int mapped to the specific instruction label enumerated above
 */
int determine_instruction_label(uint8_t opcode, uint8_t addtional_opcodes[]);

/*
 * Desc: extracts the immediate bits depending on the type of instruction given
 * Params: the 32-bit instruction and an int representing the instruction type
 * Return: the sign extended 32-bit immediate number number 
 */
uint32_t extract_immediate_number(uint32_t instruction, int instruction_type);

#endif