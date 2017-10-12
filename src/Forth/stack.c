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
 *
 * \file     stack.c
 * \brief    This file has functions that handle all the stack related operations
 *
 *           As you know, the FORTH langauge has two stacks. One is data stack and the other is called return stack.
 *           You can set the size of both the stacks. To set the size of data stack, change \a STACK_DAT_SIZE. To change
 *           the size of return stack use \a STACK_RET_SIZE.
 *
 */

#include <stdio.h>
#include <string.h>
#include "stack.h"
#include "CoreForth.h"

int DatStack[STACK_DAT_SIZE];                           /**< Actual data stack */
int RetStack[STACK_RET_SIZE];                           /**< Return stack */

int DatStackTop;                                       /**< Top of stack pointer for data stack */
int RetStackTop;                                       /**< Always points to top of return stack */

/**
 *
 * \fn         PushDs(int dat, int* err_code)
 * \brief      Pushes a quantum of data into stack
 *
 *             This function pushes dat into data stack. It uses \a DatStackTop for push operstion.
 *
 * \param[in]  dat      data to be pushed into the stack
 * \param[out] err_code logs in error. \a STACK_ERR_FULL if stack is full \a STACK_ERR_SUCCESS if push operatin was completed
 *                      successfully
 *
 * \return      STACK_ERR_FULL if stack was full or else STACK_ERR_SUCCESS.
 *
 */

int PushDs (int dat, int* err_code) {
    int ret;

    /*
     * I am using err_code because if i choose to return error conditions put in ret variable i cannot push -1 if
     *  STACK_ERR_FULL was defined as -1
     */

    if (DatStackTop == STACK_DAT_SIZE-1) {
        ret = *err_code = STACK_ERR_FULL;
        printf ("Stack full\n");
    } else {
        DatStack[DatStackTop++] = dat;
        ret = *err_code = STACK_ERR_SUCCESS;
    }

    return ret;
}

/**
 *
 * \fn         PopDs(int *err_code)
 * \brief      Pops a word from the data stack
 *
 *             Pop function is stack destructive call that destroys the top of stack and returns the value at top of stack
 *
 * \param[out] err_code  logs in error. \a STACK_ERR_EMPTY if stack is empty pr else \a STACK_ERR_SUCCESS
 *
 * \returns    Data that was on tos or -1 if stack was empty
 *
 */

int PopDs(int *err_code) {
    int ret = STACK_ERR_EMPTY;

    if (DatStackTop == 0) {
        *err_code = STACK_ERR_EMPTY;
        printf ("Stack under flow \n");
    } else {
        *err_code = STACK_ERR_SUCCESS;
        DatStackTop--;
        ret = DatStack[DatStackTop];
    }

    return ret;
}


/**
 *
 * \fn        DispDs(void)
 * \brief     Displays contents of the data stack
 *
 * \return    Nothing
 *
 */
void DispDs(void) {
    int i;

    printf ("\n");

    if (DatStackTop == 0) {                   // if empty
        printf ("\nData stack empty\n");
    } else {
        for (i=0; i<DatStackTop; i++) {
            printf( "%d  ", DatStack[i]);
        }
    }

}

/**
 *
 * \fn           PushRs(int dat, int* err_code)
 * \brief        This function pushes data into Return stack
 *
 *               Return stack is used in FORTH to hold the address of a Word when a call to another word is made.
 *               This stack can also be used as a general purpouse storage area with some extra care.
 *
 * \param[in]    dat      data to be pushed in
 * \param[out]   err_code holds error code on exit
 *
 * \return       STACK_ERR_FULL if stack was full or else STACK_ERR_SUCCESS
 *
 */

int PushRs(int dat, int* err_code) {
    int ret;

    if (RetStackTop == STACK_RET_SIZE-1) {
        ret = *err_code = STACK_ERR_FULL;

    } else {
        RetStack[RetStackTop++] = dat;
        ret = *err_code = STACK_ERR_SUCCESS;
    }

    return ret;
}


/**
 *
 * \fn       PopRs(int * err_code)
 * \brief    This function pops out a data from stack
 *
 *           Return stack is used in FORTH to hold the address of a Word when a call to another word is made.
 *           This stack can also be used as a general purpouse storage area with some extra care.
 *
 * \param[out]  err_code  holds error code
 *
 * \return      data poped out or error code (\a STACK_ERR_EMPTY or STACK_ERR_SUCCESS)
 *
 */

int PopRs(int* err_code) {
    int ret =  STACK_ERR_EMPTY;

    if (RetStackTop == 0) {
        *err_code = STACK_ERR_EMPTY;

    } else {
        *err_code = STACK_ERR_SUCCESS;
        RetStackTop--;
        ret = RetStack[RetStackTop];
    }

    return ret;
}



/**
 *
 * \fn        DispRs(void)
 * \brief     Displays contents of the return stack
 *
 * \return    Nothing
 *
 */

void DispRs(void) {
    int i;
    printf ("\n");

    if (RetStackTop == 0) {                   // if empty
        printf ("return stack empty\n");
    } else {
        for (i=0; i<RetStackTop; i++) {
            printf ("%d\t", RetStack[i]);
        }
    }

}


