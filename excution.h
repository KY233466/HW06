/**************************************************************
 *                     execution.h
 * 
 *     Assignment: Homework 6 - Universal Machine Program
 *     Authors: Katie Yang (zyang11) and Pamela Melgar (pmelga01)
 *     Date: November 24, 2021
 *
 *     Purpose: This is the header file for our execution module, that which 
 *              runs the program and passes around components as needed
 *     
 *     Success Output:
 *              exit_sucess
 *              
 *     Failure output:
 *              exit_faliure 
 *                  
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>

#ifndef EXECUTION_H
#define EXECUTION_H

/*
* execute
* Purpose: To pass in the opened file pointer to the beginning of the 
*          input file containing UM word instructions, This module will 
*          run the bulk of the program, including instruction executions
* Input: a FILE * input pointer to the beginning of the file containing 
*               UM word instsructions in order to run the program
* Expected Output: nothihng, the program gets executed and is expected to 
*                  run successfully
* Note: 
*/
void excute(FILE *input);

#endif 