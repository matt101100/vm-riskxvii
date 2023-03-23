#include <string.h>

#include "vm_memory.h"
#include "utils.h"
#include "vm_methods.h"

void translate_mi(uint32_t instruction) {
    uint8_t opcode = get_opcode(instruction);
    int label = determine_instruction_label(opcode, instruction);
    uint8_t target = get_target_register(instruction);
    uint8_t source[2];
    get_source_registers(instruction, R, source);
    uint32_t immediate;

    if (label == add) {
        printf("I = add, ");
        immediate = extract_immediate_number(instruction, R);
    } else if (label == addi) {
        printf("I = addi, ");
        immediate = extract_immediate_number(instruction, I);
    } else if (label == sub) {
        printf("I = sub, ");
        immediate = extract_immediate_number(instruction, R);
    } else if (label == lui) {
        printf("I = lui, ");
        immediate = extract_immediate_number(instruction, U);
    } else if (label == xor) {
        printf("I = xor, ");
        immediate = extract_immediate_number(instruction, R);
    } else if (label == xori) {
        printf("I = xori, ");
        immediate = extract_immediate_number(instruction, I);
    } else if (label == or) {
        printf("I = or, ");
        immediate = extract_immediate_number(instruction, R);
    } else if (label == ori) {
        printf("I = ori, ");
        immediate = extract_immediate_number(instruction, I);
    } else if (label == and) {
        printf("I = and, ");
        immediate = extract_immediate_number(instruction, R);
    } else if (label == andi) {
        printf("I = andi, ");
        immediate = extract_immediate_number(instruction, I);
    } else if (label == sll) {
        printf("I = sll, ");
        immediate = extract_immediate_number(instruction, R);
    } else if (label == srl) {
        printf("I = srl, ");
        immediate = extract_immediate_number(instruction, R);
    } else if (label == sra) {
        printf("I = sra, ");
        immediate = extract_immediate_number(instruction, R);
    } else if (label == lb) {
        printf("I = lb, ");
        immediate = extract_immediate_number(instruction, I);
    } else if (label == lh) {
        printf("I = lh, ");
        immediate = extract_immediate_number(instruction, I);
    } else if (label == lw) {
        printf("I = lw, ");
        immediate = extract_immediate_number(instruction, I);
    } else if (label == lbu) {
        printf("I = lbu, ");
        immediate = extract_immediate_number(instruction, I);
    } else if (label == lhu) {
        printf("I = lhu, ");
        immediate = extract_immediate_number(instruction, I);
    } else if (label == sb) {
        printf("I = sb, ");
        immediate = extract_immediate_number(instruction, S);
    } else if (label == sh) {
        printf("I = sh, ");
        immediate = extract_immediate_number(instruction, S);
    } else if (label == sw) {
        printf("I = sw, ");
        immediate = extract_immediate_number(instruction, S);
    } else if (label == slt) {
        printf("I = slt, ");
        immediate = extract_immediate_number(instruction, R);
    } else if (label == slti) {
        printf("I = slti, ");
        immediate = extract_immediate_number(instruction, I);
    } else if (label == sltu) {
        printf("I = sltu, ");
        immediate = extract_immediate_number(instruction, R);
    } else if (label == sltiu) {
        printf("I = sltiu, ");
        immediate = extract_immediate_number(instruction, I);
    } else if (label == beq) {
        printf("I = beq, ");
        immediate = extract_immediate_number(instruction, SB);
    } else if (label == bne) {
        printf("I = bne, ");
        immediate = extract_immediate_number(instruction, SB);
    } else if (label == blt) {
        printf("I = blt, ");
        immediate = extract_immediate_number(instruction, SB);
    } else if (label == bltu) {
        printf("I = bltu, ");
        immediate = extract_immediate_number(instruction, SB);
    } else if (label == bge) {
        printf("I = bge, ");
        immediate = extract_immediate_number(instruction, SB);
    } else if (label == bgeu) {
        printf("I = bgeu, ");
        immediate = extract_immediate_number(instruction, SB);
    } else if (label == jal) {
        printf("I = jal, ");
        immediate = extract_immediate_number(instruction, UJ);
    } else if (label == jalr) {
        printf("I = jalr, ");
        immediate = extract_immediate_number(instruction, I);
    }
    printf("rd = r%d, ", target);
    for (int i = 0; i < 2; i++) {
        printf("rs%d = r%d, ", i + 1, source[i]);
    }
    printf("imm = %d\n", immediate);

}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid number of arguments supplied.\n");
        return 1;
    }
    
    // initializing vm
    virtual_machine vm;
    initialize_virtual_machine(&vm);

    // open the instruction file and read the data into instruction memory
    FILE *machine_instruction_ptr = open_machine_instructions(argv[1], &vm);
    if (machine_instruction_ptr == NULL) {
        printf("Invalid file path provided or error loading image.\n");
        return 1;
    }
    fclose(machine_instruction_ptr);

    // main virtual machine loop
    int running = 1;
    while (running) {

        // get the current instruction and extract the opcode from it
        if (vm.pc < 0) {
            printf("Invalid program position reached.\n");
            return 1;
        }
        uint32_t instruction = vm.instruction_memory[vm.pc / 4];
        translate_mi(instruction);
        continue;
        uint8_t opcode = get_opcode(instruction);
        /*
         * This array stores the func3 and func7 bytes with
         * func3 = additional_opcode[0], func7 = addtional_opcode[1]
         */
        int instruction_label = determine_instruction_label(opcode, instruction);

        // executing instructions
        switch (instruction_label)
        {
            case (add):
                execute_add(instruction, &vm);
                break;
            
            case (addi):
                execute_addi(instruction, &vm);
                break;
            
            case (sub):
                printf("sub\n");
                break;
            
            case (lui):
                execute_lui(instruction, &vm);
                break;
            
            case (xor):
                printf("xor\n");
                break;
            
            case (xori):
                printf("xori\n");
                break;
            
            case (or):
                printf("or\n");
                break;
            
            case (ori):
                printf("ori\n");
                break;
            
            case (and):
                printf("and\n");
                break;
            
            case (andi):
                printf("andi\n");
                break;
            
            case (sll):
                printf("sll\n");
                break;
            
            case (srl):
                printf("srl\n");
                break;
            
            case (sra):
                printf("sra\n");
                break;
            
            case (lb):
                printf("lb\n");
                break;
            
            case (lh):
                printf("lh\n");
                break;
            
            case (lw):
                execute_lw(instruction, &vm);
                break;
            
            case (lbu):
                execute_lbu(instruction, &vm);
                break;

            case (lhu):
                printf("lhu\n");
                break;
            
            case (sb):
                running = execute_sb(instruction, &vm);
                break;
            
            case (sh):
                printf("sh\n");
                break;
            
            case (sw):
                running = execute_sw(instruction, &vm);
                break;
            
            case (slt):
                printf("slt\n");
                break;
            
            case (slti):
                printf("slti\n");
                break;
            
            case (sltu):
                printf("sltu\n");
                break;
            
            case (sltiu):
                printf("sltiu\n");
                break;
            
            case (beq):
                execute_beq(instruction, &vm);
                break;
            
            case (bne):
                execute_bne(instruction, &vm);
                break;

            case (blt):
                printf("blt\n");
                break;
            
            case (bltu):
                printf("bltu\n");
                break;
            
            case (bge):
                printf("bge\n");
                break;
            
            case (bgeu):
                printf("bgeu\n");
                break;
            
            case (jal):
                execute_jal(instruction, &vm);
                break;
            
            case (jalr):
                execute_jalr(instruction, &vm);
                break;
            
            default:
                // invalid
                printf("Invalid\n");
                return 1;
        }
        // printf("pc: %d\n", (vm.pc));
        // for (int i = 0; i < NUM_REGISTERS; i++) {
        //     if (vm.registers[i] == 0) {
        //         continue;
        //     }
        //     printf("reg %d = %d\n", i, vm.registers[i]);
        // }
        // printf("\n");

    }

    return 0;
}

