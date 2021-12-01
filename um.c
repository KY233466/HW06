
/**************************************************************
 *                     um.c
 * 
 *     Assignment: Homework 6 - Universal Machine Program
 *     Authors: Katie Yang (zyang11) and Pamela Melgar (pmelga01)
 *     Date: November 24, 2021
 *
 *     Purpose: This C file will hold the main driver for our Universal
 *              MachineProgram (HW6). 
 *     
 *     Success Output:
 *              
 *     Failure output:
 *              1. 
 *              2. 
 *                  
 **************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "excution.h"

int main(int argc, char *argv[])
{
    /*Progam can only run with two arguments [execute] and [machinecode_file]*/
    assert(argc > 1 && argc < 3);
    
    if (argc != 2) {
        fprintf(stderr, "Usage: ./um [filename] < [input] > [output]\n");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(argv[1], "r");
    assert(fp != NULL);

    excute(fp);

    return EXIT_FAILURE;
}