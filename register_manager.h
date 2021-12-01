/**************************************************************
 *                     register_manager.h
 * 
 *     Assignment: Homework 6 - Universal Machine Program
 *     Authors: Katie Yang (zyang11) and Pamela Melgar (pmelga01)
 *     Date: November 24, 2021
 *
 *     Purpose: Interface for register_manager. Allows the rest of the program
 *              to create an instance of the set of registers, as will be 
 *              manipulated throughout the program depending on the 
 *              instruction. It allows the client to set and access values
 *              within those registers, (so long as those indices are not out 
 *              of bounds).
 *     
 *     Success Output:
 *              Depends on the function used
 * 
 *     Failure output:
 *              Run-time errors
 *                  
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef REGISTER_MANAGER_H
#define REGISTER_MANAGER_H

/*A struct pointer to create a hidden instance of the Register manager*/
typedef struct Registers *Registers;


/*
* initialize_registers
* Purpose: To create an instance of a Register manager struct that contains 
*          the 8 initialized uint32_t registers to be used throughout the 
*          program.
* Input: none
* Expected Output: an instance of a register_manager pointer with 8
*                  newly initialized registers
* Note: 
*/
Registers initialize_registers();


/*
* get_register_value
* Purpose: To retrieve the uint32_t value contained within one of the 8 
*          UM program registers, at an index indicated by the parameter
* Input: A reference to an initialized Registers instance and an integer 
*        representing which register index the value is desired from
* Expected Output: a uint32_t representing the value contained at the 
*                  desired register
* Note: 
*/
uint32_t get_register_value(Registers all_registers, uint32_t num_register);

/*
* set_register_value
* Purpose: To set/change the uint32_t value contained within one of the 8 
*          UM program registers, at a register indicated by the parameter, 
*          with a value indicated by the parameter.
* Input: A reference to an initialized Registers instance, an integer 
*        representing which register index the word to change is located, 
*        and a uint32_t containing the value that will replace the old value
*        contained in that register
* Expected Output: none -- sets a value
* Note: 
*/
void set_register_value(Registers all_registers, uint32_t num_register, 
                        uint32_t value);


/*
* free_segments
* Purpose: To free all the allocated memory taken up by the registers
*          and uint32_t words within those registers.
* Input: an instance of the register manager
* Expected Output: none
* Note: all_registers must not be null and MUST be initialized
*/
void free_registers(Registers all_registers);

#endif