void initialize_virtual_machine(virtual_machine *vm) {
    // initialize memory and registers to 0
    memset(vm->instruction_memory, 0, MEMORY_SIZE * sizeof(uint32_t));
    memset(vm->data_memory, 0, MEMORY_SIZE * sizeof(uint32_t));
    memset(vm->registers, 0, NUM_REGISTERS * sizeof(int));

    vm->pc = 0; // start of the instruction memory
    vm->head = NULL; // no other blocks assigned
}

FILE *open_machine_instructions(char filename[], virtual_machine *vm) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return NULL;
    }

    // read the instruction memory and data memory into their respective arrays
    size_t instruction_bytes_read = load_image_into_memory(fp, vm->instruction_memory);
    size_t data_bytes_read = load_image_into_memory(fp, vm->data_memory);
    if (instruction_bytes_read == -1 || data_bytes_read == -1) {
        // file contains invalid data
        return NULL;
    }

    return fp;

}

size_t load_image_into_memory(FILE *fp , uint32_t memory[]) {
    size_t bytes_read = fread(memory, sizeof(uint32_t), MEMORY_SIZE, fp);
    if (bytes_read < MEMORY_SIZE) {
        // file did not contain the valid amount of data 
        // ie: it contained less than 1024 bytes
        return -1;
    }
    return bytes_read;
}

