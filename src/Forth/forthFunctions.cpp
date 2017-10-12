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
 * \file       forthFunctions.c
 * \brief      This file contains functions which implement basic Forth words.
 *
 *             All the c functions that will be identified with a word are defined in this file
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mbed.h"
#include "interprter.h"
#include "forthFunctions.h"
#include "CoreForth.h"
#include "stack.h"
#include "CoreForth.h"
#include "types.h"
#include "gui.h"
#include "utils.h"
#include "forth_files.h"



#define  GEN_SIZE         20

#define  INSUFF_PARAMS    0
#define  GUI_NOT_FOUND    1
#define  INVALID_PORT     2
#define  COULD_NOT_FIND_FILE 3
#define  COULD_NOT_ADD_GUI   4



const char ERR_TABLE[][50] = { "\nInsufficient number of parameters ",
                               "\nCould not find GUI element with id ",
                               "\nInvalid port name ",
                               "\nCould not find the file ",
                               "\nCould not add GUI element "
                             };



/**
*
* \fn        init_dictionary(void)
* \brief     Builds the dictionary
*
* \return    0 on success 1 on error
*
*/

int init_dictionary(void) {
    AddDicEntry(":", FORTH_WORD_INBUILT, &ColonFunc, NULL, 0);
    AddDicEntry("+", FORTH_WORD_INBUILT, &Add, NULL, 0);
    AddDicEntry("-", FORTH_WORD_INBUILT, &Sub, NULL, 0);
    AddDicEntry("*", FORTH_WORD_INBUILT, &Mul, NULL, 0);
    AddDicEntry("/", FORTH_WORD_INBUILT, &Div, NULL, 0);
    AddDicEntry(".", FORTH_WORD_INBUILT, &Dot, NULL, 0);
    AddDicEntry(".S", FORTH_WORD_INBUILT, &DotS, NULL, 0);
    AddDicEntry("LIT", FORTH_WORD_INBUILT, &Lit, NULL, 0);
    AddDicEntry("SWAP", FORTH_WORD_INBUILT, &Swap, NULL, 0);
    AddDicEntry("DUP", FORTH_WORD_INBUILT, &Dup, NULL, 0);
    AddDicEntry("OVER", FORTH_WORD_INBUILT, &Over, NULL, 0);
    AddDicEntry("DROP", FORTH_WORD_INBUILT, &Drop, NULL, 0);
    AddDicEntry("BASE", FORTH_WORD_INBUILT, &BaseSet, NULL, 0);
    AddDicEntry("EXIT", FORTH_WORD_INBUILT, &Exit, NULL, 0);
    AddDicEntry("VARIABLE", FORTH_WORD_INBUILT, &Create, NULL, 0);
    AddDicEntry("@", FORTH_WORD_INBUILT, &Read, NULL, 0);
    AddDicEntry("!", FORTH_WORD_INBUILT, &Write, NULL, 0);
    AddDicEntry("?BASE", FORTH_WORD_INBUILT, &QueryBase, NULL, 0);
    AddDicEntry("0BRANCH", FORTH_WORD_INBUILT, &CondBranch, NULL, 0);
    AddDicEntry("BRANCH", FORTH_WORD_INBUILT, &UnCondBranch, NULL, 0);
    AddDicEntry("IF", FORTH_WORD_INBUILT | FORTH_WORD_IMED | FORTH_COMPILE_ONLY , &If, NULL, 0);
    AddDicEntry("THEN", FORTH_WORD_INBUILT | FORTH_WORD_IMED| FORTH_COMPILE_ONLY, &Then, NULL, 0);
    AddDicEntry("ELSE", FORTH_WORD_INBUILT | FORTH_WORD_IMED | FORTH_COMPILE_ONLY, &Else, NULL, 0);
    AddDicEntry("BEGIN", FORTH_WORD_INBUILT | FORTH_WORD_IMED | FORTH_COMPILE_ONLY, &Begin, NULL, 0);
    AddDicEntry("UNTIL", FORTH_WORD_INBUILT | FORTH_WORD_IMED | FORTH_COMPILE_ONLY, &Until, NULL, 0);
    AddDicEntry("=", FORTH_WORD_INBUILT, &Equal, NULL, 0);
    AddDicEntry("<", FORTH_WORD_INBUILT, &LT, NULL, 0);
    AddDicEntry(">", FORTH_WORD_INBUILT, &GT, NULL, 0);
    AddDicEntry("<=", FORTH_WORD_INBUILT, &LTE, NULL, 0);
    AddDicEntry(">=", FORTH_WORD_INBUILT, &Equal, NULL, 0);
    AddDicEntry(";", FORTH_WORD_INBUILT | FORTH_WORD_IMED, &StopCompile, NULL, 0);
    AddDicEntry("STR", FORTH_WORD_INBUILT, &DispStr, NULL, 0);
    AddDicEntry(".\"", FORTH_WORD_INBUILT | FORTH_WORD_IMED | FORTH_COMPILE_ONLY, &DotStr, NULL, 0);
    AddDicEntry("\\", FORTH_WORD_INBUILT | FORTH_WORD_IMED , &SkipComment1, NULL, 0);
    AddDicEntry("(", FORTH_WORD_INBUILT | FORTH_WORD_IMED , &SkipComment2, NULL, 0);
    AddDicEntry("NOT", FORTH_WORD_INBUILT, &Not, NULL, 0);
    AddDicEntry("AND", FORTH_WORD_INBUILT, &And, NULL, 0);
    AddDicEntry("OR", FORTH_WORD_INBUILT, &Or, NULL, 0);
    AddDicEntry("XOR", FORTH_WORD_INBUILT, &Xor, NULL, 0);
    AddDicEntry("?BITSET", FORTH_WORD_INBUILT, &BitSet, NULL, 0);
    AddDicEntry("?BITCLEAR", FORTH_WORD_INBUILT, &BitClear, NULL, 0);
    AddDicEntry("CR", FORTH_WORD_INBUILT, &Cr, NULL, 0);


    AddDicEntry("ML", FORTH_WORD_INBUILT, &MainLoop, NULL, 0);
    AddDicEntry("ADDTICKER", FORTH_WORD_INBUILT, &AddTicker, NULL, 0);
    AddDicEntry("CRT_BTN", FORTH_WORD_INBUILT, &CreateBtn, NULL, 0);
    AddDicEntry("SHOW", FORTH_WORD_INBUILT, &ShowWidgets, NULL, 0);
    AddDicEntry("CRT_P_BAR", FORTH_WORD_INBUILT, &CreatePBar, NULL, 0);
    AddDicEntry("SET_P_BAR", FORTH_WORD_INBUILT, &SetPBar, NULL, 0);
    AddDicEntry("CRT_ST_TXT", FORTH_WORD_INBUILT, &CreateStTxt, NULL, 0);
    AddDicEntry("SET_ST_TXT", FORTH_WORD_INBUILT , &SetStTxt, NULL, 0);
    AddDicEntry("SET_ST_CLR", FORTH_WORD_INBUILT , &SetStClr, NULL, 0);
    AddDicEntry("DIGITALOUT", FORTH_WORD_INBUILT , &SetPort, NULL, 0);
    AddDicEntry("DIGITALIN", FORTH_WORD_INBUILT , &ReadPort, NULL, 0);
    AddDicEntry("ANALOGIN", FORTH_WORD_INBUILT , &AnalogRead, NULL, 0);
    AddDicEntry("ANALOGOUT", FORTH_WORD_INBUILT , &AnalogWrite, NULL, 0);
    AddDicEntry("EXIT_ML", FORTH_WORD_INBUILT , &ExitMainLoop, NULL, 0);
    AddDicEntry("FLOAD", FORTH_WORD_INBUILT , &Fload, NULL, 0);
    AddDicEntry("CRT_BMP", FORTH_WORD_INBUILT , &AddBmp, NULL, 0);
    AddDicEntry("SET_BMP", FORTH_WORD_INBUILT , &SetBmp, NULL, 0);
    AddDicEntry("CLR_GUI", FORTH_WORD_INBUILT , &ClearGui, NULL, 0);
    AddDicEntry("SPIWRITE", FORTH_WORD_INBUILT , &SpiWrite, NULL, 0);
    return 0;
}
/**
 *
 * \fn        ColonFunc(void)
 * \brief     Its primary function is to switch from interpreted mode to compiled mode.
 *            All this function does is, it simply sets the flag variable \a CompileMode to true
 *
 */


