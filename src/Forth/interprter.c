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
 * \file       interprter.c
 * \brief      This file contains functions which implement the FORTH interpreter
 *
 *             Forth is interpreted word by word hence this interpreter has to extract each word which are seperated
 *             by spaces
 *
 */

#include <stdio.h>
#include <string.h>
#include "interprter.h"
#include "CoreForth.h"
#include "stack.h"

int AddDicEntry(char* name, int ForthFlags, func_ptr func, int* CodeList, int len);

typedef struct Buffer Buffer;
Buffer output;
char HoldBuffer[200];

int CmdPos;                                 /**< This variable holds the current position of word being parsed */
char CmdBuff[BUFFER_SIZE];                  /**< Buffer used to hold the commands */
bool CompileMode = FALSE;                       /**< Flag to indicate compile mode in FORTH */
int CurrentAddr;                            /**< Holds the current address of word being executed */
int i_pc;                                   /**< Index for current address in a word */
int BASE  = 10;                             /**< Holds current base system. Base 10 by default  */
int CompileCode[FORTH_CODE_SIZE];           /**< To store compiled key words*/
int j_pc;                                   /**< Points to current word that is being compiled within a word */
bool WrdNameFlag = FALSE;                   /**< To indicate that we already have the name */


/**
 *
 * \fn         Word(char* wrd)
 * \brief      This function returns a word from current command buffer
 *
 *             Every successive call to this function will return next word in the command buffer.
 *
 * \param[out] wrd contains a extarcted word from the command buffer
 * \return     Number of words the function Word has extarcted
 *
 */

int skip_flag;               /**< To skip spaces within quotes */
int Word (char* wrd) {

    int i=0;
    static int count;
    int space_flag = 0, q_flag = 0;

    while (CmdBuff[CmdPos] == ' ' || CmdBuff[CmdPos] == '\t' || CmdBuff[CmdPos] == '\n' ) {                                              // skip all the spaces before the word
        CmdPos++;
    }

    while ((CmdBuff[CmdPos] != ' ' || space_flag == 1 )&& CmdBuff[CmdPos] != '\t'&&
            CmdBuff[CmdPos] != '\n' && CmdBuff[CmdPos] != '\0')
        // until the next space skip everything
    {
        if (CmdBuff[CmdPos] == '\"' && skip_flag == 1) {
            space_flag = 1;
            if (q_flag == 0) {
                q_flag = 1;
            } else {
                q_flag = 0;
                space_flag = 0;
            }
        }
        wrd[i] = CmdBuff[CmdPos];
        i++;
        CmdPos++;
    }
    if (i>0) {
        count++;
    }

    wrd[i] = '\0';

    return count;
}

/**
 *
 * \fn          SkipLine(void)
 * \brief       This function skips a line from the input stream
 *
 *              This function is used for implementing line comments
 *
 * \return      Nothing
 *
 */

void SkipLine(void) {
    while (CmdBuff[CmdPos] != '\n') CmdPos++;
}


/**
 * \fn          Interpret(void)
 * \brief       Compiles a word
 *
 *              This function examines each word in the array and then creates an address list
 *              which corresponds to the addresses of composed code words into arr
 *
 *
 * \return      COMPILE_SUCCESS if the compilation was a success else COMPILE_ERROR
 *
 */

