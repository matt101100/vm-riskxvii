#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#include "vm_memory.h"

/*
 * Desc: Opens the file containing machine instructions with binary read flags
 * Params: path to the file, pointer to the virtual machine struct
 * Return: pointer to the beginning of the file
 */
FILE *open_machine_instructions(char filename[], virtual_machine *vm);

/*
 *Desc: Reads the instructions / data into corresponding memory
 * Params: pointer to an open file, pointer to the virtual machine
 * Return: the number of bytes read
 */
size_t load_image_into_memory(FILE *fp, uint32_t memory[]);



#endif