extern bool CompileMode;
void ColonFunc(void) {
    CompileMode = TRUE;
}


/**
 *
 * \fn       Add(void)
 * \brief    Adds two numbers.
 *           Takes 2 numbers from data stack adds them leaves the result on tos ( a b -- a+b )
 * \return   void
 *
 */

void Add(void) {
    int cond, temp1, temp2;

    temp1 = PopDs(&cond) ;
    temp2 = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        return ;
    }

    temp1 = temp1+temp2;

    PushDs(temp1, &cond);

}


/**
 *
 * \fn       Sub(void)
 * \brief    Subtracts two numbers.
 *           Takes 2 numbers from data stack subtracts them, leaves the result on tos ( a b -- a-b )
 * \return   void
 *
 */

void Sub(void) {
    int cond, temp1, temp2;

    temp1 = PopDs(&cond) ;
    temp2 = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        return ;
    }

    temp1 = temp2-temp1;

    PushDs(temp1, &cond);

}


/**
 *
 * \fn       DotS(void)
 * \brief    Displays the contents of the stack
 *
 * \return   void
 *
 */

void DotS(void) {
    DispDs();
}


/**
 *
 * \fn     Exit(void)
 * \brief  This function ends the execution of Forth interpreter and returns to OS
 * \return void
 *
 */

void Exit(void) {
    exit(0);
}

/**
 *
 * \fn     Lit(void)
 * \brief  This function pushes a number into DS present within a compiled word
 * \note   Manipulates Return stack
 *
 */

extern int i_pc;
extern int CurrentAddr;
extern int InitialAddr;

void Lit(void) {
    int temp, cond, Addr, tempi;
    NodePtr CodePtr;

    Addr = PopRs(&cond);         // take out the address
    tempi = PopRs(&cond);



    CodePtr = (NodePtr)Addr;


    temp = CodePtr->code[tempi];  // read the number
    //i_pc++;

    PushDs(temp, &cond);      // Push the number onto stack

    temp = CodePtr->code[tempi+1];

    if (temp == END_WORD) return ; // look at Interpreter() function, we are not saving addresses if we are nearing
    // end of the word. The interpreter is built with this logic let us use violate that

    PushRs(tempi+1, &cond);   // store back the address so that interpreter can continue what it was doing
    PushRs(Addr, &cond);      // after skipping the number (tempi+1)



}

