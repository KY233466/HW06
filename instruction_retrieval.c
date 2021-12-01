/**************************************************************
 *                     instruction_retrieval.c
 * 
 *     Assignment: Homework 6 - Universal Machine Program
 *     Authors: Katie Yang (zyang11) and Pamela Melgar (pmelga01)
 *     Date: November 24, 2021
 * 
 *     implementation for instruction_retrieval.h
 *
 *     Purpose: Extract necessary information from the instruction
 *              and calls respective functions to excute
 *              the instruction.
 *     
 *     Success Output:
 *              void or exit_success
 *              
 *     Failure output:
 *              Exit_faliure
 * 
 *     Note: 
 *              Available instructions include:
 *              Conditional Move, Segmented Load, Segmented Store, 
 *              Addition, Multiplication, Division, Bitwise NAND, 
 *              Halt, Map Segment, Unmap Segment, Output,  Input, 
 *              Load Program, and Load Value
 *                  
 **************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "instruction_retrieval.h"
#include "bitpack.h"
#include "assert.h" 

typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;

#define num_registers 8
#define two_pow_32 4294967296
uint32_t MIN = 0;   
uint32_t MAX = 255;

/*
* struct Info
* Purpose: To combine all of the parsed information retrieved from an 
*          interpreted instruction into a single type. This way the 
*          Info struct can be passed around without needing to call 
*          other functions to extract this information. The client 
*          will have to create an instance of this struct and then pass it
*          through to the execution of instructions once it is initialized
* Members: uint32_t op - this value represents the last 4 bits of the 
*                        word instruction, or the opcode, indicating which 
*                        of the defined instructions this word is meant to 
*                        execute.
*          uint32_t rA - this value is the index of the first register 
*                        specified in the word instruction, LSB 6. 
*          uint32_t rB - this value is the index of the second register 
*                        specified in the word instruction, LSB 3. 
*          uint32_t rC - this value is the index of the third register 
*                        specified in the word instruction, LSB 0.
*          uint32_t value - this value is for the special case ('load value')
*                           instruction when the first 25 bits are dedicated 
*                           for a value to load into a register. 
* Notes: The client cannot see this struct Info implmentation, and will 
*        only have access to a pointer to this struct
*/
struct Info
{
    uint32_t op;
    uint32_t rA;
    uint32_t rB;
    uint32_t rC;
    uint32_t value;
};

/*Helper functions used throughout the specified instructino executions*/

/*performs conditional move --- instruction: 0*/
void conditional_move(struct Info info, Registers all_registers);

/*performs segment load op --- instruction: 1*/
void segmented_load(struct Info info, Registers all_registers, 
                    Memory all_segements);
/*performs segment store op --- instruction: 2*/
void segmented_store(struct Info info, Registers all_registers, 
                     Memory all_segements);

/*performs arithmetic operations --- instruction: 3-6*/
void arithmetics(struct Info info, Registers all_registers, 
                 uint32_t arith_code);

/*performs output--- instruction: 10*/
void output(struct Info info, Registers all_registers);

/*performs input--- instruction: 11*/
void input(struct Info info, Registers all_registers);

/*performs load value --- instruction: 13*/
void load_val(struct Info info, Registers all_registers);

/*performs map--- instruction: 8*/
void map_a_segment(struct Info info, Registers all_registers, 
                   Memory all_segments);

/*performs unmap --- instruction: 9*/
void unmap_a_segment(struct Info info, Registers all_registers, 
                     Memory all_segments);

/*performs load program --- instruction: 12*/
void load_program(struct Info info, Registers all_registers, 
                  Memory all_segments, uint32_t *counter);

/*performs halt program --- instruction: 7*/
void halt_program(struct Info *info, Registers all_registers, 
                  Memory all_segments);


/*
* get_info
* Purpose: To interpret a given uint32_t instruction and assign 
*          corresponding values to the struct Info members that will then 
*          be given back to the function caller.
* Parameters: uint32_t instruction - the 32-bit word instruction that has 
*                      been parsed through in a previous module and only 
*                      contains the bitpacked instructions that we need to 
*                      unpack and interpret
* Returns: a pointer to an Info struct containing all of the instruction
*          execution information seperated from the uint32_t word.
* Notes: 
*/
struct Info *get_Info(uint32_t instruction)
{
    struct Info *info = malloc(sizeof(struct Info));
    assert(info != NULL); /*Check if heap allocation successful*/

    uint32_t op = (int)Bitpack_getu(instruction, 4, 28);
    info->op = op;

