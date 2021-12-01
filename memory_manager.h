/**************************************************************
 *                     memory_manager.h
 * 
 *     Assignment: Homework 6 - Universal Machine Program
 *     Authors: Katie Yang (zyang11) and Pamela Melgar (pmelga01)
 *     Date: November 24, 2021
 * 
 *     interface for memory_manager
 *
 *     Purpose: 
 *     
 *     Success Output:
 *              
 *     Failure output:
 *              
 *                  
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

/*A struct pointer to create a hidden instance of the Memory manager*/
typedef struct Memory *Memory;

/*
* initialize_memory
* Purpose: To create an instance of a Memory_manager struct that will 
*           store the segments used throughout the program (program 
*           initializes with segment 0)
* Input: none
* Expected Output: an instance of a memory_manager with one memory segment
* Note: 
*/
Memory initialize_memory();


/*
* memorylength
* Purpose: To retrieve the number of segments in the entire memory_manager 
* Input: an instance of the memory manager
* Expected Output: an integer representing how many total segments have
*                   been initialized in the memory manager
* Note: memory must not be NULL
*/
uint32_t memorylength(Memory memory);


/*
* segmentlength
* Purpose: To retrieve the number of words inside a sepcified segment 
*          within the memory segments. 
* Input: an instance of the memory manager and an index integer to specify 
*        which segment length to retrieve
* Expected Output: an integer representing the number of words contained 
*                  in a given segment of memory
* Note: the index must be in bounds, and memory cannot be NULL
*/
uint32_t segmentlength(Memory memory, uint32_t segment_index);


/*
* add_to_seg0
* Purpose: To populate the 0th segment that will contain the UM program
* Input: an intance of the memory manager and a reference to a 
*        single uint32_t word 
*        instruction
* Expected Output: none
* Note: This function populates the 0th segment one word at a time, and 
*       should be called iteratively.
*       memory and word cannot be NULL
*/
void add_to_seg0(Memory memory, uint32_t *word);


/*
* get_word
* Purpose: To retrieve a word form a desired segment in memory
* Input: an instance of the memory manager, an integer representing 
*        the segment to retrieve the word from, and an integer 
*        representing which word form that segment to return.
* Expected Output: A uint32_t word instruction from the desired location
* Note: memory cannot be NULL, segment index and word in segmnet must be 
*       in bounds
*/
uint32_t get_word(Memory memory, uint32_t segment_index, 
                  uint32_t word_in_segment);

/*
* set_word
* Purpose: To set/change the value of a word in a given segment of memory
* Input: An instance of the memory manager contaiining all segments, 
*        an integer indicating which segment to find the word, 
*        an integer indicating which word within a given segment to change 
*        a uint32_t word representing the value that will replace the word 
*        at that index
* Expected Output: none
* Note: memory must not be NULL, segment_index and word_index must be in 
*       bounds 
*/
void set_word(Memory memory, uint32_t segment_index, uint32_t word_index, 
              uint32_t word);


/*
* map_segment
* Purpose: To create a new segment within the memory manager
* Input: An instance of the memory manager and an integer representing the 
*        desired number of words that the new segment will hold
* Expected Output: none
* Note: memory must not be NULL and num_words must be positive and less than 
*       2^32
*/
uint32_t map_segment(Memory memory, uint32_t num_words);


/*
* unmap_segment
* Purpose: To kill a segment from the memory manager segments, words 
*          that were previously in this segment could no longer be accessed
* Input: an instance of the memory manager, and an integer representing 
*        which segment of memory to kill
* Expected Output: none
* Note:  memory must not be NULL, and segment index must be in bounds
*/
void unmap_segment(Memory memory, uint32_t segment_index);


/*
* duplicate_segment
* Purpose: To replace segment 0 with another memory segment at the 
*          specified index of memory
* Input: an instance of the memory manager and an integer indicating which 
*        segment to replace segment 0 with.
* Expected Output: none
* Note: if the segment_to_copy is 0, nothing will happen
*       memory must not be NULL, and segment to copy index must be 
*       in bounds
*/
void duplicate_segment(Memory memory, uint32_t segment_to_copy);


/*
* free_segments
* Purpose: To free all the allocated memory taken up by the memory segments
*          and words within those segments.
* Input: an instance of the memory manager
* Expected Output: none
* Note: 
*/
void free_segments(Memory memory);

#endif 