/**
 *
 * \fn        Dot(void)
 * \brief     Pops a data element from memory and displays it.
 *
 */

void Dot(void) {
    int temp, cond;

    temp = PopDs(&cond);

    if (cond != STACK_ERR_EMPTY) {
        printf ("\n%d ", temp);
    }
}


/**
 *
 *  \fn       Mul(void)
 *  \brief    multiplies tos with nos (next on stack) and leaves the result on tos ( a b -- a*b ).
 *
 */

void Mul(void) {
    int temp1, temp2, cond;

    temp1 = PopDs(&cond);
    temp2 = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        return ;
    }

    temp1 = temp1*temp2;

    PushDs(temp1, &cond);
}


/**
 *
 *  \fn       Div(void)
 *  \brief    divide nos by tos and leaves the result on tos ( a b -- a/b ).
 *
 */

void Div(void) {
    int temp1, temp2, cond;

    temp1 = PopDs(&cond);
    temp2 = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY || temp1 == 0) {
        return ;
    }

    temp1 = temp2/temp1;

    PushDs(temp1, &cond);
}


/**
 *
 *   \fn      Swap(void)
 *   \brief   This function swaps tos with nos ( a b -- b a ).
 *
 */

void Swap(void) {
    int temp1, temp2, cond;

    temp1 = PopDs(&cond);
    temp2 = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        return ;
    }

    PushDs(temp1, &cond);
    PushDs(temp2, &cond);
}


/**
 *
 *   \fn       Dup(void)
 *   \brief    This function copies tos ( a -- a a )
 *
 */

void Dup(void) {
    int temp, cond;

    temp = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        return ;
    }

    PushDs(temp, &cond);
    PushDs(temp, &cond);
}

/**
 *  \fn       Drop(void)
 *  \brief    Implements the drop word of FOrth ( a -- )
 */

void Drop(void) {
    int  cond;
    PopDs(&cond);

}


/**
 *
 *   \fn       Over(void)
 *   \brief    Iplements the over word of ANS Forth ( a b  -- a b a )
 *
 */

void Over(void) {
    int temp1, temp2, cond;

    temp1 = PopDs(&cond);
    temp2 = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        return ;
    }

    PushDs(temp2, &cond);
    PushDs(temp1, &cond);
    PushDs(temp2, &cond);
}



/**
 *
 * \fn      Base(void)
 * \brief  This function changes the current Base system used in forth.Expects base number to be on TOS
 *
 */

extern int BASE;
void BaseSet(void) {
    int temp, cond;

    temp = PopDs(&cond);

    if (temp < 2 || temp > 34) return;       // what kind of base system 0 is?

    if (cond == STACK_ERR_EMPTY) {
        return ;
    }

    BASE = temp;
}


/**
 *  \fn      Create(void)
 *  \brief   Creates a variable by given name and then allocates memory to it
 *
 */

void Create(void) {
    int *addr;                            // to hold the address
    int CodeArr[5];                       // to hold newely created word
    char buff[10];                        // to hold variable name
    int i=0;
    Word(buff);                           // get the name

    addr = (int*) malloc(sizeof(int));    // allocate memory

    Find("LIT", &CodeArr[i]);
    i++;
    CodeArr[i] = (int)addr;
    i++;

    AddDicEntry(buff, FALSE, NULL, CodeArr, i);


}

/**
 *  \fn       Read(void)
 *  \brief    Performs a read operation from a memroy location found on TOS and stores it on TOS ( addr -- data )
 */

void Read(void) {
    int *TempAddr, temp, cond;

    temp = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        return;
    }

    TempAddr = (int*)temp;

    // try and access memory if crashes nothing can be done
    temp = *TempAddr;

    PushDs(temp, &cond);

}

/**
 *  \fn      Write(void)
 *  \brief   Performs a write operation to a mem location found on TOS and reads data to be written from NOS ( data addr -- )
 */

void Write (void) {
    int *TempAddr, temp, cond;

    temp = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        return ;
    }

    TempAddr = (int*) temp;

    temp = PopDs(&cond);            // read the data

    if (cond == STACK_ERR_EMPTY) {
        return ;
    }

    *TempAddr = temp;
}


/**
 *   \fn      QueryBase
 *   \brief   Reads variable BASE and displays it onto the display.
 */

void QueryBase(void) {
    printf (" %d ", BASE);
}

/**
 *  \fn      CondBranch
 *  \brief   This a conditional branching instruction if tos is 0, false it continues or else branches to offset value
 */

void CondBranch(void) {
    int temp, offset, tempi, cond, Addr;
    NodePtr CodePtr;


    temp = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        temp = 0;
    }

    Addr = PopRs(&cond);         // take out the address
    tempi = PopRs(&cond);

    CodePtr = (NodePtr)Addr;
    offset = CodePtr->code[tempi];  // read the number

    if (temp != 0) {
        PushRs(tempi+1, &cond);   // store back the address so that interpreter can continue what it was doing
        PushRs(Addr, &cond);      // after skipping the offset number
    } else {
        PushRs(tempi+offset, &cond);
        PushRs(Addr, &cond);
    }
}


