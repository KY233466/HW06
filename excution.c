/**************************************************************
 *                     execution.c
 * 
 *     Assignment: Homework 6 - Universal Machine Program
 *     Authors: Katie Yang (zyang11) and Pamela Melgar (pmelga01)
 *     Date: November 24, 2021
 *
 *     Purpose: This file executes the program for the Universal Machine
 *              to be able to run and pass any necessary components to other 
 *              modules
 *     
 *     Success Output:
 *              exit_sucess
 *              
 *     Failure output:
 *              exit_faliure
 *                  
 **************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "excution.h"
#include "read_file.h"
#include "memory_manager.h"
#include "register_manager.h"
#include "instruction_retrieval.h"
#include "seq.h"
#include "bitpack.h"

#define BYTESIZE 8

void print(Memory all_memory);
void printRs(Registers all_registers);


/*
* execute
* Purpose: This function in the execution module serves as the outer 
*          driving module that initializes the program segments and 
*          registers, and runs all the instructions contained in the 0th
*          segment This outer function ensures that all module instances 
*          get freed as needed and also manages the word counter that will 
*          execute the program by accessing uint32_t words in segment0.
* Parameters: FILE *input: the file pointer to the input containing all 
*             the word instructions that will be parsed and interpreted
*             through the readFile module.
* Returns: nothing, runs the program
* Notes:
*/
void excute(FILE *input)
{
    /*Initialize memory and register manager to default state*/
    Memory all_segments = initialize_memory();
    Registers all_registers = initialize_registers();

    /*Populate the 0th segment based on input*/
    readFile(input, all_segments);

    uint32_t program_counter = 0; /*start program counter at beginning*/
    
    /*Run through all instructions in segment0, note that counter may loop*/
    while (program_counter < segmentlength(all_segments, 0)) {
        uint32_t instruction = get_word(all_segments, 0, program_counter);
        Info info = get_Info(instruction);
        program_counter++;

        /*program executer is passed in to update (in loop) if needed*/
        instruction_executer(info, all_segments, all_registers, 
                             &program_counter);
    }

    free_registers(all_registers);
    free_segments(all_segments);
    exit(EXIT_FAILURE); /*failure mode, out of bounds of $m[0]*/
}
