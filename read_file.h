/**************************************************************
 *                     read_file.h
 * 
 *     Assignment: Homework 6 - Universal Machine Program
 *     Authors: Katie Yang (zyang11) and Pamela Melgar (pmelga01)
 *     Date: November 24, 2021
 *
 *     Purpose: This file is the interface for our read_file module, it 
 *              provides the public functions for the client to use to
 *              enable reading functionality
 *     
 *     Success Output: 
 *              
 *        
 *     Failure output:
 *              
 *                  
 **************************************************************/

#ifndef READ_FILE_H
#define READ_FILE_H

#include <stdio.h>
#include "memory_manager.h"


/*
* readFile
* Purpose: To read in the FILE read in by the program, parse out the 
*          uint32_t word instructions, and populate the 0th memory segment
*          with the parsed word instructions.
* Input: A FILE *input pointing to the beginning of the File containing 
*        machine instructions, passed in after being opened successfully, 
*        A struct pointer to the Memory manager instance to initialize 
*        the memory segment based on the information from the FILE
* Expected Output: none -- parses uint32_t words and completes
* Note: memory and *input must not be NULL
*/
void readFile(FILE *input, Memory memory);


#endif 
