#ifndef VM_METHODS_H
#define VM_METHODS_H

#include "vm_memory.h"

/*
 * Desc: initializes the virtual machine
 * Params: pointer to the virtual machine struct
 */
void initialize_virtual_machine(virtual_machine *vm);

// !! executions for each machine instruction label are declared below !!

/*
 * Executes for each machine instruction are declared below.
 * Desc: see Section 4 of the specs.
 * Params: the 32-bit instructions, a pointer to the virtual machine
 */

void execute_add(uint32_t instruction, virtual_machine *vm);

void execute_addi(uint32_t instruction, virtual_machine *vm);

void execute_sub(uint32_t instruction, virtual_machine *vm);

void execute_lui(uint32_t instruction, virtual_machine *vm);

void execute_xor(uint32_t instruction, virtual_machine *vm);

void execute_xori(uint32_t instruction, virtual_machine *vm);

void execute_or(uint32_t instruction, virtual_machine *vm);

void execute_ori(uint32_t instruction, virtual_machine *vm);

void execute_and(uint32_t instruction, virtual_machine *vm);

void execute_andi(uint32_t instruction, virtual_machine *vm);

void execute_sll(uint32_t instruction, virtual_machine *vm);

void execute_srl(uint32_t instruction, virtual_machine *vm);

void execute_sra(uint32_t instruction, virtual_machine *vm);

/*
 * Includes all load ops: lb, lh, lw, lbu, lhu 
 */
int execute_load(uint32_t instruction, int instruction_label,
                  virtual_machine *vm);

/*
 * Includes all store ops: sb, sh, sw
 */
int execute_store(uint32_t instruction, int instruction_label,
                  virtual_machine *vm);

void execute_slt(uint32_t instruction, virtual_machine *vm);

void execute_slti(uint32_t instruction, virtual_machine *vm);

void execute_sltu(uint32_t instruction, virtual_machine *vm);

void execute_sltiu(uint32_t instruction, virtual_machine *vm);

void execute_beq(uint32_t instruction, virtual_machine *vm);

void execute_bne(uint32_t instruction, virtual_machine *vm);

void execute_blt(uint32_t instruction, virtual_machine *vm);

void execute_bltu(uint32_t instruction, virtual_machine *vm);

void execute_bge(uint32_t instruction, virtual_machine *vm);

void execute_bgeu(uint32_t instruction, virtual_machine *vm);

void execute_jal(uint32_t instruction, virtual_machine *vm);

void execute_jalr(uint32_t instruction, virtual_machine *vm);

#endif