    if (op != 13){
        info->rA = (int)Bitpack_getu(instruction, 3, 6);
        info->rB = (int)Bitpack_getu(instruction, 3, 3);
        info->rC = (int)Bitpack_getu(instruction, 3, 0);
    }
    else {
        info->rA = (int)Bitpack_getu(instruction, 3, 25);
        info->value = (int)Bitpack_getu(instruction, 25, 0);
    }
    return info;
}


/*
* instruction_executer
* Purpose: To run a defined instruction 0-13 based on the opcode of 
*          the Info struct. To seperate the functions that interpret the 
*          uint32_t word vs. the function that actually executes the 
*          proper instruction or handles invalid instructions.
* Parameters: Info info - a struct pointer to the information containing 
*                         the seperated unpacked instruction values.
*             Memory all_segments - a reference to the segment manager 
*                       used throughout the program.
*             Registers all_registers - a reference to the register manager 
*                       used throughout the program.
*             uint32_t *counter - a reference to the program counter 
*                       that is keeping track of which instruction to 
*                       execute in segment 0.
* Returns: nothing - executes an instruction and ends
* Notes: struct pointer info must be non-NULL
*        all_segments must not be NULL
*        all_registers must be non-NULL 
*        reference to program counter must be non-NULL
*/
void instruction_executer(Info info, Memory all_segments, 
                          Registers all_registers, uint32_t *counter) 
{
    assert(info != NULL);
    assert(all_segments != NULL);
    assert(all_registers != NULL);
    assert(counter != NULL);

    uint32_t code = info->op;
    
    /*We want a halt instruction to execute quicker*/
    if (code == HALT) {
        halt_program(info, all_registers, all_segments);
    }

    /*Rest of instructions*/
    if (code == CMOV) {
        conditional_move(*info, all_registers);
    }
    else if (code == SLOAD) {
        segmented_load(*info, all_registers, all_segments);
    }
    else if (code == SSTORE) {
        segmented_store(*info, all_registers, all_segments);
    }
    else if (code == ADD || code == MUL || code == DIV || code == NAND) {
        arithmetics(*info, all_registers, code);
    }
    else if (code == ACTIVATE) {
        map_a_segment(*info, all_registers, all_segments);
    }
    else if (code == INACTIVATE) {
        unmap_a_segment(*info, all_registers, all_segments);
    }
    else if (code == OUT) {
        output(*info, all_registers);
    }
    else if (code == IN) {
        input(*info, all_registers);
    }
    else if (code == LOADP) {
        load_program(*info, all_registers, all_segments, counter);
    }
    else if (code == LV) {
        load_val(*info, all_registers);
    }
    else {
       exit(EXIT_FAILURE);
    }

    free(info);
    return;
}

/*
* halt_program
* Purpose: A helper function that will run the halt instruction and 
*          perform the necessary memory freeing in order to have a successful 
*          termination of the program
* Parameters: Info info - a struct pointer to the information containing 
*                         the seperated unpacked instruction values.
*             Registers all_registers - a reference to the register manager 
*                       used throughout the program.
*             Memory all_segments - a reference to the segment manager 
*                       used throughout the program.
* Returns: nothing - executes an instruction and ends
* Notes: struct pointer info must be non-NULL
*        all_segments must not be NULL
*        all_registers must be non-NULL 
*/
void halt_program(struct Info *info, Registers all_registers, 
                  Memory all_segments)
{
    assert(info != NULL);
    assert(all_registers != NULL);
    assert(all_segments != NULL);

    free(info);
    free_registers(all_registers);
    free_segments(all_segments);


    exit(EXIT_SUCCESS);
}

/*
* map_a_segment
* Purpose: A helper function that will run the map instruction and 
*          update the memory segment managers and access register information 
*          as needed
* Parameters: Info info - a struct containing the seperated unpacked 
*                         instruction values.
*             Registers all_registers - a reference to the register manager 
*                       used throughout the program.
*             Memory all_segments - a reference to the segment manager 
*                       used throughout the program.
* Returns: nothing - executes an instruction and ends
* Notes: all_segments must not be NULL
*        all_registers must be non-NULL 
*/
void map_a_segment(struct Info info, Registers all_registers, 
                   Memory all_segments)
{
    assert(all_registers != NULL);
    assert(all_segments != NULL);

    uint32_t rB = info.rB;
    uint32_t rC_val = get_register_value(all_registers, info.rC);
    uint32_t mapped_index = map_segment(all_segments, rC_val);
    set_register_value(all_registers, rB, mapped_index);
}


