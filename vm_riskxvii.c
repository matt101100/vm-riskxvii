#include <string.h>
#include <ctype.h>

#include "vm_memory.h"
#include "utils.h"
#include "vm_methods.h"

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

        if (vm.pc < 0 || vm.pc > 1023) {
            printf("Invalid program position reached.\n");
            return 1;
        }
        
        // get instruction and opcode
        uint32_t instruction = get_instruction(&vm);
        uint8_t opcode = get_opcode(instruction);

        // !! TESTING ONLY !!
        // translate_mi(instruction);
        // vm.pc+=4;
        // continue;

        // executing instructions
        int instruction_label = determine_instruction_label(opcode, instruction);
        switch (instruction_label)
        {
            case (add):
                execute_math_type_R(instruction, add, &vm);
                break;
            
            case (addi):
                execute_addi(instruction, &vm);
                break;
            
            case (sub):
                execute_math_type_R(instruction, sub, &vm);
                break;
            
            case (lui):
                execute_lui(instruction, &vm);
                break;
            
            case (xor):
                execute_logical_type_R(instruction, xor, &vm);
                break;
            
            case (xori):
                execute_logical_immediate(instruction, xori, &vm);
                break;
            
            case (or):
                execute_logical_type_R(instruction, or, &vm);
                break;
            
            case (ori):
                execute_logical_immediate(instruction, ori, &vm);
                break;
            
            case (and):
                execute_logical_type_R(instruction, and, &vm);
                break;
            
            case (andi):
                execute_logical_immediate(instruction, andi, &vm);
                break;
            
            case (sll):
                execute_shift(instruction, sll, &vm);
                break;
            
            case (srl):
                execute_shift(instruction, srl, &vm);
                break;
            
            case (sra):
                execute_shift(instruction, sra, &vm);
                break;
            
            case (lb):
                running = execute_load(instruction, lb, &vm);
                break;
            
            case (lh):
                running = execute_load(instruction, lh, &vm);
                break;
            
            case (lw):
                running = execute_load(instruction, lw, &vm);
                break;
            
            case (lbu):
                running = execute_load(instruction, lbu, &vm);
                break;

            case (lhu):
                running = execute_load(instruction, lhu, &vm);
                break;
            
            case (sb):
                running = execute_store(instruction, sb, &vm);
                break;
            
            case (sh):
                running = execute_store(instruction, sh, &vm);
                break;
            
            case (sw):
                running = execute_store(instruction, sw, &vm);
                break;
            
            case (slt):
                execute_register_set(instruction, slt, &vm);
                break;
            
            case (slti):
                execute_register_set(instruction, slti, &vm);
                break;
            
            case (sltu):
                execute_register_set(instruction, sltu, &vm);
                break;
            
            case (sltiu):
                execute_register_set(instruction, sltiu, &vm);
                break;
            
            case (beq):
                execute_branch(instruction, beq, &vm);
                break;
            
            case (bne):
                execute_branch(instruction, bne, &vm);
                break;

            case (blt):
                execute_branch(instruction, blt, &vm);
                break;
            
            case (bltu):
                execute_branch(instruction, bltu, &vm);
                break;
            
            case (bge):
                execute_branch(instruction, bge, &vm);
                break;
            
            case (bgeu):
                execute_branch(instruction, bgeu, &vm);
                break;
            
            case (jal):
                execute_jal(instruction, &vm);
                break;
            
            case (jalr):
                execute_jalr(instruction, &vm);
                break;
            
            default:
                // invalid
                printf("Instruction Not Implemented: 0x%08x\n", instruction);
                printf("PC = 0x%08x;\n", vm.pc);
                register_dump(&vm);
                return 1;
        }
    }

    return 0;
}

void initialize_virtual_machine(virtual_machine *vm) {
    // initialize memory and registers to 0
    memset(vm->memory, 0, MEMORY_SIZE * sizeof(uint8_t));
    memset(vm->registers, 0, NUM_REGISTERS * sizeof(uint32_t));
    memset(vm->heap, 0, HEAP_SIZE * sizeof(uint8_t));

    vm->pc = 0; // start of the instruction memory
    vm->head = NULL; // no blocks assigned
    vm->total_allocated_memory = 0; // no memory allocated at start
}

FILE *open_machine_instructions(char filename[], virtual_machine *vm) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return NULL;
    }

    // read the instruction memory and data memory into their respective arrays
    size_t bytes_read = load_image_into_memory(fp, vm->memory);
    if (bytes_read == -1) {
        // file contains invalid data
        return NULL;
    }

    return fp;

}

