# vm-riskxvii
This repository contains source code for a simple virtual machine that can execute instructions defined by a reduced version of the RISC-V instruction set architecture and produce output based on the contents of the instructions within the binary. The VM is written in C and a Python file is provided to generate machine instruction (```.mi```) files that the VM can run. There is also a Makefile provided to build, test and run the VM.

## Instructions
Each instruction is similar in format to RISC-V instructions, with opcodes, operands and immediates specified in a 32-bit instruction. The lower 7 bits specify the opcode and the remaining bits specify operands, destination registers and immediates, depending on the type of instruction.

RISKXVII is a subset of RISC-V. As such, it supports specific instruction types that allow for simple computation, including mathematical operations, logical operations, bitwise shifting operations, load from and store to memory operations and branch and jump operations to allow for control flow. There are also versions of some of these instructions that specify an immediate within the 32-bits. The structure of each 32-bit instruction type ('''R, I, S, U''') can be found in *The RISC-V Instruction Set Manual Volume I* on page 40 of the PDF found at https://riscv.org/technical/specifications/.

## Opcodes

## Memory Structure
