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
 * \file        CoreForth.c
 * \brief       This file contains all the functions related with linked list creation
 *              and handling required for FORTH implementation
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CoreForth.h"


NodePtr LATEST;                       /**< Always holds address of the latest entry to the dictionary */
NodePtr FIRST;                        /**< Always holds the address of the first entry in the dictionary */


/* In terms of Linked list standard defination Latest is the head, first is the tail */

/**
 *
 * \fn        AddDicEntry(char* name, int ForthFlags, func_ptr func, int* CodeList, int len)
 * \brief     Adds a Word to dictionary
 *
 *            This function adds a word from front into the dictionary
 *
 * \param[in] name          name of the word
 * \param[in] ForthFlags    flags to indicate various attributes of a word
 * \param[in] func          A function pointer to attach a function with word.
 *                          If the flag ForthInbuilt is false, NULL pointer is to be passed
 * \param[in] CodeList      If the flag ForthInbuilt is false, then this variable points to a list of addresses corresponding to words
 *                          defined within the current word to be created in the dictionary.
 *                          Should be NULL for ForthInbuilt == TRUE
 * \param[in] len           length of the CodeList i.e, the number of addresses the CodeList pointer has
 *                          this can be any number if CodeList is not used
 *
 * \return                  NODE_ADDING_SUCCESS if the code word was successfully added \n
 *                          NODE_ADDING_ERROR   if the code word was not added to the dictionary
 *
 */

int AddDicEntry(char* name, int ForthFlags, func_ptr func, int* CodeList, int len) {
    NodePtr mid;                    // for our node processing purpouses
    int i;

    mid = (NodePtr)malloc(sizeof(struct Node));

    if (mid == NULL) {
#ifdef DEBUG
        printf ("\nCould not allocate memory \n");
#endif
        return NODE_ADDING_ERROR;
    }

    strcpy(mid->WrdName, name);
    mid->WrdLen = strlen(name);                 // store name and length

    if (LATEST == NULL) {                   // first entry ever
        LATEST = mid;
        mid->next = NULL;
        FIRST = mid;

    } else {
        mid->next = LATEST;
        LATEST = mid;
    }

    /* we have added the nodes as required now is the time to do set up the environment for each word */

    mid->flag = ForthFlags;
    // first let us add function handler
    if (mid->flag & FORTH_WORD_INBUILT) {
        mid->func = func;
    } else {
        if (len == 0) {
            mid->code[0] = 0;
            mid->code[1] = END_WORD;
            return NODE_ADDING_SUCCESS;             // if list is empty create a list and leave (empty entry)
        }

        // else
        mid->code = (int*)malloc((len+1)*sizeof(int));
        for (i=0; i<len; i++) {
            mid->code[i] = CodeList[i];              // store the addresses of words that this word is composed of
        }
        mid->code[i] = END_WORD;                             // to indicate end of code word
    }

    return NODE_ADDING_SUCCESS;

}


/**
 * \fn             DisplayDic(void)
 * \brief          Displays attributes of entire dictionary
 *
 *                 This function displays attributes of entires in the dictionary starting from the latest defination
 * \return         void
 */

void DisplayDic(void) {
    NodePtr temp;
    temp = LATEST;

    while (temp != NULL) {
        printf ("Name     : %s\n", temp->WrdName);
        printf ("length   : %d\n", temp->WrdLen);
        temp = temp->next;
    }
}

/**
 * \fn              Find(char* name, int* addr)
 * \brief           Finds a dictionary entry with given name
 *
 *                  This function serches for a word with given name if it finds the word then returns FORTH_WORD_FOUND
 *                  else returns FOTH_WORD_NOT_FOUND
 *
 * \param[in]       name name of the word
 * \param[out]      addr address at which the entry was found
 * \return          FORTH_WORD_FOUND  If the word was found  \n
 *                  FORTH_WORD_NOT_FOUN if the word was not found in the dictnory
 *
 */

int Find(char* name, int* addr) {
    int len = strlen(name);
    NodePtr temp = LATEST;                     // start with the latest entry


    while (temp != NULL) {

        if (temp->WrdLen == len) {            // name matching speed up thingy
            if (strcmp(temp->WrdName, name) == 0) {
                *addr = (int)temp;
                return FORTH_WORD_FOUND;
            }
        }

        temp = temp->next;
    }

    addr = NULL;
    return FORTH_WORD_NOT_FOUND;
}

/**
 *
 * \fn             DelDicEntry(char* name)
 * \brief          This function deletes a dictionary entry.
 *                 This function searches for a dictionary entry with given name. If it finds one it deletes the entry
 *                 and returns FORTH_WORD_DEL or else FORTH_WORD_NOT_FOUND
 * \param[in]      name name of the dictionary entry to be deleted
 * \return         FORTH_WORD_DEL if word was found and deleted \n
 *                 FORTH_WORD_NOT_FOUND if word could not be found
 *
 */

int DelDicEntry(char *name) {
    int len = strlen(name), *x;
    NodePtr temp, temp1;
    temp = LATEST;           // start with head, find the most recent entry with given name


    if (strcmp(temp->WrdName, name) == 0 || strcmp(name, "LATEST")==0) {
        LATEST = temp->next;
        if (temp->flag & FORTH_WORD_VAR) {
            x = (int*) temp->code[1];
            free(x);                            // free memory allocated for the variable
        }
        free(temp->code);
        free(temp);
        return FORTH_WORD_DEL;
    }

    while (temp != NULL) {
        temp1 = temp->next;
        if (temp1 != NULL) {
            if (temp1->WrdLen == len) {
                if (strcmp(temp1->WrdName, name) == 0) {
                    break;                 // found the word
                }
            }
            temp = temp->next;
        }
    }

    if (temp != NULL) {
        // do these things only if word was found
        temp1 = temp->next;
        temp->next = temp->next->next;
        if (temp1->flag & FORTH_WORD_VAR) {
            x = (int*)  temp1->code[1];
            free(x);
        }
        free(temp1->code);                 // free memory allocated for the code
        free(temp1);
        return FORTH_WORD_DEL;
    }

    return FORTH_WORD_NOT_FOUND;
}

/**
 *    \fn                DelLatestEntries(int no)
 *    \brief             This function deletes 'no' number of entries starting from latest
 *    \param[in]         no  number of entires to be deleted
 *    \return            void
 */

void DelLatestEntries(int no) {
    int i, *x;
    NodePtr temp ,temp1;
    temp = LATEST;
    if (0 == no) {
        return ;
    }

    for (i=0; i<no; i++) {
        LATEST = temp->next;
        if (temp->flag & FORTH_WORD_VAR) {
            x = (int*) temp->code[1];
            free(x);                            // free memory allocated for the variable
        }
        temp1 = temp->next;
        free(temp->code);
        free(temp);
        temp = temp1;
    }
}







