/**************************************************************
 *                     memory_manager.c
 * 
 *     Assignment: Homework 6 - Universal Machine Program
 *     Authors: Katie Yang (zyang11) and Pamela Melgar (pmelga01)
 *     Date: November 24, 2021
 * 
 *     implementation for memory_manager.h
 *
 *     Purpose: Provides the public functions in order to manage memory in 
 *              the UM emulator. Some of the public functions handle the 
 *              heavy-work for some instructions such as load program, or 
 *              mapping/unmapping.
 *     
 *     Success Output:
 *              Varies based on the function.
 * 
 *     Failure output:
 *              Checked run-time errors
 * 
 *     Note:
 *          There are 8 registers in total.
 *                  
 **************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "memory_manager.h"
#include "seq.h"
#include "bitpack.h"
#include "assert.h"

#define BYTESIZE 8

/*
* struct Memory
* Purpose: To manage the segments used throughout the program and keep
*          secrets from the client of how these segments are represented 
*          under-the-hood.
* Members: Seq_T segments - the data structure to hold the memory segments
*                   used throughout the program. Each segment Sequence will 
*                   contain a Sequence of word instructions
*          Seq_T map_queue - the data structure to keep track of which 
*                   segments within the memory manager have been unmapped, 
*                   "or killed". This keeping track is important so that 
*                   when a new segment is desired, the queue will quickly  
*                   retrieve the oldest unmapped segment index to 
*                   revive/recycle whenever possible.
* Notes: The client cannot see this struct Memory implmentation, and will 
*        only have access to a pointer to this struct
*/
    struct Memory
{
    Seq_T segments;
    Seq_T map_queue;
};

/*
* initialize_memory
* Purpose: To create an instance of a Memory_manager struct pointer that will 
*           store a reference to the segments used throughout the program 
*           (program initializes with segment 0). This function also 
*           allocates the necessary memory/space for this struct before 
*           returning to the caller
* Parameters: none
* Returns: a struct pointer to the intialized memory manager containing 
*          an empty Segment 0 ready to be populated with program instructions
* Notes: Will fail if memory cannot be allocated
*/
 struct Memory *initialize_memory()
 {
     struct Memory *memory = malloc(sizeof(struct Memory));
     assert(memory != NULL);

     memory->segments = Seq_new(30);
     memory->map_queue = Seq_new(30);
     Seq_T segment0 = Seq_new(10);

     Seq_addhi(memory->segments, segment0);
     /*added segment 0*/ 
     return memory;
 }

/*
 * memorylength
 * Purpose: To return the length of the memory manager, that is, the number 
 *           of created segments within the memory manager, this number 
 *           will also include unmapped segments
 * Parameters: struct Memory *memory - a struct pointer to an instance of 
 *                  the memory manager
 * Returns: an integer repreesnting the number of segments within the entire 
 *          memory manager
 * Notes: the pointer to the struct cannot be NULL
 */
uint32_t memorylength(struct Memory *memory){
    assert(memory != NULL);

    return Seq_length(memory->segments);
}

/*
* segmentlength
* Purpose: To retrieve the number of words inside a sepcified segment 
*          within the memory segments.
* Parameters: struct Memory *memory - a struct pointer to an instance of 
*             an initalized memory manager. uint32_t segment_index - an 
*             integer representing the desired segment of memory to retrieve 
*             the size from, that is, the number of uint32_t words within it
* Returns: an integer representing the number of words contained 
*          in a given segment of memory
* Notes: the pointer to the struct cannot be NULL
*/
uint32_t segmentlength(struct Memory *memory, uint32_t segment_index){
    assert(memory != NULL);

    return Seq_length(Seq_get(memory->segments, segment_index));
}


/*
* add_to_seg0
* Purpose: To populate the 0th segment that will contain the uint32_t 
*          word instructions of the UM program. This function will 
*          retrieve segment 0 from the memory_manager and append 
*          word instructions onto it as they are being dynamically allocated 
*          in the read module
* Parameters: struct Memory *memory - a struct pointer to an instance of 
*                   an initalized memory manager. 
*             uint32_t *word - a pointer to a uint32_t word instruction 
*                   that has been read an dynamically allocated in the 
*                   read file module
* Returns: nothing - sets words within segment0
* Notes: the pointer to the struct cannot be NULL
*        the pointer to the word cannot be NULL
*/
void add_to_seg0(struct Memory *memory, uint32_t *word)
{
    assert(memory != NULL);

    Seq_T find_segment = (Seq_T)(Seq_get(memory->segments, 0));
    Seq_addhi(find_segment, word);
}


