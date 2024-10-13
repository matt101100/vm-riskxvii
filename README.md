# vm-riskxvii
This repository contains source code for a simple virtual machine that can execute instructions defined by a reduced version of the RISC-V instruction set architecture and produce output based on the contents of the instructions within the binary. The VM is written in C and a Python file is provided to generate machine instruction (```.mi```) files that the VM can run. There is also a Makefile provided to build, test and run the VM.

## Instructions
Each instruction is similar in format to RISC-V instructions, with opcodes, operands and immediates specified in a 32-bit instruction. The lower 7 bits specify the opcode and the remaining bits specify operands, destination registers and immediates, depending on the type of instruction.

Test

## Opcodes

## Memory Structure