size_t load_image_into_memory(FILE *fp , uint8_t memory[]) {
    size_t bytes_read = fread(memory, sizeof(uint8_t), MEMORY_SIZE, fp);
    if (bytes_read < MEMORY_SIZE) {
        // file did not contain the valid amount of data 
        // ie: it contained less than 2048 bytes
        return -1;
    }
    return bytes_read;
}

uint8_t check_valid_heap_memory_access(uint32_t mem_address,
                                       virtual_machine *vm, uint8_t data_size) {
    /*
     * We need to check that mem_address + data_size falls inside a currently
       allocated block, with enough memory to hold that data
     */
    if (vm->head == NULL) {
        return 0;
    }

    block *current_block = vm->head;
    uint32_t block_end_pointer = current_block->mem_base_address + current_block->usable_mem_size;
    while (current_block != NULL) {
        block_end_pointer = current_block->mem_base_address + current_block->usable_mem_size;

        if (mem_address <= block_end_pointer) {
            if (data_size <= current_block->usable_mem_size) {
                return 1;
            }
        }
        current_block = current_block->next;
    }
    return 0;
}

// NOTE: binary is in little endian so the binary operations account for this

uint32_t get_instruction(virtual_machine* vm) {
    uint32_t instruction = (vm->memory[vm->pc]) |
                           ((vm->memory[vm->pc + 1] << 8)) |
                           ((vm->memory[vm->pc + 2] << 16)) |
                           ((vm->memory[vm->pc + 3] << 24));
    return instruction;
}

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

void register_dump(virtual_machine *vm) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        printf("R[%d] = 0x%08x;\n", i, vm->registers[i]);
    }
}

void get_operation_resources_type_R(uint32_t instruction, uint8_t *target, 
                             uint8_t source[]) {
    // gets the resources needed to run R-type ops
    *target = get_target_register(instruction);
    get_source_registers(instruction, R, source);
}

// !! executes for each machine instruction defined below !!

void execute_math_type_R(uint32_t instruction, int instruction_label,
                         virtual_machine *vm) {
    uint8_t target = get_target_register(instruction);
    if (target == 0) {
        // ignore writes to zero register
        vm->pc += 4;
        return;
    }
    uint8_t source[2];
    get_source_registers(instruction, R, source);

    switch (instruction_label)
    {
        case (add):
            vm->registers[target] = vm->registers[source[0]] 
                                    + vm->registers[source[1]];
            break;
        
        case (sub):
            vm->registers[target] = vm->registers[source[0]]
                                    - vm->registers[source[1]];
            break;
    }
    vm->pc += 4;
}

void execute_logical_type_R(uint32_t instruction, int instruction_label,
                            virtual_machine *vm) {
    uint8_t target = get_target_register(instruction);
    if (target == 0) {
        vm->pc += 4;
        return;
    }
    uint8_t source[2];
    get_source_registers(instruction, R, source);

    switch (instruction_label)
    {
        case (xor):
            vm->registers[target] = vm->registers[source[0]] 
                                ^ vm->registers[source[1]];
            break;
        
        case (or):
            vm->registers[target] = vm->registers[source[0]] 
                                | vm->registers[source[1]];
            break;
        
        case (and):
            vm->registers[target] = vm->registers[source[0]] 
                                & vm->registers[source[1]];
            break;
    }
    vm->pc += 4;
}

