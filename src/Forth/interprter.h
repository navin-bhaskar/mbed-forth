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
 * \file       interpreter.h
 * \brief      Function prototype and defination declerations required by interpreter.c
 *
 */

#ifndef __INTERPRETER_H
#define __INTERPRETER_H

#define BUFFER_SIZE      100         /**< Buffer size for holding commands to be parsed */

#define RESET_CMDPOS    CmdPos = 0  /**< Reset command pos so that it points to the begining of the CmdBuff */

#define EMPTY_WORD       7          /**< Inserted when there is empty word defination */

#define COMPILE_SUCCESS  0          /**< Error code to indicate compilation was successfull */
#define COMPILE_ERROR    1          /**< Error code to indicate compilation was unsuccesfull */
#define STOP_FORTH_INTERPRET 3      /**< Indication to stop interpreting */
#define CONTINUE_FORTH_INTERPRET 4  /**< continue interpreting */
#define CONTINUE_FORTH_COMPILE   5  /**< continue compiling */
#define FORTH_CODE_SIZE      100              /**< Maximum words a dictionary entry can have */



extern char CmdBuff[BUFFER_SIZE];

int Word (char* wrd);
int Interpret(void);
void Number(char* str);
int Find(char* name, int* addr);


#endif