/**
 *  \fn      UnCondBranch
 *  \brief   This a conditional branching instruction if tos is 0, false it continues or else branches to offset value
 */

void UnCondBranch(void) {
    int offset, tempi, cond, Addr;
    NodePtr CodePtr;


    Addr = PopRs(&cond);         // take out the address
    tempi = PopRs(&cond);

    CodePtr = (NodePtr)Addr;
    offset = CodePtr->code[tempi];  // read the number

    /* Same as conditional branching but no condition checking */
    PushRs(tempi+offset, &cond);
    PushRs(Addr, &cond);

}

/**
 *  \fn       If(void)
 *  \brief    This function implements if only to be used in compile mode
 *
 */

extern int CompileCode[];
extern int j_pc;

void If(void) {
    int TempAddr, cond;

    Find("0BRANCH", &TempAddr);                // find the conditional branching instruction
    CompileCode[j_pc] = TempAddr;
    j_pc++;
    PushDs(j_pc, &cond);                      // We are in compiled mode i guess it is safe to use data stack now for internal use
    if (cond == STACK_ERR_FULL) {
        CompileMode= FALSE;                   // stop compiling
        return ;
    }
    CompileCode[j_pc] = 0;                    // add dummy offset;
    j_pc++;


}

/**
 *  \fn        Then(void)
 *  \brief     This function calculates the ofsett and stores it at the appropriate location
 */
void Then(void) {
    int offset, cond, temp;

    temp = offset = PopDs(&cond);
    if (cond == STACK_ERR_EMPTY) {
        CompileMode= FALSE;
        return;
    }

    offset = j_pc - offset;                     // j_pc is at current location

    CompileCode[temp] = offset;
}

/**
 * \fn         Else(void)
 * \brief      Implements else of Forth language
 */

void Else(void) {
    int offset, cond, temp, TempAddr;

    temp = offset = PopDs(&cond);
    if (cond == STACK_ERR_EMPTY) {
        CompileMode = FALSE;
        return;
    }

    Find("BRANCH", &TempAddr);
    CompileCode[j_pc] = TempAddr;
    j_pc++;

    PushDs(j_pc, &cond);                 // save  PC for Then

    j_pc++;

    if (cond == STACK_ERR_FULL) {
        CompileMode = FALSE;
        return;
    }

    offset = j_pc - offset;
    CompileCode[temp] = offset;
}


/**
 * \fn          Begin(void)
 * \brief       Implements begin of Forth language
 */

void Begin(void) {
    int cond;
    PushDs(j_pc, &cond);

    if (cond == STACK_ERR_FULL) {
        CompileMode = FALSE;
        return ;
    }
}

/**
 * \fn     Until(void)
 * \brief  Implements Until of Forth language
 */

void Until(void) {
    int cond, offset, TempAddr;

    offset = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        CompileMode = FALSE;
        return ;
    }

    Find("0BRANCH", &TempAddr);
    CompileCode[j_pc] = TempAddr;
    j_pc++;

    offset = offset-j_pc;                   // negative offset

    CompileCode[j_pc] = offset;
    j_pc++;
}

/**
 * \fn     Equal(void)
 * \brief  Implements conditional operator =
 */

void Equal (void) {
    int cond, temp1, temp2;

    temp1 = PopDs(&cond);
    temp2 = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        return;
    }

    if (temp1 == temp2) {
        PushDs(FORTH_TRUE, &cond);              // indicate true
    } else {
        PushDs(FORTH_FALSE, &cond);              // indicate false
    }
}


/**
 * \fn     GT(void)
 * \brief  Implements comparision operator >
 */

void GT (void) {
    int cond, temp1, temp2;

    temp1 = PopDs(&cond);
    temp2 = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        return;
    }

    if (temp2 > temp1) {
        PushDs(FORTH_TRUE, &cond);              // indicate true
    } else {
        PushDs(FORTH_FALSE, &cond);              // indicate false
    }
}

/**
 * \fn     LT(void)
 * \brief  Implements comparision operator <
 */

void LT (void) {
    int cond, temp1, temp2;

    temp1 = PopDs(&cond);
    temp2 = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        return;
    }

    if (temp2 < temp1) {
        PushDs(FORTH_TRUE, &cond);              // indicate true
    } else {
        PushDs(FORTH_FALSE, &cond);              // indicate false
    }
}

/**
 * \fn     LTE(void)
 * \brief  Implements conditional operator <=
 */

void LTE (void) {
    int cond, temp1, temp2;

    temp1 = PopDs(&cond);
    temp2 = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        return;
    }

    if (temp2 <= temp1) {
        PushDs(FORTH_TRUE, &cond);              // indicate true
    } else {
        PushDs(FORTH_FALSE, &cond);              // indicate false
    }
}



/**
 * \fn     GTE(void)
 * \brief  Implements conditional operator >=
 */


void GTE (void) {
    int cond, temp1, temp2;

    temp1 = PopDs(&cond);
    temp2 = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        return;
    }

    if (temp2 >= temp1) {
        PushDs(FORTH_TRUE, &cond);              // indicate true
    } else {
        PushDs(FORTH_FALSE, &cond);              // indicate false
    }
}



/**
 *  \fn     StopCompile(void)
 *  \brief  Exits from compile mode, sets \a CompileMode flag to FALSE
 */