void execute_logical_immediate(uint32_t instruction, int instruction_label,
                               virtual_machine *vm) {
    uint8_t target = get_target_register(instruction);
    if (target == 0) {
        vm->pc += 4;
        return;
    }
    uint8_t source[2];
    get_source_registers(instruction, I, source);
    uint32_t immediate = extract_immediate_number(instruction, I);

    switch (instruction_label)
    {
        case (xori):
            vm->registers[target] = vm->registers[source[0]] ^ immediate;
            break;
        
        case (ori):
            vm->registers[target] = vm->registers[source[0]] | immediate;
            break;
        
        case (andi):
            vm->registers[target] = vm->registers[source[0]] & immediate;
            break;
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

void execute_shift(uint32_t instruction, int instruction_label,
                   virtual_machine *vm) {
    uint8_t target = get_target_register(instruction);
    if (target == 0) {
        vm->pc += 4;
        return;
    }
    uint8_t source[2];
    get_source_registers(instruction, R, source);

    uint32_t num = vm->registers[source[0]];
    uint32_t shift = vm->registers[source[1]];

    switch (instruction_label)
    {
        case (sll):
            vm->registers[target] = num << shift;
            break;
        
        case (srl):
            vm->registers[target] = num >> shift;
            break;
        
        case (sra):
            vm->registers[target] = (num >> shift) 
                                    | (num << (sizeof(uint32_t) - shift));
            break;
    }
    vm->pc += 4;
}

int execute_load(uint32_t instruction, int instruction_label, 
                  virtual_machine *vm) {
    // get registers and immediate
    uint8_t target = get_target_register(instruction);
    uint8_t source[2];
    get_source_registers(instruction, I, source);
    uint32_t immediate = extract_immediate_number(instruction, I);

    // save the memory address we are loading from for comparison
    uint32_t mem_address = vm->registers[source[0]] + immediate;
    char read_char = 0;
    int read_int = 0;
    switch (mem_address)
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
            if (target == 0) {
                break;
            }
            
            switch (instruction_label)
            {
                case (lb):
                    // load byte
                    if (mem_address >= 0xb700) {
                        if (check_valid_heap_memory_access(mem_address, vm, 1)) {
                            vm->registers[target] = sign_extend(vm->heap[mem_address - 0xb700], 8);
                            break;
                        }
                        printf("Illegal Operation: 0x%08x\n", instruction);
                        printf("PC = 0x%08x;\n", vm->pc);
                        register_dump(vm);
                        return 0;
                    }

                    vm->registers[target] = sign_extend(vm->memory[mem_address], 8);
                    break;
                
                case (lh):
                    // load half word -- 16 bits
                    if (mem_address >= 0xb700) {
                        if (check_valid_heap_memory_access(mem_address, vm, 2)) {
                            vm->registers[target] = sign_extend(vm->heap[(mem_address - 0xb700)] | vm->heap[(mem_address - 0xb700) + 1] << 8, 16);
                            break;
                        }
                        printf("Illegal Operation: 0x%08x\n", instruction);
                        printf("PC = 0x%08x;\n", vm->pc);
                        register_dump(vm);
                        return 0;
                    }

                    vm->registers[target] = sign_extend(vm->memory[mem_address] | vm->memory[(mem_address) + 1] << 8, 16);
                    break;
                
                case (lw):
                    // load word -- 32 bits
                    if (mem_address >= 0xb700) {
                        if (check_valid_heap_memory_access(mem_address, vm, 4)) {
                            vm->registers[target] = sign_extend(vm->heap[(mem_address - 0xb700)] |
                                        vm->heap[(mem_address - 0xb700) + 1] << 8 |
                                        vm->heap[(mem_address - 0xb700) + 2] << 16 |
                                        vm->heap[(mem_address - 0xb700) + 3] << 24, 32);
                            break;
                        }
                        printf("Illegal Operation: 0x%08x\n", instruction);
                        printf("PC = 0x%08x;\n", vm->pc);
                        register_dump(vm);
                        return 0;
                    }

                    vm->registers[target] = sign_extend(vm->memory[mem_address] |
                                        vm->memory[mem_address + 1] << 8 |
                                        vm->memory[mem_address + 2] << 16 |
                                        vm->memory[mem_address + 3] << 24, 32);
                    break;
                    
                case (lbu):
                    // load byte but treat val as unsigned (don't sign extend)
                    if (mem_address >= 0xb700) {
                        if (check_valid_heap_memory_access(mem_address, vm, 1)) {
                            vm->registers[target] = vm->heap[mem_address - 0xb700];
                            break;
                        }
                        printf("Illegal Operation: 0x%08x\n", instruction);
                        printf("PC = 0x%08x;\n", vm->pc);
                        register_dump(vm);
                        return 0;
                    }

                    vm->registers[target] = vm->memory[mem_address];
                    break;

                
                case (lhu):
                    // load half word but treat val as unsigned
                    if (mem_address >= 0xb700) {
                        if (check_valid_heap_memory_access(mem_address, vm, 2)) {
                            vm->registers[target] = vm->heap[mem_address - 0xb700] | vm->heap[mem_address - 0xb700 + 1] << 8;
                            break;
                        }
                        printf("Illegal Operation: 0x%08x\n", instruction);
                        printf("PC = 0x%08x;\n", vm->pc);
                        register_dump(vm);
                        return 0;
                    }

                    vm->registers[target] = vm->memory[mem_address] | vm->memory[mem_address + 1] << 8;
                    break;
            }

    }
    vm->pc += 4;
    return 1;
}

int execute_store(uint32_t instruction, int instruction_label,
                  virtual_machine *vm) {
    uint8_t source[2];
    get_source_registers(instruction, S, source);
    uint32_t immediate = extract_immediate_number(instruction, S);
    block *current;
    block *prev;

    uint32_t mem_address = vm->registers[source[0]] + immediate; // to write
    switch (mem_address)
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
             * --> output written value as unsigned 32-bit number in lower-case
             * hex format
             */
            printf("%x", vm->registers[source[1]]);
            break;
        
        case (0x0824):
            /*
             * Dump register banks
             * --> prints the value of all registers
             */
            register_dump(vm);
            break;
        
        case(0x0828):
            /*
             * Dump memory word
             * --> prints the value of M[v], with index v being the value to
             * store interpreted as a 32-bit unsigned int
             */
            printf("%08x", vm->memory[vm->registers[source[1]]] |
                         vm->memory[vm->registers[source[1]] + 1] << 8 |
                         vm->memory[vm->registers[source[1]] + 2] << 16 |
                         vm->memory[vm->registers[source[1]] + 3] << 24);
            break;
        
        case (0x080C):
            /*
            * Halt
            * --> print halt to stdout and send flag signalling end of program
            */
            printf("CPU Halt Requested\n");
            return 0;
        
        case (0x0830):
            /*
             * Virtual malloc
             * --> allocates bytes = value being stored as memory
             * memory is rounded up to the nearest 64 byte block
             * pointer to memory chunk stored in register 28
             */
            if (vm->registers[source[1]] <= 0) {
                // non-valid amount of bytes request
                printf("Invalid amount of memory requested.\n");
                vm->registers[28] = 0;
                break;
            } else if ((vm->total_allocated_memory + vm->registers[source[1]]) > HEAP_SIZE) {
                // not enough memory left to fulfil request
                vm->registers[28] = 0;
                break;
            }

            /*
             * Create the new block and update its fields to store the meta-data
               of the newly 'allocated' memory
             */
            block new_block;
            new_block.usable_mem_size = vm->registers[source[1]];
            new_block.next = NULL;
            new_block.total_mem_size = 0;
            while(new_block.total_mem_size < vm->registers[source[1]]) {
                 /*
                 * increments by 64 until total_mem_size is at least equal to
                   requested memory amount.
                 * Ensures total_mem_size is always a multiple of 64
                 */
                new_block.total_mem_size += 64;
            }
            
            // update list of nodes with new block at the end
            if (vm->head == NULL) {
                vm->head = &new_block;
            } else {
                block *current_node = vm->head;
                while (current_node->next != NULL) {
                    current_node = current_node->next;
                }
                current_node->next = &new_block;
            }
            
            // store pointer to the first byte of the allocated block
            vm->registers[28] = 0xb700 + vm->total_allocated_memory;
            new_block.mem_base_address = 0xb700 + vm->total_allocated_memory;

            // update total amount of allocated memory
            vm->total_allocated_memory += new_block.total_mem_size;
            break;
        
        case (0x0834):
            /*
             * Virtual free
             * frees block of memory starting at byte = to value being stored
             */

            // to deallocate a block, we just need to remove its node from list
            current = vm->head;
            prev = vm->head;
            while (current != NULL) {
                if (current == vm->head && current->mem_base_address == vm->registers[source[1]]) {
                    // head was requested for deletion
                    vm->head = vm->head->next;
                    vm->total_allocated_memory -= vm->head->total_mem_size;

                } else {
                    if ((current->mem_base_address == vm->registers[source[1]]) && current) {
                        prev->next = current->next;
                        vm->total_allocated_memory -= current->total_mem_size;
                    } else {
                        prev = current;
                        if (prev == NULL) {
                            break;
                        }
                        current = current->next;
                    }
                }
            }

        default:
            switch (instruction_label)
            {
                case (sb):
                    // store byte
                    if (mem_address >= 0xb700) {
                        // accessing heap memory
                        // check if mem_address falls inside an alloc'd block
                        if (check_valid_heap_memory_access(mem_address, vm, 1)) {
                            vm->heap[mem_address - 0xb700] = vm->registers[source[1]];
                            break;
                        }
                        printf("Illegal Operation: 0x%08x\n", instruction);
                        printf("PC = 0x%08x;\n", vm->pc);
                        register_dump(vm);
                        return 0;
                    }

                    vm->memory[mem_address] = vm->registers[source[1]];
                    break;
                    
                
                case (sh):
                    /*
                     * store half word -- 16 bits
                     * mask and shift to store in 8-bit chunks at adjacent
                     * indices
                     */
                    if (mem_address >= 0xb700) {
                        if (check_valid_heap_memory_access(mem_address, vm, 2)) {
                            vm->heap[mem_address - 0xb700] = vm->registers[source[1]] & 0xFF;
                            vm->heap[(mem_address - 0xb700) + 1] = (vm->registers[source[1]] >> 8) & 0xFF;
                            break;
                        }
                        printf("Illegal Operation: 0x%08x\n", instruction);
                        printf("PC = 0x%08x;\n", vm->pc);
                        register_dump(vm);
                        return 0;
                    }

                    vm->memory[mem_address] = vm->registers[source[1]] & 0xFF;
                    vm->memory[mem_address + 1] = (vm->registers[source[1]] >> 8) & 0xFF;
                    break;
                
                case (sw):
                    // store word -- 32 bits
                    if (mem_address >= 0xb700) {
                        if (check_valid_heap_memory_access(mem_address, vm, 4)) {
                            vm->heap[mem_address - 0xb700] = vm->registers[source[1]] & 0xFF;
                            vm->heap[(mem_address - 0xb700) + 1] = (vm->registers[source[1]] >> 8) & 0xFF;
                            vm->heap[(mem_address - 0xb700) + 2] = (vm->registers[source[1]] >> 16) & 0xFF; 
                            vm->heap[(mem_address - 0xb700) + 3] = (vm->registers[source[1]] >> 24) & 0xFF; 
                            break;
                        }
                        printf("Illegal Operation: 0x%08x\n", instruction);
                        printf("PC = 0x%08x;\n", vm->pc);
                        register_dump(vm);
                        return 0;
                    }

                    vm->memory[mem_address] = vm->registers[source[1]] & 0xFF;
                    vm->memory[mem_address + 1] = (vm->registers[source[1]] >> 8) & 0xFF;
                    vm->memory[mem_address + 2] = (vm->registers[source[1]] >> 16) & 0xFF; 
                    vm->memory[mem_address + 3] = (vm->registers[source[1]] >> 24) & 0xFF; 
                    break;

            }
    }
    vm->pc += 4;
    return 1;
}

