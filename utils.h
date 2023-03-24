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
size_t load_image_into_memory(FILE *fp, uint8_t memory[]);

/*
 * Desc: shifts and masks adjacent memory addresses to get the 32-bit 
 * instruction
 * Params: a pointer to the virtual machine, to access memory
 * Returns: the 32-bit instruction
 */
uint32_t get_instruction(virtual_machine *vm);

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
 * Desc: gets the target register, specified in the instruction bits
 * Params: the 32-bit instruction
 * Return: the target register stored using 8 bits
 */
uint8_t get_target_register(uint32_t instruction);

/*
 * Desc: gets the source registers and stores them in a 2-tuple
 * Params: the 32-bit instructions, an instruction type flag and the
 *         2-tuple to update
 */
void get_source_registers(uint32_t instruction, int instruction_type,
                               uint8_t source_registers[]);

/*
 * Desc: determines the specific instruction based on the opcodes provided
 * Params: the 8-bit opcode and addtional func3 and func7 opcodes
 * Return: int mapped to the specific instruction label enumerated above
 */
int determine_instruction_label(uint8_t opcode, uint32_t instruction);

/*
 * Desc: extracts the immediate bits depending on the type of instruction given
 * Params: the 32-bit instruction and an int representing the instruction type
 * Return: the sign extended 32-bit immediate number number 
 */
uint32_t extract_immediate_number(uint32_t instruction, int instruction_type);

/*
 * Desc: sign extends a 32-bit unsigned number, used on immediate numbers
 * Params: a 32-bit unsigned number, the number of bits that were extracted
 * Return: a sign extended 32-bit number
 */
uint32_t sign_extend(int32_t num, int original_bit_count);

/*
 * Desc: prints all registers to stdout
 * Params: a pointer to the virtual machine, to access the register bank
 */
void register_dump(virtual_machine *vm);

#endif