void StopCompile(void) {
    CompileMode = FALSE;

}




/**
 *  \fn      DotStr(void)
 *  \brief   Compiles a string in a word.
 *  \note    This function is strictly tied to 32-bit architecture and also Unicode characters cannot be used with this function
 */

extern char CmdBuff[];
extern int CmdPos;
void DotStr(void) {
    int TempAddr;
    int i=0;


    Find("STR", &TempAddr);        // get the word which actually prints out the string

    CompileCode[j_pc] = TempAddr;
    j_pc++;

    CmdPos++;                     // skip the blank

    while (CmdBuff[CmdPos] != '\"') {
        //CompileCode[j_pc] = CmdBuff[CmdPos];
        CompileCode[j_pc] = 0;
        for (i=0; i<ARCHITECTURE; i++) {
            if (CmdBuff[CmdPos] == '\"' || CmdBuff[CmdPos] == 0x0d) {
                break;
            }
            CompileCode[j_pc] |= CmdBuff[CmdPos] << (i*ARCHITECTURE*2);   // shift and pack the data
            CmdPos++;
        }
        j_pc++;
    }

    CmdPos++;                      // skip "

    if (i>0) {
        while (i != 0) {
            CompileCode[j_pc] |= 0 << (i*ARCHITECTURE);        // fill remaining fields
            i--;
        }
    } else {
        CompileCode[j_pc] = 0;         // indicate end of string
    }
    j_pc++;
}


/**
 *  \fn      DispStr(void)
 *  \brief   Displays the string
 *  \note    This function is strictly tied to 32-bit architecture and also Unicode characters cannot be used with this function
 */

bool lcd_st_txt = false;          /**< This flag will be set by the word that targets the static text, somewhere down this file */
int lcd_st_id;                    /**< The word resposnisble for setting text of a string should load approporiate id to this variable */
bool lcd_bmp = false;            /**< Similar to \a lcd_st_txt but works on bmp control */
int lcd_bmp_id;                  /**< Same as \a lcd_st_id but works on bit map */

void DispStr(void) {

    char temp;
    char buff[MAX_TXT], temp_buff[2];
    int tempi, cond, Addr, packed, i=0;
    NodePtr CodePtr;

    Addr = PopRs(&cond);         // get the address
    tempi = PopRs(&cond);


    buff[0] = '\0';            // this array will hold the unpacked string
    temp = 5;                 // to start with
    while (temp != 0) {
        CodePtr = (NodePtr)Addr;
        packed = CodePtr->code[tempi];  // read the number

        for (i=0; i<ARCHITECTURE; i++) {
            temp = (char)(packed & 0xFF);
            if (temp == 0) break;
            sprintf (temp_buff, "%c", temp);
            strcat(buff, temp_buff);
            packed = packed >> ARCHITECTURE*2;
        }
        tempi++;
    }

    if ( i > 0) tempi++;

    if (lcd_st_txt == true) {
        if (lcd_st_id != -1) { // no error
            SetStText(lcd_st_id, buff);
        }
        lcd_st_txt = false;
    } else if (lcd_bmp == true) {
        if (lcd_bmp_id != -1 ) {
            UpdateBMP(lcd_bmp_id, buff);
        }
        lcd_bmp = false;   // ready the flag for next run
    }

    else {
        // go to stdout
        printf ("%s", buff);
    }

    PushRs(tempi, &cond);   // store back the address so that interpreter can continue what it was doing
    PushRs(Addr, &cond);      // after skipping the offset number
}


/**
 *   \fn       SkipComment1(void)
 *   \brief    Skips line comments that start with    \
 */

void SkipComment1(void) {
    RESET_CMDPOS;             // skip every thing there is in input buffer
    CmdBuff[0] = '\0';        // empty the string buffer
}


/**
 *  \fn      SkipComment2(void)
 *  \brief   This skips inline/parantesized comment starting with ( and ending with )
 */

void SkipComment2(void) {
    while (CmdBuff[CmdPos] != '\0') {
        if (CmdBuff[CmdPos] == ')') {
            break;
        }
        CmdPos++;
    }
    CmdPos++;
}


/**
 *    \fn       Not(void)
 *    \brief    Inverts the truth value of Top of stack
 *
 */

void Not(void) {
    int cond;

    PopDs(&cond)?PushDs(FORTH_FALSE, &cond):PushDs(FORTH_TRUE, &cond);
}

/**
 *   \fn       And(void)
 *   \brief    performs logical 'and' on TOS and NOS. Leaves the result on TOS
 *
*/

void And(void) {
    int temp1, temp2, cond;

    temp1 = PopDs(&cond);
    temp2 = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        return;
    }

    temp1 = temp1 & temp2;

    PushDs(temp1, &cond);
}


/**
 *   \fn       Or(void)
 *   \brief    performs logical 'or' on TOS and NOS. Leaves the result on TOS
 *
 */

void Or(void) {
    int temp1, temp2, cond;

    temp1 = PopDs(&cond);
    temp2 = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        return;
    }

    temp1 = temp1 | temp2;

    PushDs(temp1, &cond);
}

/**
 *   \fn       Xor(void)
 *   \brief    performs logical 'Xor' on TOS and NOS. Leaves the result on TOS
 *
 */

