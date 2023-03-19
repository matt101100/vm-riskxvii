#include <string.h>

#include "vm_memory.h"
#include "utils.h"
#include "vm_methods.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid number of arguments supplied.\n");
        return 1;
    }

    virtual_machine vm;

    // initializing vm
    initialize_virtual_machine(&vm);

    // open the instruction file and read the data into instruction memory
    FILE *machine_instruction_ptr = open_machine_instructions(argv[1], &vm);
    if (machine_instruction_ptr == NULL) {
        printf("Invalid file path provided or error loading image.\n");
        return 1;
    }

    // main virtual machine loop
    int running = 1;
    while (running) {
        if (vm.pc == 32) { // temporaray halting for TESTING !! REMOVE !!
            // reached end of instructions
            running = 0;
        }

        // get the current instruction and extract the opcode from it
        uint32_t instruction = vm.instruction_memory[vm.pc];
        uint8_t opcode = get_opcode(instruction);
        /*
         * This array stores the func3 and func7 bytes with
         * func3 = additional_opcode[0], func7 = addtional_opcode[1]
         */
        uint8_t additional_opcodes[2] = { 0 };
        int instruction_label = determine_instruction_label(opcode, additional_opcodes);
        printf("instr: %x\n", instruction);
        printf("opcode: %x\n", opcode);
        printf("running: %d\n", running);
        printf("pc: %d\n", vm.pc);

        // executing instructions
        switch (instruction_label)
        {
            case (add):
                // execute add
                printf("add\n");
                break;
            
            case (addi):
                // execute addi
                execute_addi(instruction, &vm);
                break;
            
            case (sub):
                //execute sub
                printf("sub\n");
                break;
            
            case (lui):
                //execute lui
                execute_lui(instruction, &vm);
                break;
            
            case (xor):
                // execute xor
                printf("xor\n");
                break;
            
            case (xori):
                // execute xori
                printf("xori\n");
                break;
            
            case (or):
                // execute or
                printf("or\n");
                break;
            
            case (ori):
                // execute ori
                printf("ori\n");
                break;
            
            case (and):
                // execute and
                printf("and\n");
                break;
            
            case (andi):
                // execute andi
                printf("andi\n");
                break;
            
            case (sll):
                // execute sll
                printf("sll\n");
                break;
            
            case (srl):
                // execute srl
                printf("srl\n");
                break;
            
            case (sra):
                // execute sra
                printf("sra\n");
                break;
            
            case (lb):
                // execute lb
                printf("lb\n");
                break;
            
            case (lh):
                // execute lh
                printf("lh\n");
                break;
            
            case (lw):
                // execute lw
                printf("lw\n");
                break;
            
            case (lbu):
                // execute lbh
                printf("lbh\n");
                break;

            case (lhu):
                // execute lhu
                printf("lhu\n");
                break;
            
            case (sb):
                // execute sb
                running = execute_sb(instruction, &vm);
                break;
            
            case (sh):
                // execute sh
                printf("sh\n");
                break;
            
            case (sw):
                // execute sw
                printf("sw\n");
                break;
            
            case (slt):
                // execute slt
                printf("slt\n");
                break;
            
            case (slti):
                // execute slti
                printf("slti\n");
                break;
            
            case (sltu):
                // execute sltu
                printf("sltu\n");
                break;
            
            case (sltiu):
                // execute sltiu
                printf("sltiu\n");
                break;
            
            case (beq):
                // execute beq
                printf("beq\n");
                break;
            
            case (bne):
                // execute bne
                printf("bne\n");
                break;

            case (blt):
                // execute blt
                printf("blt\n");
                break;
            
            case (bltu):
                // execute bltu
                printf("bltu\n");
                break;
            
            case (bge):
                // execute bge
                printf("bge\n");
                break;
            
            case (bgeu):
                // execute bgeu
                printf("bgeu\n");
                break;
            
            case (jal):
                // execute jal
                execute_jal(instruction, &vm);
                break;
            
            case (jalr):
                // execute jalr
                execute_jalr(instruction, &vm);
                break;
        }
    }

    return 0;
}

void initialize_virtual_machine(virtual_machine *vm) {
    // initialize memory and registers to 0
    memset(vm->instruction_memory, 0, MEMORY_SIZE * sizeof(uint32_t));
    memset(vm->data_memory, 0, MEMORY_SIZE * sizeof(uint32_t));
    memset(vm->registers, 0, MEMORY_SIZE);
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

    fclose(fp);

    return fp;

}

