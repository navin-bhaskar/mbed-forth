/* Reconfigurable computing system
 * Registration number: NXP3878 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * \file        CoreForth.h
 * \brief       This file contains defs and fumction prototypes required by
 *              CoreForth.c
 *
 */

#ifndef __LINKED_LIST_H
#define __LINKED_LIST_H


#define NODE_ADDING_ERROR     2             /**< Error code to return when there is no more memory to create words */
#define NODE_ADDING_SUCCESS   0             /**< Return code if node (dictionory entry) was created successfully */

#define FORTH_NAMEMAX         16              /**< Maximum FORTH word name length */
#define FORTH_WORD_FOUND      1              /**< Condition code returned if forth word found */
#define FORTH_WORD_NOT_FOUND  2              /**< Condition code returned if forth word not found */


#define SIZE                  150             /**< General purpouse size define */

#define bool    char                         /**< Boolean type */
#define TRUE    1                            /**< True condition */
#define FALSE   0                            /**< False Condition */

#define FORTH_WORD_DEL      2                /**< Code for indicating word was deleted successfully */

#define _BV(bit)(1 << bit)                   /**< Sets a bit position */
#define WORD_INBUILT         1               /**< Inbuilt word */
#define FORTH_WORD_INBUILT _BV(WORD_INBUILT) /**< if \a WORD_INBUILT is set then the word is an inbuilt function */
#define WORD_IMED            2               /**< Immediate word bit position */
#define FORTH_WORD_IMED    _BV(WORD_IMED)    /**< Flag for indicating immediate word, used in compiled mode */
#define WORD_USER           3                /**< Bit position for user defined flag */
#define FORTH_WORD_USER    _BV(WORD_USER)    /**< Flag for indicating user defined word */
#define COMPILE_ONLY        4                /**< Bit position for \a FORTH_COMPILE_ONLY */
#define FORTH_COMPILE_ONLY _BV(COMPILE_ONLY) /**< Function can be executed only in compile mode */
#define VAR                    5                /**< Bit position for \a FORTH_WORD_VAR */
#define FORTH_WORD_VAR     _BV(VAR) /**< Word has a variable to which memory has been allocated */


#define END_WORD            -55               /**< YOU CANNOT USE THIS CONSTANT IN FORTH PROGRAM. IF YOU USE IT FORTH WILL CRASH */


/// can hold address of a node. struct node* is 'typedef'ed as NodePtr
typedef struct Node* NodePtr ;

/// func_ptr holds pointer to functions of type void func(void)
typedef void(*func_ptr)(void);

/**
 * \struct      Node
 * \brief       Structure for handling FORTH dictionary.
 *
 *              This structure defines all that is necessary to have a linked list
 *              for creating FORTH dictionary.
 */

struct Node {
    char WrdName[FORTH_NAMEMAX];                    /**< Holds the word name */
    int flag;                                        /**< To hold various conditions such as FORTH_WORD, FORTH_INBUILT etc */
    int *code;                                       /**< Array to hold the address of various code word */
    NodePtr next;                                    /**< To point to next entry in the dictionary */
    int WrdLen;                                     /**< length of word name */
    func_ptr func;                                  /**< Function pointer to inbuilt function */
};



//void DisplayDic(void);

//int DelDicEntry(char *name);
//void DelLatestEntries(int no);

struct Buffer {
    char buff[500];
    char flag;
};

int AddDicEntry(char* name, int ForthFlags, func_ptr func, int* CodeList, int len);

#endif