void Xor(void) {
    int temp1, temp2, cond;

    temp1 = PopDs(&cond);
    temp2 = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        return;
    }

    temp1 = temp1 ^ temp2;

    PushDs(temp1, &cond);
}

/**
 *    \fn      BitSet(void)
 *    \brief   Tests if a particular bit of next on sack is set. TOS should have bit position to be tested.
 *             Leaves TOS with the TRUE value if bit was set or else leaves FALSE value.
 */

void BitSet(void) {
    int temp1, temp2, cond;

    temp1 = PopDs(&cond);
    temp2 = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        return;
    }

    (1 << temp1) & temp2 ? PushDs(FORTH_TRUE, &cond):PushDs(FORTH_FALSE, &cond);

}


/**
 *    \fn      BitClear(void)
 *    \brief   Tests if a particular bit of next on sack is reset. TOS should have bit position to be tested.
 *             Leaves TOS with the TRUE value if bit was reset or else leaves FALSE value.
 */

void BitClear(void) {
    int temp1, temp2, cond;

    temp1 = PopDs(&cond);
    temp2 = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        return;
    }

    (~(1 << temp1)) & temp2 ? PushDs(FORTH_TRUE, &cond):PushDs(FORTH_FALSE, &cond);

}

/**
* This function emits a new line
*/
void Cr(void) {
    printf ("\n");
}


// ------------------------------------------------------------------------

/**
 *  \fn     DelayInSec(void)
 *  \brief  Delays execution in seconds, obtained from data stack
 */

void DelayInSec(void) {
    int temp, cond;

    temp = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        return;
    }

    wait(temp);

}

/**
*   This function causes the execution to come out of the main loop
*/

bool exit_ml = false;

void ExitMainLoop(void) {
    exit_ml = true;
}

/**
 *  \fn         MainLoop(void)
 *  \brief      Word call back
*/

void MainLoop(void) {
    int ret, id;
    int cmd_pos_temp;
    char cmd_buff_temp[50];
    char cb_wrd[MAX_WRD_SIZE];

    strcpy(cmd_buff_temp, CmdBuff);  // save the context
    cmd_pos_temp = CmdPos;

    while (1) {

        ret = NO_EVENT;
        while (ret != EVENT) {
            // wait for an event
            ret = Dispatcher(cb_wrd, &id);
        }

        strcpy(CmdBuff, cb_wrd);         // load with the new context
        CmdPos = 0;

        ret = Interpret();

        if (exit_ml == true) {
            exit_ml = false;            // ready the flag for next round
            break;
        }
    }

    CmdPos = cmd_pos_temp;
    strcpy(CmdBuff, cmd_buff_temp);

}


/**
 * Executes the ticker word
 */

char ticker_cb[20];            /*< Ticker callback word name */
Ticker tickWord;               /*< Ticker */

void CallBackTick(void) {
    int cmd_pos_temp;
    int res;
    char cmd_buff_temp[50];

    strcpy(cmd_buff_temp, CmdBuff);  // save the context
    cmd_pos_temp = CmdPos;
    CmdPos = 0;
    strcpy(CmdBuff, ticker_cb);

    res = Interpret();               // execute the word

    if (res == COMPILE_ERROR || res == STOP_FORTH_INTERPRET) {
        // error while executing the word. No word defined ?
        tickWord.detach();
    }
    CmdPos = cmd_pos_temp;
    strcpy(CmdBuff, cmd_buff_temp);

}

/**
 * Adds a ticker word
 */

void AddTicker(void) {
    int del, cond;
    Word(ticker_cb);

    if (ticker_cb[0] == '\0') {
        printf ("\nPlease specify a callback word for the ticker ");
        return;
    }
    ToUp(ticker_cb);

    del = PopDs(&cond);
    if (cond == STACK_ERR_EMPTY) {
        printf ("\nThis word requires delay in seconds to be pushed onto stack ");
        return;
    }

    // else
    tickWord.attach(&CallBackTick, del);       // no real delays

}

/**
 * Creates a button with given parameters.
 * On stack, the parameter list is x, y id crt_btn "button_name" "button_lbl" call_back_wrd
 */

extern int skip_flag;
void CreateBtn(void) {
    int cond = STACK_ERR_FULL;
    int id, x, y;
    char btn_name[GEN_SIZE], btn_lbl[GEN_SIZE], cb_wrd[MAX_WRD_SIZE];

    id = PopDs(&cond);
    y = PopDs(&cond);
    x = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        ErrorCond(FORTH_FALSE);
        return ;
    }
    // get button_name and button_label
    skip_flag = 1;
    Word(btn_name);
    skip_flag = 0;
    if (btn_name[0] == '\0') {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        ErrorCond(FORTH_FALSE);
        return ;
    }
    skip_flag = 1;
    Word(btn_lbl) ;
    skip_flag = 0;
    if (btn_lbl[0] == '\0') {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        ErrorCond(FORTH_FALSE);
        return ;
    }

    Word(cb_wrd) ;
    if (btn_lbl[0] == '\0') {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        ErrorCond(FORTH_FALSE);
        return ;
    }

    ReplaceQuotes(btn_lbl);
    ReplaceQuotes(btn_name);
    ToUp(cb_wrd);
    cond = AddButton(id, btn_lbl, btn_name, cb_wrd, x, y);
    if (cond == ERR_ERROR) {
        printf (ERR_TABLE[COULD_NOT_ADD_GUI]);
        ErrorCond(FORTH_FALSE);
    }
    ErrorCond(FORTH_TRUE);
}

