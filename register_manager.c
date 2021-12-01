/**************************************************************
 *                     register_manager.c
 * 
 *     Assignment: Homework 6 - Universal Machine Program
 *     Authors: Katie Yang (zyang11) and Pamela Melgar (pmelga01)
 *     Date: November 24, 2021
 * 
 *     implementation for register_manager.h
 *
 *     Purpose: Allows the rest of the program to create an instance of the 
 *              set of registers, as will be manipulated throughout the 
 *              program depending on the instruction. It allows the client 
 *              to set and access values within those registers, (so long 
 *              as those indices are not out of bounds).
 *     
 *     Success Output:
 *              
 *     Failure output:
 *              Run-time errors
 * 
 *     Note:
 *          There are 8 registers in total
 *                  
 **************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "register_manager.h"
#include "uarray.h"
#include "assert.h"

#define num_registers 8


/*
* struct Registers
* Purpose: To manage the 8 REGISTERS used throughout the program and keep
*          secrets from the client of how these REGISTERS are represented 
*          under-the-hood.
* Members: UArray_T - the data structure to hold the 8 registers
*                   used throughout the program. Each element of the 
*                   UArray_T registers will store a uint32_t word instruction
* Notes: The client cannot see this struct Registers implmentation, and will 
*        only have access to a pointer to this struct to keep secrets but 
*        also to avoid invalid behavior
*/
struct Registers
{
    UArray_T registers;
};


/*
* initialize_registers
* Purpose: To create an instance of a Registers Manager struct pointer that 
*           will store a reference to the UArray of registers to be 
*           used throughout the program.
* Parameters: none
* Returns: a struct pointer to the intialized Registers manager containing 
*          8 registers initialized to a value of 0, with memory allocated 
*          on the heap
* Notes: Will fail if memory cannot be allocated for the struct pointer
*/
struct Registers *initialize_registers()
{
    struct Registers *all_registers = malloc(sizeof(struct Registers));
    assert(all_registers != NULL);
    
    all_registers->registers = UArray_new(num_registers, sizeof(uint32_t));

    /* Create the 8 registers based on constant of 8*/
    for (int i = 0; i < num_registers; i++) {
        uint32_t *word = UArray_at(all_registers->registers, i);
        *word = 0;
    }

    return all_registers;
}


/*
* get_register_value
* Purpose: To retreive a uint32_t word instruction from a desired Register
*          given the index to indicate which register to retrieve a value from
* Parameters: struct Registers *all_registers - a struct pointer to an 
*                   instance of an initialized Registers manager
*             int num_register - an integer indicating which register 
*                   to retrieve the uint32_t value from
* Returns: a uint32_t word instruction that had been stored within the 
*               register at the desired index
* Notes: all_registers must be non-NULL and initialized, and the num_register 
*        index must be in-bounds
*/
uint32_t get_register_value(Registers all_registers, uint32_t num_register)
{
    assert(all_registers != NULL);
    assert(8 >= num_register);

    return *(uint32_t*)UArray_at(all_registers->registers, num_register);
}


/*
* set_register_value
* Purpose: To set/change a uint32_t word instruction in a desired Register
*          given the index to indicate which register to change a value in
* Parameters: struct Registers *all_registers - a struct pointer to an 
*                   instance of an initialized Registers manager
*             uint32_t num_register - an integer indicating which register's 
*                   value to replace/change
*             uint32_t value - the uint32_t word instruction to be 
*                   inserted into one of the registers
* Returns: nothing -- changes a value
* Notes: all_registers must be non-NULL and initialized, and the
*        num_registers integer must be in-bounds. 
*/
void set_register_value(Registers all_registers, uint32_t num_register, 
                        uint32_t value)
{
    assert(all_registers != NULL);
    assert(8 >= num_register);

    uint32_t *word = UArray_at(all_registers->registers, num_register);
    *word = value;
}


/*
* free_registers
* Purpose: To free all the allocated memory taken up by the Registers
*          and words within those regisers. To allow the client to have 
*          quick-access to avoiding memory leaks if the program needs to
*          abort via a checked-runtime-error, if the program counter reaches 
*          the end of segment 0, or a halt-instruction is encountered.
* Parameters: struct Registers *all_registers - a struct pointer to an 
*                   instance of an initialized Registers manager
* Returns: nothing -- changes a value
* Notes: all_registers must be non-NULL and initialized,
*/ 
void free_registers(struct Registers *all_registers)
{
    UArray_free(&(all_registers->registers));
    free(all_registers);
}