/*
* unmap_a_segment
* Purpose: A helper function that will run the unmap instruction and 
*          update the memory segment managers and access register information
*          as needed
* Parameters: Info info - a struct containing the seperated unpacked 
*                         instruction values.
*             Registers all_registers - a reference to the register manager 
*                       used throughout the program.
*             Memory all_segments - a reference to the segment manager 
*                       used throughout the program.
* Returns: nothing - executes an instruction and ends
* Notes: all_segments must not be NULL
*        all_registers must be non-NULL 
*/
void unmap_a_segment(struct Info info, Registers all_registers, 
                     Memory all_segments)
{
    assert(all_registers != NULL);
    assert(all_segments != NULL);

    uint32_t rC_val = get_register_value(all_registers, info.rC);
    unmap_segment(all_segments, rC_val);
}


/*
* load_program
* Purpose: A helper function that will run the load program instruction and 
*          update the memory segment managers and access register information 
*          as needed, it will call the duplicate function from the memory 
*          manager module. It will also update the program counter as needed.
* Parameters: Info info - a struct containing the seperated unpacked 
*                         instruction values.
*             Registers all_registers - a reference to the register manager 
*                       used throughout the program.
*             Memory all_segments - a reference to the segment manager 
*                       used throughout the program.
*             uint32_t *counter - a reference to the program counter so that 
*                       it can be updated in position to point to a new 
*                       place in the 0th segment (if necessary)
* Returns: nothing - executes an instruction and ends
* Notes: all_segments must not be NULL
*        all_registers must be non-NULL 
*        counter must be non-NULL
*/
void load_program(struct Info info, Registers all_registers, 
                  Memory all_segments, uint32_t *counter)
{
    assert(all_registers != NULL);
    assert(all_segments != NULL);
    assert(counter != NULL);

    uint32_t rB_val = get_register_value(all_registers, info.rB);
    uint32_t rC_val = get_register_value(all_registers, info.rC);
    duplicate_segment(all_segments, rB_val);
    *counter = rC_val;
}


/*
* conditional_move
* Purpose: A helper function that runs the conditional move instruction and 
*          updates the memory segment managers and access register information
*          as needed.
* Parameters: Info info - a struct containing the seperated unpacked 
*                         instruction values.
*             Registers all_registers - a reference to the register manager 
*                       used throughout the program.
* Returns: nothing - executes an instruction and ends
* Notes: all_registers must be non-NULL 
*/
void conditional_move(struct Info info, Registers all_registers)
{
    assert(all_registers != NULL);

    uint32_t rA = info.rA;
    uint32_t rC = info.rC;

    uint32_t rC_val = get_register_value(all_registers, rC);

    if ( rC_val != 0) {
        uint32_t rB = info.rB;

        uint32_t rB_val = get_register_value(all_registers, rB);
        set_register_value(all_registers, rA, rB_val);
    }
}

/*
* arithmetics
* Purpose: A helper function that runs that performs the specified arithmetic 
*          operations with the 3 register values according to the instruction.
*          it holds the defined procedure for each of the 4 similar 
*          arithmetic operations.
* Parameters: Info info - a struct containing the seperated unpacked 
*                         instruction values.
*             Registers all_registers - a reference to the register manager 
*                       used throughout the program.
*             uint32_t arith_code - the opeartion code used to differentiate 
*                       which arithmetic operation should be executed 
*                       with the 3 register values, add, mult, div, or nand
* Returns: nothing - executes an instruction and ends
* Notes: all_registers must be non-NULL 
*/
void arithmetics(struct Info info, Registers all_registers, 
                 uint32_t arith_code)
{
    assert(all_registers != NULL);

    uint32_t rA = info.rA;
    uint32_t rB = info.rB;
    uint32_t rC = info.rC;

    uint32_t rB_val = get_register_value(all_registers, rB);
    uint32_t rC_val = get_register_value(all_registers, rC);

    uint32_t value = 0;

    /*Determine which math operation to perform based on 4bit opcode*/
    if (arith_code == ADD) {
        value = (rB_val + rC_val) % two_pow_32;
    }
    else if (arith_code == MUL) {
        value = (rB_val * rC_val) % two_pow_32;
    }
    else if (arith_code == DIV) {
        value = rB_val / rC_val;
    }
    else if (arith_code == NAND) {
        value = ~(rB_val & rC_val);
    }
    else {
        free_registers(all_registers);
        exit(EXIT_FAILURE);
    }

    set_register_value(all_registers, rA, value);
}


