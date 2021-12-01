/**************************************************************
 *                     read_file.c
 * 
 *     Assignment: Homework 6 - Universal Machine Program
 *     Authors: Katie Yang (zyang11) and Pamela Melgar (pmelga01)
 *     Date: November 24, 2021
 *
 *     Purpose: This file is the implementation of our read_file module 
 *              that's main job is to read in the machine instructions, 
 *              to then be passed into a memory segment in the right formatting
 *     
 *     Success Output:
 *              
 *     Failure output:
 *              1. 
 *              2. 
 *                  
 **************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "read_file.h"
#include "bitpack.h"
#include "stdint.h"
#include "assert.h"

static const int BYTESIZE = 8;

/*
* readFile
* Purpose: To read in the FILE read in by the program, parse out the 
*          uint32_t word instructions, and populate the 0th memory segment
*          of the memory segments with the parsed word instructions.
*          This function ensures the machine instructions can be 
*          interpreted correctly as uint32_ts (4 bytes) for the 0th segment
* Parameters: struct Memory *memory - a struct pointer to an instance of 
*                  the memory manager, passed in as a reference so any 
*                  changes to the 0th segment are preserved after the end 
*                  of the function call
*             FILE *input - a FILE pointer to the beginning of the opened 
*                   input file containing 4-byte word instructions to be 
*                   interpreted by this function and stored into the 0th
*                   segment
* Returns: nothing, changes the 0th segment that has been passed through 
*          the memory manager by reference
* Notes: the pointer to the struct Memory cannot be NULL, the FILE *input 
*        cannot be NULL
*/
void readFile(FILE *input, Memory memory)
{
    /* check for valid input */
    assert(input != NULL);
    
    uint32_t byte = 0; 
    uint32_t word = 0;
    int c;
    int counter = 0;

    /*Keep reading the file and parsing through "words" until EOF*/
    /*Populate the sequence every time we create a new word*/
    while ((c = fgetc(input)) != EOF) {
        word = Bitpack_newu(word, 8, 24, c);
        counter++;
        for (int lsb = 2 * BYTESIZE; lsb >= 0; lsb = lsb - BYTESIZE) {
            byte = fgetc(input);
            word = Bitpack_newu(word, BYTESIZE, lsb, byte);
        }
        
        uint32_t *word_ptr = malloc(sizeof(uint32_t));
        *word_ptr = word;
        
        /*Populate sequence*/
        add_to_seg0(memory, word_ptr);
    }

   fclose(input);
}