// NOTE: binary is in little endian so the binary operations account for this

uint8_t get_opcode(uint32_t instruction) {
    // apply a bit mask to get the first 6 bits of the 32-bit instruction
    return instruction & 0x7F;
}

void get_additional_opcode(uint32_t instruction, int instruction_type,
                            uint8_t additional_opcodes[]) {
    additional_opcodes[0] = (instruction & 0x7000) >> 12;
    additional_opcodes[1] = 0; // default value that does not overlap
    if (instruction_type == R) {
        // extract the func7 bits in the case of an R-type instruction
        additional_opcodes[1] = (instruction & 0xFE000000) >> 25;
    }
}

uint8_t get_target_register(uint32_t instruction) {
    return (instruction & 0xF80) >> 7;
}

void get_source_registers(uint32_t instruction, int instruction_type,
                               uint8_t source_registers[]) {
    source_registers[0] = (instruction & 0xF8000) >> 15; // rs1
    source_registers[1] = -1; // default rs2
    if (instruction_type != I) {
        // extract second source register for types R, S, SB but not I
        source_registers[1] = (instruction & 0x1F00000) >> 20;
    }

}

uint32_t extract_immediate_number(uint32_t instruction, int instruction_type) {
    int32_t res = 0;
    if (instruction_type == I) {
        // bits imm[11:0] are found at bits instruction[31:20] for I-types
        // there are 12 bits in the immediate of an I-type
        res = sign_extend((instruction & 0xFFF00000) >> 20, 12);
    } else if (instruction_type == S) {
        // imm[11:5] = instruction[31:25] and imm[4:0] = instruction[11:7]
        // also 12 bits in immediate of an I-type
        res = sign_extend(((instruction & 0xF80) >> 7) |
                          ((instruction & 0xFE000000) >> 20), 12);
    } else if (instruction_type == SB) {
        // imm[12] = instruction[31], imm[10:5] = instruction[30:25]
        // imm[11] = instruction[7], imm[4:1] = instruction[11:8]
        res = sign_extend(((instruction & 0x80000000) >> 19) | ((instruction & 0x7E000000) >> 20) | ((instruction & 0x80) << 4) | ((instruction & 0xF00) >> 7), 13);
    } else if (instruction_type == U) {
        // imm[31:12] = instruction[31:12]
        res = sign_extend((instruction & 0xFFFFF000), 20);
    } else if (instruction_type == UJ) {
        // imm[20] = instruction[31], imm[10:1] = instruction[30:21]
        // imm[11] = instruction[20], imm[19:12] = instruction[19:12]
        res = sign_extend(((instruction & 0x80000000) >> 11) | ((instruction & 0x7FE00000) >> 20) | ((instruction & 0x100000) >> 9) | ((instruction & 0xFF000)), 20);
    }

    return res;
}

uint32_t sign_extend(int32_t num, int original_bit_count) {
    if ((num >> (original_bit_count - 1)) & 1) { // check if the sign bit is set
        num |= (0xFFFFFFFF << original_bit_count); // sign extend if set
    }
    return num;
}