size_t load_image_into_memory(FILE *fp , uint32_t memory[]) {
    size_t bytes_read = fread(memory, sizeof(uint32_t), MEMORY_SIZE, fp);
    if (bytes_read < 32) {
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
    additional_opcodes[0] = instruction & 0x7000;
    additional_opcodes[1] = 0; // default value that does not overlap
    if (instruction_type == R) {
        // extract the func7 bits in the case of an R-type instruction
        additional_opcodes[1] = instruction & 0xFE000000;
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
        // bits imm[11:0] are found at bits instruction[31:20] for R-types
        // there are 12 bits in the immediate of an R-type
        res = sign_extend((instruction & 0xFFF00000) >> 20, 12);
    } else if (instruction_type == S) {
        // imm[11:5] = instruction[31:25] and imm[4:0] = instruction[11:7]
        // also 12 bits in immediate of an I-type
        res = sign_extend(((instruction & 0xF80) >> 7) |
                          ((instruction & 0xFE000000) >> 20), 12);
    } else if (instruction_type == SB) {
        // imm[12] = instruction[31], imm[10:5] = instruction[30:25]
        // imm[11] = instruction[7], imm[4:1] = instruction[11:8]
        res = sign_extend(((instruction & 0x80000000) >> 19) | ((instruction & 0x7E000000) >> 15) | ((instruction & 0x80) >> 7) | ((instruction & 0xF00) << 4), 13);
    } else if (instruction_type == U) {
        // imm[31:12] = instruction[31:12]
        res = sign_extend((instruction & 0xFFFFF000) >> 12, 20);
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

int determine_instruction_label(uint8_t opcode, uint8_t addtional_opcodes[]) {
    switch (opcode) 
    {
        case (0x33):
            // type R
            if (addtional_opcodes[1] == 0x20) {
                if (addtional_opcodes[0] == 0x0) {
                    return sub;
                } else if (addtional_opcodes[0] == 0x5) {
                    return sra;
                }
            } else if (addtional_opcodes[1] == 0x0) {
                // all others have func7 = 0
                switch (addtional_opcodes[0])
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
            switch (addtional_opcodes[0])
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
            switch (addtional_opcodes[0])
            {
                case (0x0):
                    return lb;
            
                case (0x1):
                    return lh;
            
                case (0x4):
                    return lbu;
            
                case (0x5):
                    return lhu;
            }

        case (0x67):
            // type I -- jalr
            if (addtional_opcodes[0] == 0x0) {
                return jalr;
            } else {
                return -1;
            }
        
        case (0x37):
            // type U -- lui
            return lui;
        
        case (0x23):
            // type S
            switch (addtional_opcodes[0])
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
            switch (addtional_opcodes[0])
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

}

void execute_addi(uint32_t instruction, virtual_machine *vm) {
    // get registers and immediate
    uint8_t target = get_target_register(instruction);
    uint8_t source[2];
    get_source_registers(instruction, I, source);
    uint32_t immediate = extract_immediate_number(instruction, I);

    // add the immediate to source and store at target
    vm->registers[target] = vm->registers[source[0]] + immediate;

    // update pc to move onto next instruction
    vm->pc++;
}

void execute_sub(uint32_t instruction, virtual_machine *vm) {

}

void execute_lui(uint32_t instruction, virtual_machine *vm) {
    // get target and immediate
    uint8_t target = get_target_register(instruction);
    uint32_t immediate = extract_immediate_number(instruction, U);

    // store the immediate in the target register
    vm->registers[target] = immediate;

    // update pc to move onto next instruction
    vm->pc++;
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

void execute_lw(uint32_t instruction, virtual_machine *vm) {

}

void execute_lbu(uint32_t instruction, virtual_machine *vm) {

}

void execute_lhu(uint32_t instruction, virtual_machine *vm) {

}

int execute_sb(uint32_t instruction, virtual_machine *vm) {
    // get source registers, immediate
    uint8_t source[2];
    get_source_registers(instruction, S, source);
    uint32_t immediate = extract_immediate_number(instruction, S);

    uint32_t memory_address = source[0] + immediate; // to write to
    switch (memory_address)
    {
        case (0x0800):
            /*
             * console write char
             * --> output written value as char to stdout
             */
            printf("%c\n", vm->registers[source[1]]);
        
        case (0x0804):
            /*
             * console write signed int
             * --> output written value as signed 32-bit decimal number
             */ 
            printf("%d\n", vm->registers[source[1]]);
        
        case (0x0808):
            /*
             * console write unsigned int
             * --> output written value as unsigned 32-bit decimal number
             */
            printf("%d\n", (uint32_t)vm->registers[source[1]]);
        
        case (0x080C):
            /*
            * Halt
            * --> print message to stdout and send a flag signalling end of program
            */
            printf("CPU Halt Requested\n");
            return 0;
        
        default:
            // update requested data memory address
            printf("%d\n", vm->registers[source[0]] );
            printf("%d\n", source[0]);
            vm->data_memory[(vm->registers[source[0]] + immediate) / 32] = source[1];
    }
    // update pc to move onto next instruction
    vm->pc++;
    return 1;

}

void execute_sh(uint32_t instruction, virtual_machine *vm) {

}

void execute_sw(uint32_t instruction, virtual_machine *vm) {

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

}

void execute_bne(uint32_t instruction, virtual_machine *vm) {

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
    /*
     * The target reg should store the pc of the NEXT instruction
     * Hence, we +1 and store that
     * Note: +1 since each index in instruction memory is 32 bits (4 bytes)
     * so we should only jump 1 index == jumping 4 bytes
     */
    vm->registers[target] = vm->pc + 1;
    vm->pc = (vm->pc + (immediate / 4)) ; // immediate is already shifted
}

void execute_jalr(uint32_t instruction, virtual_machine *vm) {
    // get registers and immediate
    int8_t target = get_target_register(instruction);
    uint8_t source[2];
    get_source_registers(instruction, I, source);
    uint32_t immediate = extract_immediate_number(instruction, I);

    // update pc
    vm->registers[target] = vm->pc + 1;
    vm->pc = (vm->registers[source[0]] + immediate) / 4;
}