/*
* get_word
* Purpose: A getter function to retrieve a uint32_t word instruction
*          from a desired segment in memory according to a specified segment
*          number and word index number within that segment
* Parameters: struct Memory *memory - a struct pointer to an instance of 
*                   an initalized memory manager. 
*           uint32_t segment_index - an integer indiating which segment of
*                   memory the desired word is contained.
*           uint32_t word_in_segment - an integer indicating which index within
*                   the desired segment that the actual desired word is 
*                   located
* Returns: a uint32_t word instruction value retrieved from the given indices
* Notes: the pointer to the struct cannot be NULL
*        Both the segment and the word index should not be out of bounds
*/
uint32_t get_word(struct Memory *memory, uint32_t segment_index, 
                  uint32_t word_in_segment)
{
    assert(memory != NULL);

    Seq_T find_segment = (Seq_T)(Seq_get(memory->segments, segment_index));
    assert(find_segment != NULL);

    uint32_t *find_word = (uint32_t *)(Seq_get(find_segment, word_in_segment));

    return *find_word;
}


/*
* set_word
* Purpose: To set/change the value of a word in a given segment of memory, 
*          and at a specified index within the segment
* Input: struct Memory *memory - a struct pointer to an instance of 
*                   an initalized memory manager. 
*        nuint32_tt segment_index - an integer indiating which segment of
*                   memory the desired word is contained.
*        uint32_t word_index - an integer indicating where within the desired 
*                   segment to change the uint32_t word value
*       
* Expected Output: none -- setter function
* Note: memory cannot be NULL, segment index and word in segmnet must be 
*       in bounds
*/
void set_word(struct Memory *memory, uint32_t segment_index, 
              uint32_t word_index, uint32_t word)
{
    assert(memory != NULL);

    /*failure mode if out of bounds*/
    Seq_T find_segment = Seq_get(memory->segments, segment_index);

    assert(find_segment != NULL);/*Failure mode if refer to unmapped*/ 

    uint32_t *word_ptr = malloc(sizeof(uint32_t));
    assert(word_ptr != NULL);
    *word_ptr = word;

    /*failure mode if out of bounds*/
    uint32_t *old_word_ptr = Seq_put(find_segment, word_index, word_ptr);
    free(old_word_ptr);
}

/*
* map_segment
* Purpose: To create a new segment within the memory manager that will
*          contain a size that is indicated in the parameter, where 
*          "num_words"-many words all initialized to 0 will be stored. This
*          function will either recycle the oldest segment that has been 
*          unmapped, or append a brand new segment onto the end of the 
*          Sequence of memory segments, only if there are no unmapped
*          segments to re-use. 
* Input: struct Memory *memory - a struct pointer to an instance of 
*                   an initalized memory manager. 
*        uint32_t num_words - an integer indiating the size of the newly 
*                        created segment. This new segment will contain 
*                        an indicated number of uint32_t word all 
*                        initialized to 0.
* Expected Output: none
* Note: memory cannot be NULL, segment index and word in segmnet must be 
*       in bounds
*/
uint32_t map_segment(struct Memory *memory, uint32_t num_words)
{
    assert(memory != NULL);
    assert(memory->segments != NULL);
    assert(memory->map_queue != NULL);

    Seq_T new_segment = Seq_new(num_words);
    /*Initialize*/
    for (uint32_t i = 0; i < num_words; i++) {
        uint32_t *word_ptr = malloc(sizeof(uint32_t));
        assert(word_ptr != NULL);

        *word_ptr = 0;
        Seq_addhi(new_segment, word_ptr);
    }

    if (Seq_length(memory->map_queue) != 0) {
        uint32_t *seq_index = (uint32_t *)Seq_remlo(memory->map_queue);
        Seq_put(memory->segments, *seq_index, new_segment);
        
        uint32_t segment_index = *seq_index;
        free(seq_index);
        return segment_index;
    }
    else {
        uint32_t length = (uint32_t)Seq_length(memory->segments);
        Seq_addhi(memory->segments, new_segment);
        return length;
    }
}