int determine_instruction_label(uint8_t opcode, uint32_t instruction) {
    uint8_t additional_opcodes[2] = { 0 };
    switch (opcode) 
    {
        case (0x33):
            // type R
            get_additional_opcode(instruction, R, additional_opcodes);
            if (additional_opcodes[1] == 0x20) {
                if (additional_opcodes[0] == 0x0) {
                    return sub;
                } else if (additional_opcodes[0] == 0x5) {
                    return sra;
                }
            } else if (additional_opcodes[1] == 0x0) {
                // all others have func7 = 0
                switch (additional_opcodes[0])
                {
                    case (0x0):
                        return add;
                    
                    case (0x4):
                        return xor;
                    
                    case (0x6):
                        return or;
                    
                    case (0x7):
                        return and;
                    
                    case (0x1):
                        return sll;
                    
                    case (0x5):
                        return srl;
                    
                    case (0x2):
                        return slt;
                    
                    case (0x3):
                        return sltu;
                }
            }

        case (0x13):
            // type I, opcode = 0010011
            get_additional_opcode(instruction, I, additional_opcodes);
            switch (additional_opcodes[0])
            {
                case (0x0):
                    return addi;
                
                case (0x4):
                    return xori;
                
                case (0x6):
                    return ori;
                
                case(0x7):
                    return andi;

                case (0x2):
                    return slti;

                case (0x3):
                    return sltiu; 
            }
            
        case (0x03):
            // type I, opcode = 0000011
            get_additional_opcode(instruction, I, additional_opcodes);
            switch (additional_opcodes[0])
            {
                case (0x0):
                    return lb;
            
                case (0x1):
                    return lh;
                
                case (0x2):
                    return lw;
            
                case (0x4):
                    return lbu;
            
                case (0x5):
                    return lhu;
            }

        case (0x67):
            // type I -- jalr
            get_additional_opcode(instruction, I, additional_opcodes);
            if (additional_opcodes[0] == 0x0) {
                return jalr;
            }
        
        case (0x37):
            // type U -- lui
            return lui;
        
        case (0x23):
            // type S
            get_additional_opcode(instruction, S, additional_opcodes);
            switch (additional_opcodes[0])
            {
                case (0x0):
                    return sb;
                
                case (0x1):
                    return sh;
                
                case (0x2):
                    return sw;
            }
        
        case (0x63):
            // type SB
            get_additional_opcode(instruction, SB, additional_opcodes);
            switch (additional_opcodes[0])
            {
                case (0x0):
                    return beq;
                
                case (0x1):
                    return bne;
                
                case (0x4):
                    return blt;
                
                case (0x6):
                    return bltu;
                
                case (0x5):
                    return bge;
                
                case (0x7):
                    return bgeu;
            }
        
        case (0x6F):
            // type UJ -- jal
            return jal;
    }
    // invalid instruction label
    return -1;
}

// !! executes for each machine instruction defined below !!

void execute_add(uint32_t instruction, virtual_machine *vm) {
    // get registers
    uint8_t target = get_target_register(instruction);
    uint8_t source[2];
    get_source_registers(instruction, R, source);

    // add the nums in source registers and store in target
    if (target != 0) {
        vm->registers[target] = vm->registers[source[0]] + vm->registers[source[1]];
    }

    vm->pc += 4;
}

void execute_addi(uint32_t instruction, virtual_machine *vm) {
    // get registers and immediate
    uint8_t target = get_target_register(instruction);
    uint8_t source[2];
    get_source_registers(instruction, I, source);
    uint32_t immediate = extract_immediate_number(instruction, I);

    // add the immediate to source and store at target
    if (target != 0) {
        vm->registers[target] = vm->registers[source[0]] + immediate;
    }

    // update pc to move onto next instruction
    vm->pc += 4;
}

void execute_sub(uint32_t instruction, virtual_machine *vm) {

}

void execute_lui(uint32_t instruction, virtual_machine *vm) {
    // get target and immediate
    uint8_t target = get_target_register(instruction);
    uint32_t immediate = extract_immediate_number(instruction, U);

    // store the upper 31:12 immediate bits into the target register
    if (target != 0) {
        vm->registers[target] = immediate & 0xFFFFF000;
    }

    // update pc to move onto next instruction
    vm->pc += 4;
}

void execute_xor(uint32_t instruction, virtual_machine *vm) {

}

void execute_xori(uint32_t instruction, virtual_machine *vm) {

}

void execute_or(uint32_t instruction, virtual_machine *vm) {

}

void execute_ori(uint32_t instruction, virtual_machine *vm) {

}

