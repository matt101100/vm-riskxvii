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
        int32_t imm_num;

        // executing instructions
        switch (instruction_label)
        {
            case (add):
                // execute add
                printf("add\n");
                break;
            
            case (addi):
                // execute addi
                imm_num = extract_immediate_number(instruction, I);
                printf("imm: %d\n", imm_num);
                printf("addi\n");
                break;
            
            case (sub):
                //execute sub
                printf("sub\n");
                break;
            
            case (lui):
                //execute lui
                printf("lui\n");
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
                printf("sb\n");
                imm_num = extract_immediate_number(instruction, I);
                printf("imm: %d\n", imm_num);
                printf("addi\n");
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
                printf("jal\n");
                break;
            
            case (jalr):
                // execute jalr
                printf("jalr\n");
                break;
        }
        

        // update program counter
        vm.pc++;
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
    uint32_t mask = 0x7F;
    return instruction & mask;
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

int32_t extract_immediate_number(uint32_t instruction, int instruction_type) {
    int32_t res = 0;
    if (instruction_type == I) {
        // bits imm[11:0] are found at bits instruction[31:20] for R-types
        // shift since we want these as bits 11:0 for the immediate
        int32_t pre_shifted_bits = instruction & 0xFFF00000; // bits in-place
        res = pre_shifted_bits >> 20;
    } else if (instruction_type == S) {
        // imm[11:5] = instruction[31:25] and imm[4:0] = instruction[11:7]
        /*
         * part_1 = instruction[11:7]
         * part_2 = instruction[31:25]
         * both pre-shifting so we are just getting the bits in-place
         */
        // 11111110000000000000000000000000
        int32_t pre_shifted_part_1 = instruction & 0xF80;
        int32_t pre_shifted_part_2 = instruction & 0xFE000000;
        // shifting
        int32_t shifted_1 = pre_shifted_part_1 >> 7;
        int32_t shifted_2 = pre_shifted_part_2 >> 20;
        // bitwise or to carry over set bits
        res = shifted_1 | shifted_2;
    } else if (instruction_type == SB) {
        // imm[12] = instruction[31], imm[10:5] = instruction[30:25]
        // imm[11] = instruction[7], imm[4:1] = instruction[11:8]
        /*
         * part_1 = instruction[31]
         * part_2 = instruction[30:25]
         * part_3 = instruction[7]
         * part_4 = instruction[11:8]
         */
        uint32_t pre_shifted_part_1 = instruction & 0x80000000;
        uint32_t pre_shifted_part_2 = instruction & 0x7E000000;
        uint32_t pre_shifted_part_3 = instruction & 0x80;
        uint32_t pre_shifted_part_4 = instruction & 0xF00;
        // shifting
        uint32_t shifted_1 = pre_shifted_part_1 >> 31;
        uint32_t shifted_2 = pre_shifted_part_2 >> 25;
        uint32_t shifted_3 = pre_shifted_part_3 >> 7;
        uint32_t shifted_4 = pre_shifted_part_4 >> 8;
        // bitwise or to carry over set bits
        res = shifted_1 | shifted_2 | shifted_3 | shifted_4;
    } else if (instruction_type == U) {

    } else if (instruction_type == UJ) {

    }

    return res;
}

// int32_t sign_extend(int32_t num) {
//     int32_t res = (int32_t)num;
//     /*
//      * note we use the mask below as it represents the MSB of
//      * a 32-bit unsigned int
//      */
//     if (num & 0x80000000) { // check if the number is negative
//         // sign extend negative numbers with 1s
//         res |= 0xffffff80;
//     }
//     return res;
// }

int32_t sign_extend(int32_t x) {
    int64_t m = 1UL << (sizeof(x) * 8 - 1);
    int64_t r = (int64_t)x & ((1UL << sizeof(x) * 8) - 1);
    r = (r ^ m) - m;
    return (int32_t)r;
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

}