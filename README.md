# vm-riskxvii
This repository contains source code for a simple virtual machine that can execute instructions defined by a reduced version of the RISC-V instruction set architecture and produce output based on the contents of the instructions within the binary. The VM is written in C and a Python file is provided to generate machine instruction (```.mi```) files that the VM can run. There is also a Makefile provided to build, test and run the VM.

## Instructions
Each instruction is similar in format to RISC-V instructions, with opcodes, operands and immediates specified in a 32-bit instruction. The lower 7 bits specify the opcode and the remaining bits specify operands, destination registers and immediates, depending on the type of instruction.

RISKXVII is a subset of RISC-V. As such, it supports specific instruction types that allow for simple computation, including mathematical operations, logical operations, bitwise shifting operations, load from and store to memory operations and branch and jump operations to allow for control flow. There are also versions of some of these instructions that specify an immediate within the 32-bits. The structure of each 32-bit instruction type (```R, I, S, U```) can be found in *The RISC-V Instruction Set Manual Volume I* on page 40 of the PDF found at https://riscv.org/technical/specifications/.

## Opcodes
RISKXVII uses a set of 25 opcodes, with most of them coming from the RISC-V ISA. Note that some of the opcodes are unique to RISKXVII so they may not be all present in the Manual linked above.

## Memory Structure
The VM is implemented with 4096 bytes of memory. It is arranged as follows:
* 1KB is allocated as instruction memory (between addresses ```0x0000 - 0x03FF```) where instructions to be executed are loaded and stored.
* 1KB is allocated as data memory (between addresses ```0x0400 - 0x07FF```) where data can be written to or read from.
* 2KB is allocated for memory-mapped I/O (between addresses ```0x0800 - 0x0FFF```).

## Building and the Makefile
The included Makefile can be used to build, test and run the VM and can also be used to clean the workspace of any binaries produced during compliation. Note that the following commands must be run from the root directory.

### Building
You can build the VM by running 
```
$ make 
```

### Running
You can run a program on the VM by supplying a ```.mi``` file location as a command line argument. So to run an example in ```./examples```, you can use
```
$ ./vm_riskxvii ./examples/hello_world/hello_world.mi
```

### Testing
Tests are found in the ```./tests``` directory and can be run all at once using the Makefile by running
```
$ make tests
```

### Cleaning
Finally, you can remove unwanted binaries by running
```
$ make clean
```
