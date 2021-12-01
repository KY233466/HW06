/**************************************************************
 *                     instruction_retrieval.h
 * 
 *     Assignment: Homework 6 - Universal Machine Program
 *     Authors: Katie Yang (zyang11) and Pamela Melgar (pmelga01)
 *     Date: November 24, 2021
 * 
 *     interface for instruction_retrieval
 *
 *     Purpose: Extrat nessisary information from the instruction
 *              and calles respective functions to excute
 *              the instruction.
 *     
 *     Success Output:
 *              Depending on the instruction, void or exit_sucess
 * 
 *     Failure output:
 *             exit_faliure
 *                  
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "register_manager.h"
#include "memory_manager.h"

#ifndef INSTRUCTION_RETRIEVAL_H
#define INSTRUCTION_RETRIEVAL_H

/* A struct pointer to the interpreted uint32_t word instruction*/
typedef struct Info *Info;


/*
* get_info
* Purpose: To initialize an Info struct based on the unpacking of 
*          the uint32_t word instruction. This struct pointer will 
*          contain the reference to the separated values that 
*          contain the necesary informatin to execute instructoins in 
*           instruction_executer
* Input: A uint32_t word instruction that repreesnts the packed version
*        of the instruction to be executed (created from read_module)
* Returns: An Info struct storing the information of the instruction
* Expectation: None-void parameter
*/
Info get_Info(uint32_t instruction);


/*
* instruction_executer
* Purpose: Call respective functions to execute the instruction based on the
*          information stored in the recieved Info struct.
* Input: An Info elmenet storing information of the instruction, a Memory
*          element storing all the segements, a Reister element storing
*          all the registers, a uint32_t* for program counter, and a file
*          pointer for input.
* Returns: void
* Expectation: None-void parameters
*/
void instruction_executer(Info info, Memory all_segments, 
                          Registers all_registers, uint32_t *counter);

#endif