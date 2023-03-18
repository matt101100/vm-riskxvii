#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#include "vm_memory.h"

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
 * Desc: determines the specific instruction based on the opcodes provided
 * Params: the 8-bit opcode and addtional func3 and func7 opcodes
 * Return: int mapped to the specific instruction label enumerated above
 */
int determine_instruction_label(uint8_t opcode, uint8_t addtional_opcodes[]);



#endif