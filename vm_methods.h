#ifndef VM_METHODS_H
#define VM_METHODS_H

#include "vm_memory.h"

/*
 * Desc: initializes the virtual machine
 * Params: pointer to the virtual machine struct
 */
void initialize_virtual_machine(virtual_machine *vm);

/*
 * Desc: runs the mathematical R-type instruction specified by the
         instruction_label. This includes the instructions: add, sub.
 * Params: the 32-bit instruction, an instruction label, a pointer to the 
           virtual machine struct
 */
void execute_math_type_R(uint32_t instruction, int instruction_label,
                         virtual_machine *vm);

/*
 * Desc: runs the logcial R-type instruction specified by instruction_label.
         This includes the instructions: xor, or, and
 *  Params: the 32-bit instruction, an instruction label, a pointer to the 
            virtual machine struct
 */
void execute_logical_type_R(uint32_t instruction, int instruction_label,
                            virtual_machine *vm);

/*
 * Desc: runs the logical instruction involving an immediate specified by
         instruction_label. This includes: xori, ori, andi, 
 *  Params: the 32-bit instruction, an instruction label, a pointer to the 
            virtual machine struct 
 */
void execute_logical_immediate(uint32_t instruction, int instruction_label,
                               virtual_machine *vm);

/*
 * Desc: runs the shifting instruction specified by instruction_label.
         This includes: sll, srl, sra
*  Params: the 32-bit instruction, an instruction label, a pointer to the 
           virtual machine struct
 */
void execute_shift(uint32_t instruction, int instruction_label,
                   virtual_machine *vm);

/*
 * Desc: runs the register setting instruction specified by instruction_label.
         This includes: slt, slti, sltu, sltiu
*  Params: the 32-bit instruction, an instruction label, a pointer to the 
           virtual machine struct
 */
void execute_register_set(uint32_t instruction, int instruction_label,
                          virtual_machine *vm);

/*
 * Desc: runs this branch instruction specified by instruction_label.
         This includes: beq, bne, blt, bltu, bge, bgeu
 * Params: the 32-bit instruction, an instruction label, a pointer to the 
           virtual machine struct
 */
void execute_branch(uint32_t instruction, int instruction_label,
                    virtual_machine *vm);

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
 * Desc: runs the load instruction specified by instruction_label. Also handles
         load-based virtual routines. This includes: lb, lh, lw, lbu, lhu
 * Params: the 32-bit instruction, an instruction label, a pointer to the
           virtual machine struct
 */
int execute_load(uint32_t instruction, int instruction_label,
                  virtual_machine *vm);

/*
 * Desc: runs the store instruction specified by instruction label. Also handles
         store-based virtual routines. This includes: sb, sh, sw
 * Params: the 32-bit instruction, an instruction label, a pointer to the
           virtual machine struct
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