void execute_and(uint32_t instruction, virtual_machine *vm) {

}

void execute_andi(uint32_t instruction, virtual_machine *vm) {

}

void execute_sll(uint32_t instruction, virtual_machine *vm) {

}

void execute_slr(uint32_t instruction, virtual_machine *vm) {

}

void execute_sra(uint32_t instruction, virtual_machine *vm) {

}

void execute_lb(uint32_t instruction, virtual_machine *vm) {

}

void execute_lh(uint32_t instruction, virtual_machine *vm) {

}

int execute_lw(uint32_t instruction, virtual_machine *vm) {
    // get registers and immediate
    uint8_t target = get_target_register(instruction);
    uint8_t source[2];
    get_source_registers(instruction, I, source);
    uint32_t immediate = extract_immediate_number(instruction, I);

    // save the memory address we are loading from for comparison
    uint32_t memory_address = (vm->registers[source[0]] + immediate);
    char read_char = 0;
    int read_int = 0;
    switch (memory_address)
    {
        case (0x0812):
            /*
             * Console read char
             * --> Scan stdin for char input and load into target
             */
            if (scanf("%c", &read_char) != 1) {
                // failed to read input
                printf("Error reading input.\n");
                // clears the buffer
                int c;
                while((c = getchar()) != '\n' && c != EOF);
                return 0;
            }
            
            // store input into target register
            vm->registers[target] = read_char;
            break;
        
        case (0x0816):
            /*
             * Console read signed int
             * --> Scan stdin for signed int input and load into target
             */
            if (scanf("%d", &read_int) != 1) {
                printf("Error reading input.\n");
                int c;
                while((c = getchar()) != '\n' && c != EOF);
                return 0;
            }

            // store input value into target register
            vm->registers[target] = read_int;
            break;
        
        default:
            // load the 32-bit value into target register
            if (target == 0) {
                break;
            }
            vm->registers[target] = vm->data_memory[(vm->registers[source[0]] + immediate) / 4];
    }
    vm->pc += 4;
    return 1;

}

int execute_lbu(uint32_t instruction, virtual_machine *vm) {
    // get registers and immediate
    uint8_t target = get_target_register(instruction);
    uint8_t source[2];
    get_source_registers(instruction, I, source);
    uint32_t immediate = extract_immediate_number(instruction, I);

    // save the memory address we are loading from for comparison
    uint32_t memory_address = (vm->registers[source[0]] + immediate);
    char read_char = 0;
    int read_int = 0;
    switch (memory_address)
    {
        case (0x0812):
            /*
             * Console read char
             * --> Scan stdin for char input and load into target
             */
            if (scanf("%c", &read_char) != 1) {
                // failed to read input
                printf("Error reading input.\n");
                // clears the buffer
                int c;
                while((c = getchar()) != '\n' && c != EOF);
                return 0;
            }
            
            // store input into target register
            vm->registers[target] = read_char;
            break;
        
        case (0x0816):
            /*
             * Console read signed int
             * --> Scan stdin for signed int input and load into target
             */
            if (scanf("%d", &read_int) != 1) {
                printf("Error reading input.\n");
                int c;
                while((c = getchar()) != '\n' && c != EOF);
                return 0;
            }

            // store input value into target register
            vm->registers[target] = read_int;
            break;
        
        default:
            // load the 8-bit value into target register
            if (target == 0) {
                break;
            }
            vm->registers[target] = vm->data_memory[(vm->registers[source[0]] + immediate) / 4];
    }
    vm->pc += 4;
    return 1;
}

void execute_lhu(uint32_t instruction, virtual_machine *vm) {

}

int execute_sb(uint32_t instruction, virtual_machine *vm) {
    // get source registers, immediate
    uint8_t source[2];
    get_source_registers(instruction, S, source);
    uint32_t immediate = extract_immediate_number(instruction, S);

    uint32_t memory_address = vm->registers[source[0]] + immediate; // to write
    switch (memory_address)
    {
        case (0x0800):
            /*
             * console write char
             * --> output written value as char to stdout
             */
            printf("%c", vm->registers[source[1]]);
            break;
        
        case (0x0804):
            /*
             * console write signed int
             * --> output written value as signed 32-bit decimal number
             */ 
            printf("%d", vm->registers[source[1]]);
            break;
        
        case (0x0808):
            /*
             * console write unsigned int
             * --> output written value as unsigned 32-bit decimal number
             */
            printf("%d", (uint32_t)vm->registers[source[1]]);
            break;
        
        case (0x080C):
            /*
            * Halt
            * --> print message to stdout and send a flag signalling end of program
            */
            printf("CPU Halt Requested\n");
            return 0;
        
        default:
            // update requested data memory address
            vm->data_memory[(vm->registers[source[0]] + immediate) / 4] = (uint8_t)vm->registers[source[1]];
            break;
    }
    vm->pc += 4;
    return 1;
}