/**
 * Lays out all the GUI elements
 */

void ShowWidgets(void) {
    DrawControls();
}

/**
*  This word adds a progress bar to the widget list
*/

void CreatePBar(void) {
    int id, cond, x, y;
    char p_name[GEN_SIZE];
    cond = STACK_ERR_FULL;

    id = PopDs(&cond);
    y = PopDs(&cond);
    x = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        ErrorCond(FORTH_FALSE);
        return ;
    }
    // get button_name and button_label
    skip_flag = 1;
    Word(p_name);
    skip_flag = 0;
    if (p_name[0] == '\0') {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        ErrorCond(FORTH_FALSE);
        return ;
    }

    ReplaceQuotes(p_name);
    cond = AddPBar(id, p_name, x, y);
    if (cond == ERR_ERROR) {
        printf (ERR_TABLE[COULD_NOT_ADD_GUI]);
        ErrorCond(FORTH_FALSE);
    }
    ErrorCond(FORTH_TRUE);
}

/**
*  This function sets volume for a progress bar
*/

void SetPBar(void) {
    int id, cond, vol;
    cond = STACK_ERR_FULL;
    id = PopDs(&cond);
    vol = PopDs(&cond);


    if (cond == STACK_ERR_EMPTY) {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        ErrorCond(FORTH_FALSE);
        return ;
    }
    cond = SetPBar(id, vol);

    if (cond == ERR_ERROR) {
        printf (ERR_TABLE[GUI_NOT_FOUND]);
        printf ("%d", id);
        ErrorCond(FORTH_FALSE);
    }
    ErrorCond(FORTH_TRUE);
}

/**
*  This function creates a static text control
*  f_Color b_bolor x y id crt_sttxt name text
*/

extern int *color_table;

void CreateStTxt(void) {
    int id, cond, x, y, f_clr, b_clr;
    char txt_name[GEN_SIZE], txt[GEN_SIZE];
    cond = STACK_ERR_FULL;

    id = PopDs(&cond);
    y = PopDs(&cond);
    x = PopDs(&cond);
    b_clr = PopDs(&cond);
    f_clr = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        ErrorCond(FORTH_FALSE);
        return ;
    }

    f_clr = ColorVal(f_clr);
    b_clr = ColorVal(b_clr);
    // get name and text
    skip_flag = 1;
    Word(txt_name);
    skip_flag = 0;
    if (txt_name[0] == '\0') {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        ErrorCond(FORTH_FALSE);
        return ;
    }
    skip_flag = 1;
    Word(txt);
    skip_flag = 0;
    if (txt[0] == '\0') {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        ErrorCond(FORTH_FALSE);
        return ;
    }

    ReplaceQuotes(txt_name);
    ReplaceQuotes(txt);
    cond = AddStText(id, txt_name, x, y, txt, f_clr, b_clr);

    if (cond == ERR_ERROR) {
        printf (ERR_TABLE[COULD_NOT_ADD_GUI]);
        ErrorCond(FORTH_FALSE);
    }
    ErrorCond(FORTH_TRUE);
}

/**
* This function sets a text for static text control
* id set_st_txt ." text "
*/

void SetStTxt(void) {
    int cond;
    lcd_st_txt = true;
    lcd_st_id = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        lcd_st_id = -1;
        ErrorCond(FORTH_FALSE);
        return ;
    }

}

/**
*  This function sets fore groud color and back ground color of a static text
*/

void SetStClr(void) {
    int id, cond, f_clr, b_clr;
    cond = STACK_ERR_FULL;

    id = PopDs(&cond);
    b_clr = PopDs(&cond);
    f_clr = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        ErrorCond(FORTH_FALSE);
        return ;
    }

    f_clr = ColorVal(f_clr);
    b_clr = ColorVal(b_clr);

    id = SetStColor(id, f_clr, b_clr);

    if (id == ERR_ERROR) {
        printf (ERR_TABLE[GUI_NOT_FOUND]);
        printf ("%d", id);
        ErrorCond(FORTH_FALSE);
    }
    ErrorCond(FORTH_TRUE);
}


/**
* This function writes a digital value to given port pin
* ( port_val port_position DigitalOut -- )
*
* @note  On mbed thereare port form 5 to 30 so valid port_position are from 5 to 30 (inclusive of both)
*        and for accessing led1-led4, the valid port_positions are 31, 32, 33 and 34
*/

void SetPort(void) {
    int cond, port_val, port_index;
    DigitalOut ports[] = {p9, p9, p9, p9, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20,
                          p21, p22, p23, p24, p25, p26, p27, p28, p29, LED1, LED1, LED2, LED3, LED4
                         };

    int max=sizeof(ports)/sizeof(DigitalOut);

    cond = STACK_ERR_FULL;

    port_index = PopDs(&cond);
    port_val = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        return ;
    }

    port_index = port_index - PORT_OFFSET;       // to index the array elements properly

    if (port_index < 0 || port_index >= max) {
        printf (ERR_TABLE[INVALID_PORT]);
        return ;
    }
    // set the port values
    if (port_val == 0) {
        ports[port_index] = 0;
    } else {
        ports[port_index] = 1;
    }
}