void execute_register_set(uint32_t instruction, int instruction_label,
                          virtual_machine *vm) {
    uint8_t target = get_target_register(instruction);
    uint8_t source[2];
    get_source_registers(instruction, R, source);
    uint32_t immediate = extract_immediate_number(instruction, I);

    switch (instruction_label)
    {
        case (slt):
            if ((int32_t)vm->registers[source[0]] 
                < (int32_t)vm->registers[source[1]]) {

                vm->registers[target] = 1;
            } else {
                vm->registers[target] = 0;
            }
        
        case (slti):
            if ((int32_t)vm->registers[source[0]] < (int32_t)immediate) {
                vm->registers[target] = 1;
            } else {
                vm->registers[target] = 0;
            }
        
        case (sltu):
            if (vm->registers[source[0]] < vm->registers[source[1]]) {
                vm->registers[target] = 1;
            } else {
                vm->registers[target] = 0;
            }
        
        case (sltiu):
            if (vm->registers[source[0]] < immediate) {
                vm->registers[target] = 1;
            } else {
                vm->registers[target] = 0;
            }
    }
    vm->pc += 4;
}

void execute_branch(uint32_t instruction, int instruction_label,
                    virtual_machine *vm) {
    uint32_t immediate = extract_immediate_number(instruction, SB);
    uint8_t source[2];
    get_source_registers(instruction, SB, source);

    switch (instruction_label)
    {
        case (beq):
            if (vm->registers[source[0]] == vm->registers[source[1]]) {
                vm->pc += immediate;
                return;
            }
            break;
        
        case (bne):
            if (vm->registers[source[0]] != vm->registers[source[1]]) {
                vm->pc += immediate;
                return;
            }
            break;
        
        case (blt):
            if ((int32_t)vm->registers[source[0]] 
                < (int32_t)vm->registers[source[1]]) {

                vm->pc += immediate;
                return;
            }
            break;
        
        case (bltu):
            if (vm->registers[source[0]] < vm->registers[source[1]]) {
                vm->pc += immediate;
                return;
            }
            break;
        
        case (bge):
            if ((int32_t)vm->registers[source[0]] 
                >= (int32_t)vm->registers[source[1]]) {

                vm->pc += immediate;
                return;
            }
            break;
        
        case (bgeu):
            if (vm->registers[source[0]] >= vm->registers[source[1]]) {
                vm->pc += immediate;
                return;
            }
            break;
    }
    vm->pc += 4;
}

void execute_jal(uint32_t instruction, virtual_machine *vm) {
    // get target and immediate
    uint8_t target = get_target_register(instruction);
    uint32_t immediate = extract_immediate_number(instruction, UJ);
    // The target register stores the pc of the NEXT instruction

    if (target != 0) {
        // ignore writes to zero register
        vm->registers[target] = vm->pc + 4;
    }
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
        vm->registers[target] = vm->pc + 4;
    }
    vm->pc = (vm->registers[source[0]] + immediate); // update pc
}