void execute_sh(uint32_t instruction, virtual_machine *vm) {

}

int execute_sw(uint32_t instruction, virtual_machine *vm) {
    // get source registers, immediate
    uint8_t source[2];
    get_source_registers(instruction, S, source);
    uint32_t immediate = extract_immediate_number(instruction, S);

    uint32_t memory_address = vm->registers[source[0]] + immediate; // to write
    switch (memory_address)
    {
        case (0x0800):
            /*
             * console write char
             * --> output written value as char to stdout
             */
            printf("%c", vm->registers[source[1]]);
            break;
        
        case (0x0804):
            /*
             * console write signed int
             * --> output written value as signed 32-bit decimal number
             */ 
            printf("%d", vm->registers[source[1]]);
            break;
        
        case (0x0808):
            /*
             * console write unsigned int
             * --> output written value as unsigned 32-bit decimal number
             */
            printf("%d", (uint32_t)vm->registers[source[1]]);
            break;
        
        case (0x080C):
            /*
            * Halt
            * --> print message to stdout and send a flag signalling end of program
            */
            printf("CPU Halt Requested\n");
            return 0;
        
        default:
            // update requested data memory address
            vm->data_memory[((vm->registers[source[0]] + immediate) / 4)] = vm->registers[source[1]];
            break;
    }
    vm->pc += 4;
    return 1;
}

void execute_slt(uint32_t instruction, virtual_machine *vm) {

}

void execute_slti(uint32_t instruction, virtual_machine *vm) {

}

void execute_sltu(uint32_t instruction, virtual_machine *vm) {

}

void execute_sltiu(uint32_t instruction, virtual_machine *vm) {

}

void execute_beq(uint32_t instruction, virtual_machine *vm) {
    uint32_t immediate = extract_immediate_number(instruction, SB);
    uint8_t source[2];
    get_source_registers(instruction, SB, source);

    if (vm->registers[source[0]] == vm->registers[source[1]]) {
        vm->pc += immediate;
        return;
    }
    vm->pc += 4;
}

void execute_bne(uint32_t instruction, virtual_machine *vm) {
    uint32_t immediate = extract_immediate_number(instruction, SB);
    uint8_t source[2];
    get_source_registers(instruction, SB, source);

    if (vm->registers[source[0]] != vm->registers[source[1]]) {
        vm->pc += immediate;
        return;
    }
    vm->pc += 4;
}

void execute_blt(uint32_t instruction, virtual_machine *vm) {

}

void execute_bltu(uint32_t instruction, virtual_machine *vm) {

}

void execute_bge(uint32_t instruction, virtual_machine *vm) {

}

void execute_bgeu(uint32_t instruction, virtual_machine *vm) {

}

void execute_jal(uint32_t instruction, virtual_machine *vm) {
    // get target and immediate
    uint8_t target = get_target_register(instruction);
    uint32_t immediate = extract_immediate_number(instruction, UJ);
    // The target register stores the pc of the NEXT instruction

    if (target != 0) {
        // ignore writes to zero register
        vm->registers[target] = vm->pc + 1;
    }
    vm->registers[target] = vm->pc + 4;
    vm->pc = (vm->pc + immediate); // immediate is already shifted
}

void execute_jalr(uint32_t instruction, virtual_machine *vm) {
    // get registers and immediate
    int8_t target = get_target_register(instruction);
    uint8_t source[2];
    get_source_registers(instruction, I, source);
    uint32_t immediate = extract_immediate_number(instruction, I);

    if (target != 0) {
        // ignore writes to zero register
        vm->registers[target] = vm->pc + 1;
    }
    vm->pc = (vm->registers[source[0]] + immediate); // update pc
}