int Interpret(void) {
    char Buff[SIZE];
    static char WrdName[SIZE];
    int ForthWrdFnd, cond, InitialAddr,TempAddr;
    int temp, tempi;
    NodePtr CodePtr;
    func_ptr Func;
    bool ExitFlag = FALSE;
    int jmp = 0;

    i_pc = 0;

    if (CompileMode == FALSE) {                            // we are in interpret mode
        Word(Buff);                                         // get a word from input stream
        if (Buff[0] == '\0') {                              // empty string
            return CONTINUE_FORTH_INTERPRET;
        }
        ForthWrdFnd = Find(Buff, &CurrentAddr);             // find the code word

        if (FORTH_WORD_NOT_FOUND == ForthWrdFnd) {

            /* Try and parse the input as a number */

            Number(Buff);
            temp = PopDs(&cond);
            /*if (cond == STACK_ERR_EMPTY)
              {
                return STOP_FORTH_INTERPRET;
                } */
            if ( temp != 0) {
                printf ("%s not recognised \n", Buff);

                temp = PopDs(&cond);                      // pop out partially converted number
                return STOP_FORTH_INTERPRET;
            }
        } else {

            InitialAddr = CurrentAddr;                     // save the first ever address

            CodePtr = (NodePtr)CurrentAddr;

            while (1) {
                CodePtr = (NodePtr)CurrentAddr;
                if ((CodePtr->flag) & FORTH_WORD_INBUILT) {    // if the current code is an inbuilt function
                    if ((CodePtr->flag) & FORTH_COMPILE_ONLY) {
                        printf ("%s can be used only in compile mode\n", Buff);
                        return STOP_FORTH_INTERPRET;
                    }
                    Func = CodePtr->func;
                    (*Func)();                              // execute the function

                    if (CompileMode == TRUE) {
                        break;
                    }

                    CurrentAddr = PopRs(&cond);            // pop up for next execution
                    i_pc = PopRs(&cond);

                    CodePtr = (NodePtr)InitialAddr;

                    if (CurrentAddr == InitialAddr) {      // this becomes true if we are nearing bottom of the stack
                        PushRs(i_pc, &cond);           // make a copy before subsequent pops
                        PushRs(InitialAddr, &cond);
                        CodePtr = (NodePtr)InitialAddr;
                        if (CodePtr->code[i_pc] == END_WORD) { // is it end of the word
                            CurrentAddr = PopRs(&cond);
                            CurrentAddr = PopRs(&cond);  // we do not want to leave some thing stack
                            ExitFlag = TRUE;       // then exit. Remember we are the initial address, first code
                        } else {
                            CurrentAddr = PopRs(&cond);  // if the word is not finished yet, pop up the recent address
                            i_pc = PopRs(&cond);
                        }
                    }

                    jmp = 0;                               // jmp pointer helps us reset i_pc when we jump to a another word
                    // from where we are
                    if (cond == STACK_ERR_EMPTY) {
                        ExitFlag = TRUE;               // Exit if stack is empty
                    }
                } else {

                    jmp++;                      // if we are here more than twice we did not execute any inbuilt function
                    // which can mean only one thing, we are executing non-inbuilt function

                    if (jmp > 1) {
                        i_pc =0;                      // therefore reset pointer so that the new word can begin executing from
                    }                               // code[0] of new word

                    if (CodePtr->code[0] == EMPTY_WORD) {
                        temp = PopRs(&cond);
                        tempi = PopRs(&cond);
                        if (temp == STACK_ERR_EMPTY) {
                            return CONTINUE_FORTH_INTERPRET;
                        }
                        PushRs(tempi, &cond);
                        PushRs(temp, &cond);
                        i_pc++;


                    }
                    if (CodePtr->code[i_pc+1] != END_WORD) {     // do not save address if we are nearing delem
                        PushRs(i_pc+1, &cond);
                        PushRs(CurrentAddr, &cond);
                    }

                    CurrentAddr = CodePtr->code[i_pc];
                    CodePtr = (NodePtr)CurrentAddr;
                    i_pc++;                               // increment program counter
                }

                if (CurrentAddr == END_WORD) {
                    CurrentAddr = PopRs(&cond);          // if at end of a word pop up the addresses
                    i_pc = PopRs(&cond);

                    CurrentAddr = PopRs(&cond);
                    i_pc = PopRs(&cond);


                    CodePtr = (NodePtr)InitialAddr;     // see if we have reached the end of original code
                    if (CodePtr->code[i_pc] != END_WORD) {
                        PushRs(i_pc, &cond);        // if not push the address for next cycle
                        PushRs(InitialAddr, &cond);
                    }

                    if (CurrentAddr == 0 || cond == STACK_ERR_EMPTY) {
                        ExitFlag = TRUE;            // exit
                        break;
                    }

                    CodePtr = (NodePtr)CurrentAddr;   // if we have come till here there are still some word to be executed
                    CurrentAddr = CodePtr->code[i_pc];
                }
                if (TRUE == ExitFlag) {
                    break;
                }

            }
        }
    }

    if (CompileMode == TRUE) {
        // in compile mode
        // here in compile mode we need to look up at every word and add it to the list
        // of array to enter into the dictionary entry but first we need to know the name of the word

        if (WrdNameFlag == FALSE) {
            Word(Buff);                          // take in the next word
            // this one should be name
            if (Buff[0] == '\0') {
                CompileMode = FALSE;
                return CONTINUE_FORTH_INTERPRET ;
            }
            strcpy(WrdName, Buff);
            WrdNameFlag = TRUE;
        }
        //j_pc = 0;                               // set counter = 0
        while (1) {
            Word(Buff);

            if (Buff[0] == '\0') {
                return CONTINUE_FORTH_COMPILE ;
            }

            //if( strcmp(Buff, ";") == 0) break;
            ForthWrdFnd = Find(Buff, &TempAddr);
            if (FORTH_WORD_FOUND == ForthWrdFnd) {
                CodePtr = (NodePtr)TempAddr;
                if ((CodePtr->flag) & (FORTH_WORD_IMED)) {
                    Func = CodePtr->func;
                    (*Func)();                              // execute the function

                    if (CompileMode == FALSE) {
                        break;
                    }
                } else {
                    CompileCode[j_pc] = TempAddr;
                    j_pc++;
                }
            } else {
                // try parsing it as a number
                Number(Buff);
                temp = PopDs(&cond);

                if (temp == 0) {    // it is number
                    Find("LIT", &TempAddr);         // First we add an address of word LIT
                    CompileCode[j_pc] = TempAddr;   // what it does is reads the number (stored next in word i.e in CodeArr as you
                    j_pc++;                      // will see in next line) and then skips this location as if never occured as in jonesforth
                    temp = CompileCode[j_pc] = PopDs(&cond);  // compile the number
                    if (END_WORD == temp) {
                        printf ("WARNING: Using %d in your compiled code will hang the system\n", END_WORD);
                    }
                    j_pc++;
                } else {
                    temp = PopDs(&cond);      // word not found
                    printf ("Word %s not found \n", Buff);
                    j_pc = 0;
                    WrdNameFlag = CompileMode = FALSE;  // continue interpreting
                    return COMPILE_ERROR;
                }
            }
        }
        if (j_pc == 0) {
            CompileCode[j_pc] = EMPTY_WORD;
            j_pc++;
        }
        temp = Find(WrdName, &TempAddr);
        if ( FORTH_WORD_FOUND == temp) {
            printf ("\nWARNING: %s redefined ", WrdName);
        }
        AddDicEntry(WrdName, FORTH_WORD_USER, NULL, CompileCode, j_pc);
        j_pc = 0;
        WrdNameFlag = CompileMode = FALSE;


    }
    return COMPILE_SUCCESS;
}


