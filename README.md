# vm-riskxvii
Simple virtual machine that can run RISC-V instructions.

Examples and tests contain binaries that the VM can run. mi_translations contains readable 'English' translations of the binaries.
vm_memory.h contains the structs and function declarations regarding the VM's virtual memory.
vm_methods.h contains the function declarations for the actual operations the VM can execute.
vm_riskxvii.c contains the function definitions and main program that takes in the binary and runs it on the VM.