/*
* unmap_segment
* Purpose: To kill a segment from the memory manager sequence of 
*          segments. In other words, the uint32_t words that were previously 
*          were contained in this segment could no longer be accessed once 
*          that segment is unmapped. This function ensures that the 
*          index of the unmapped segment gets stored onto a queue 
*          for purposes of recycling when the caller decides to map
*          a brand new segment
* Input: struct Memory *memory - a struct pointer to an instance of 
*                   an initalized memory manager. 
*        uint32_t segment_index - an integer indiating the desired segment 
*                            within the sequence of memory segments to 
*                            kill/unmap. 
* Expected Output: none
* Note:  memory must not be NULL, and segment index must be in bounds
*/
void unmap_segment(struct Memory *memory, uint32_t segment_index)
{
    assert(memory != NULL);
    assert(memory->segments != NULL);
    assert(memory->map_queue != NULL);
    /* can't un-map segment 0 */
    assert(segment_index > 0);

    Seq_T seg_to_unmap = (Seq_T)(Seq_get(memory->segments, segment_index));
    /* can't un-map a segment that isn't mapped */
    assert(seg_to_unmap != NULL);

    for (int j = 0; j < Seq_length(seg_to_unmap); j++) {
        uint32_t *word = (uint32_t *)(Seq_get(seg_to_unmap, j));
        free(word);
    }

    Seq_free(&seg_to_unmap);

    Seq_put(memory->segments, segment_index, NULL);

    uint32_t *num = malloc(sizeof(uint32_t));
    *num = segment_index;
    Seq_addhi(memory->map_queue, num);
}


/*
* duplicate_segment
* Purpose: To handle instruction 12 (load program) functionality that 
*          deals directly with rearranging the segments within the memory 
*          manager. This function will identify which segment is to be 
*          duplicated to replace segment 0.It also ensures that if the 
*          indicated segment to duplicate is segment 0 itself, that it 
*          will not spend extra time deep-copying word instructions
* Input: struct Memory *memory - a struct pointer to an instance of 
*                   an initalized memory manager. 
*        uint32_t segment_index - an integer indiating the desired segment 
*                            within the sequence of memory segments to 
*                            kill/unmap. 
* Expected Output: none
* Note:  memory must not be NULL, and segment_to_copy index must be in bounds
*/
void duplicate_segment(struct Memory *memory, uint32_t segment_to_copy)
{
    assert(memory != NULL);
    assert(memory->segments != NULL);
    assert(memory->map_queue != NULL);

    if (segment_to_copy != 0) {

        Seq_T seg_0 = (Seq_T)(Seq_get(memory->segments, 0));

        /*free all words in segment 0*/
        for (int j = 0; j < Seq_length(seg_0); j++) {
            uint32_t *word = (uint32_t *)(Seq_get(seg_0, j));
            free(word);
        }

        /*Free memory of old segment0*/
        Seq_free(&seg_0);

        /*hard copy - duplicate segment to create new segment 0*/
        Seq_T target = (Seq_T)Seq_get(memory->segments, segment_to_copy);
        assert(target != NULL); /*Check if copy index has been unmapped*/

        uint32_t seg_length = Seq_length(target);

        Seq_T duplicate = Seq_new(30);

        /*Willl copy every single word onto the duplicate segment*/
        for (uint32_t i = 0; i < seg_length; i++) {
            uint32_t *dup_word = malloc(sizeof(uint32_t));
            uint32_t *word = Seq_get(target, i);
            *dup_word = *word;
            
            Seq_addhi(duplicate, dup_word);
        }

        /*replace segment0 with the duplicate*/
        Seq_put(memory->segments, 0, duplicate);

    } else {
        /*don't replace segment0 with itself --- do nothing*/
        return;
    }
}

/*
* free_segments
* Purpose: To free all the allocated memory taken up by the memory segments
*          and words within those segments. To allow the client to have 
*          quick-access to avoiding memory leaks if the program needs to
*          abort via a checked-runtime-error, if the program counter reaches 
*          the end of segment 0, or a halt-instruction is encountered.
* Input: struct Memory *memory - a struct pointer to an instance of 
*                   an initalized memory manager containing all the 
*                   references to dynamically allocated memory
* Expected Output: none
* Note: 
*/
void free_segments(struct Memory *memory)
{
    assert(memory != NULL);


    uint32_t num_sequences = Seq_length(memory->segments);
    /*Free all words in each segment of memory*/
    for (uint32_t i = 0; i < num_sequences; i++)
    {
        Seq_T find_segment = (Seq_T)(Seq_get(memory->segments, i));
    
        /*Only free words in segments that are non-null*/
        if (find_segment != NULL) {
            uint32_t seg_length = Seq_length(find_segment);

            for (uint32_t j = 0; j < seg_length; j++) {
                // printf("    processing %d\n", j);
                uint32_t *word = (uint32_t *)(Seq_get(find_segment, j));
                free(word);
            }
            Seq_free(&find_segment);
        }
    }

    /* free map_queue Sequence that kept track of unmapped stuff*/
    uint32_t queue_length = Seq_length(memory->map_queue);
    
    /*free all the indexes words */
    for (uint32_t i = 0; i < queue_length; i++) {
        uint32_t *index = (uint32_t*)(Seq_get(memory->map_queue, i));
        free(index);
    }

    Seq_free(&(memory->segments));
    Seq_free(&(memory->map_queue));
    
    free(memory);
}