/**
 *
 * \fn           power (int base, int pwr)
 *
 * \brief        computes the power of number raised to a given number
 *
 * \param[in]   base  The base number to which power has to be raised
 * \param[in]   pwr   The power that will be raised on to the base
 *
 * \return       base^pwr
 *
 *
 */

int power(int base, int pwr) {
    int i;
    int temp=1;

    if (pwr == 0) {
        return 1;
    }

    for (i=0; i<pwr; i++)
        temp = temp*base;

    return temp;
}

/**
 *
 *   \fn           Number(char* str)
 *   \brief        This function converts a string to number.
 *                 This function converts a string to number in current base system and leaves it on the data stack.
 *                 After this function has finished executing the tos would have 0 if the number was parsed successfully
 *                 else it leaves number of unparsed numbers. NOS contains the result.
 *   \param[in]    str  contains the string to be converted into numerals
 *   \note         Forth has this feature where in variable base can be used so this function
 *                 implements variable base system.
 *                 Uses variable \a BASE for using a particular base system.
*
*/

void Number(char* str) {
    int i=0, j=0, temp, negflag = 0;
    int cond;
    int result = 0;
    i = strlen(str);

    i--;

    while (i >= 0) {
        if (str[i] >= '0' && str[i] <= '9') {
            temp = str[i] - '0';

        } else if (str[i] >= 'A' && str[i] <= 'Z') {
            temp = str[i] - 'A';
            temp = temp + 10;
        } else if (str[i]  == '-') {
            negflag = 1;
            i--;                       // subtract i for '-' at str[i]
            break;
        } else {
            break;
        }


        if (temp < BASE) {            // check if this is an allowed number in current base system
            result = result + temp * power(BASE, j);
            j++;
            i--;
        } else {
            break;
        }
    }

    if (negflag == 1) {         // number is negative
        result = result*(-1);
    }
    i++;                        // while will decrement it one extra time
    PushDs(result, &cond);
    PushDs(i , &cond);          // i will contain characters that were not parsed correctly, 0 if all is well


    if (cond == STACK_ERR_FULL) { // no point in storing either number or result
        printf (HoldBuffer, "\nStack is Full ");
        PopDs(&cond);         // do not leave any left overs
    }
}