/*
* load_val
* Purpose:  A helper function that runs the load_val instruction and 
*          updates the memory segment managers and access register information
*          as needed.
* Parameters: Info info - a struct containing the seperated unpacked 
*                         instruction values.
*             Registers all_registers - a reference to the register manager 
*                       used throughout the program.
* Returns: nothing - executes an instruction and ends
* Notes: all_registers must be non-NULL 
*        
*/
void load_val(struct Info info, Registers all_registers) 
{
    assert(all_registers != NULL);

    uint32_t rA = info.rA;
    uint32_t val = info.value;
    set_register_value(all_registers, rA, val);
}

/*
* output
* Purpose:  A helper function that runs the output instruction and 
*          updates the memory segment managers and access register information
*          as needed.
* Parameters: Info info - a struct containing the seperated unpacked 
*                         instruction values.
*             Registers all_registers - a reference to the register manager 
*                       used throughout the program.
* Returns: nothing - executes an instruction and ends
* Notes: all_registers must be non-NULL 
*        
*/
void output(struct Info info, Registers all_registers) 
{ 
    assert(all_registers != NULL);

    uint32_t rC = info.rC;
    uint32_t val = get_register_value(all_registers, rC);

    assert(val >= MIN);
    assert(val <= MAX);

    printf("%c", val);
}

/*
* input
* Purpose:  A helper function that runs the input instruction and 
*          updates the memory segment managers and access register information
*          as needed.
* Parameters: Info info - a struct containing the seperated unpacked 
*                         instruction values.
*             Registers all_registers - a reference to the register manager 
*                       used throughout the program.
* Returns: nothing - executes an instruction and ends
* Notes: all_registers must be non-NULL 
*        
*/
void input(struct Info info, Registers all_registers) 
{
    assert(all_registers != NULL);

    
    uint32_t rC = info.rC;

    uint32_t input_value = (uint32_t)fgetc(stdin);
    uint32_t all_ones = ~0;
    
    /*Check if input value is EOF...aka: -1*/
    if (input_value == all_ones) {
        set_register_value(all_registers, rC, all_ones);
        return;
    }
 
    /* Check if the input value is in bounds */
    assert(input_value >= MIN);
    assert(input_value <= MAX);

    /* $r[C] gets loaded with input value */
    set_register_value(all_registers, rC, input_value);
}

/*
* segmented_load
* Purpose:  A helper function that runs the segmented load instruction and 
*          updates the memory segment managers and access register information
*          as needed.
* Parameters: Info info - a struct containing the seperated unpacked 
*                         instruction values.
*             Registers all_registers - a reference to the register manager 
*                       used throughout the program.
*             Memory all_segments - a reference to the memory manager used 
*                       throughout the program
* Returns: nothing - executes an instruction and ends
* Notes: all_registers must be non-NULL 
*        
*/
void segmented_load(struct Info info, Registers all_registers, 
                    Memory all_segments)
{
    assert(all_registers != NULL);
    assert(all_segments != NULL);

    /* Establishes which register indexes are being used */
    uint32_t rA = info.rA;
    uint32_t rB = info.rB;
    uint32_t rC = info.rC;

     /* Accesses the values at the register indexes*/
    uint32_t rB_val = get_register_value(all_registers, rB);
    uint32_t rC_val = get_register_value(all_registers, rC);

    uint32_t val = get_word(all_segments, rB_val, rC_val);

    set_register_value(all_registers, rA, val);
}

/*
* segmented_store
* Purpose:  A helper function that runs the segmented store instruction and 
*          updates the memory segment managers and access register information
*          as needed.
* Parameters: Info info - a struct containing the seperated unpacked 
*                         instruction values.
*             Registers all_registers - a reference to the register manager 
*                       used throughout the program.
*             Memory all_segments - a reference to the memory manager used 
*                       throughout the program
* Returns: nothing - executes an instruction and ends
* Notes: all_registers must be non-NULL 
*        
*/
void segmented_store(struct Info info, Registers all_registers, 
                     Memory all_segments)
{
    assert(all_registers != NULL);
    assert(all_segments != NULL);

    /* Establishes which register indexes are being used */
    uint32_t rA = info.rA;
    uint32_t rB = info.rB;
    uint32_t rC = info.rC;

    /* Accesses the values at the register indexes*/
    uint32_t rA_val = get_register_value(all_registers, rA);
    uint32_t rB_val = get_register_value(all_registers, rB);
    uint32_t rC_val = get_register_value(all_registers, rC);

    set_word(all_segments, rA_val, rB_val, rC_val);
}