/**
* This function reads digital logic level at a given port and pushes either 1 or
* 0 on to stack depending on the logic voltage at that pin
* ( port_position DigitalIn -- logic_level_at_that_port )
*
*/

void ReadPort(void) {
    int cond, port_val, port_index;
    DigitalOut ports[] = {p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20,
                          p21, p22, p23, p24, p25, p26, p27, p28, p29, p30
                         };

    int max=sizeof(ports)/sizeof(DigitalOut);

    cond = STACK_ERR_FULL;

    port_index = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        return ;
    }

    port_index = port_index - PORT_OFFSET;       // to index the array elements properly

    if (port_index < 0 || port_index >= max) {
        printf (ERR_TABLE[INVALID_PORT]);
        return ;
    }
    port_val = ports[port_index];
    PushDs(port_val, &cond);
}

/**
* Reads analog voltage at a given pin and pushes the value onto stack
* ( ch_index DigitalIn -- read_value )
*  channel 0-4 p15-p20
*/

void AnalogRead(void) {
    int cond, channel;
    AnalogIn ain_channels[] = {p15, p16, p17, p18, p19};

    int max=sizeof(ain_channels)/sizeof(DigitalOut);

    cond = STACK_ERR_FULL;

    channel = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        return ;
    }

    if (channel < 0 || channel >= max) {
        printf (ERR_TABLE[INVALID_PORT]);
        return ;
    }

    channel =  ain_channels[channel];
    PushDs(channel, &cond);
}

/**
* Outputs a given analog value at p18 pin
*/

void AnalogWrite(void) {
    int cond, aout_val;
    AnalogOut aout(p18);

    cond = STACK_ERR_FULL;

    aout_val = PopDs(&cond);

    if (cond == STACK_ERR_EMPTY) {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        return ;
    }

    aout = aout_val;
}

/**
* Loads a file from the sd card and executes it
*/

void Fload(void) {
    char file_name[40];
    int res;

    skip_flag = 1;
    Word(file_name);
    skip_flag = 0;
    if (file_name[0] == '\0') {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        return ;
    }

    ReplaceQuotes(file_name);

    res = ExecFromFile(file_name);

    if (res == FILE_NOT_FOUND) {
        printf (ERR_TABLE[COULD_NOT_FIND_FILE]);
        printf ("%s", file_name);
    }
}

/**
* Creates a bit map control.
* ( x y id crt_bmp "name" "file_name" )
*/

void AddBmp(void) {
    int id, cond, x, y;
    char bmp_name[GEN_SIZE], file_loc[GEN_SIZE];
    cond = STACK_ERR_FULL;

    id = PopDs(&cond);
    y = PopDs(&cond);
    x = PopDs(&cond);


    if (cond == STACK_ERR_EMPTY) {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        ErrorCond(FORTH_FALSE);
        return ;
    }

    // get name and text
    skip_flag = 1;
    Word(bmp_name);
    skip_flag = 0;
    if (bmp_name[0] == '\0') {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        ErrorCond(FORTH_FALSE);
        return ;
    }
    skip_flag = 1;
    Word(file_loc);
    skip_flag = 0;
    if (file_loc[0] == '\0') {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        ErrorCond(FORTH_FALSE);
        return ;
    }

    ReplaceQuotes(bmp_name);
    ReplaceQuotes(file_loc);
    if (AddBMP(id, bmp_name, x, y, file_loc) == ERR_ERROR) {
        printf ("\nError Adding BMP control ");
        ErrorCond(FORTH_FALSE);
        return;
    }
    ErrorCond(FORTH_TRUE);

}

/**
* Given an id, this function chages the source fr bit map data and
* redraws it. This function actually sets the flags for STR word.
*/

void SetBmp(void) {
    int id, cond;
    cond = STACK_ERR_FULL;

    id = PopDs(&cond);
    lcd_bmp = true;
    if (cond == STACK_ERR_EMPTY) {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        lcd_bmp_id = -1;
        return ;
    }

    lcd_bmp_id = id;
}

/**
* this word deletes all the GUI elements.
*/

void ClearGui(void) {
    RmGuiElemAll();
}

/**
* This word provides access to the SPI bus
*/

void SpiWrite(void) {
    SPI spi(p11, p12, p13);
    DigitalOut cs(p14);
    int cond, i, data, bits, no_bytes, mode, freq;


    freq = PopDs(&cond);
    if (cond == STACK_ERR_EMPTY) {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        return;
    }

    mode = PopDs(&cond);
    mode = mode>3?0:mode;      // let mode default to 0 in case of invalid mode
    if (cond == STACK_ERR_EMPTY) {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        return;
    }
    bits = PopDs(&cond);
    if (cond == STACK_ERR_EMPTY) {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        return;
    }
    if (bits != 16 && bits != 8) {
        bits = 8;                // defualt to 8-bit mode
    }

    no_bytes = PopDs(&cond);
    if (cond == STACK_ERR_EMPTY) {
        printf (ERR_TABLE[INSUFF_PARAMS]);
        return;
    }
    spi.format(bits, mode);
    spi.frequency(freq);


    cs = 0;
    for (i=0; i<no_bytes; i++) {
        data = PopDs(&cond);
        if (cond == STACK_ERR_EMPTY) {
            cs = 1;
            return ;          // no data in the stack to write
        }
        spi.write(data);
    }
    cs = 1;               // deselct the spi device
}


