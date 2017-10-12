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
 * \file     stack.h
 * \brief    This file has definations required for stack operations.
 *
 *           This file is very important from perspective of memory usage. The stack sizes can be controlled 
 *           by changing various defines in this file.
 *
 *
 */

#ifndef __STACK_H
#define __STACK_H

#define STACK_DAT_SIZE     50         /**< The maximum size of the data stack */
#define STACK_RET_SIZE     50         /**< The maximum size of the return stack */

#define STACK_ERR_FULL     -1         /**< error code for indicating stack full condition */
#define STACK_ERR_SUCCESS   1         /**< code retuned if data was successfully inserted into stack */
#define STACK_ERR_EMPTY     2         /**< error code returned if stack is empty */

void DispDs(void);
int PopDs(int *err_code);
int PushDs (int dat, int* err_code);
int PushRs(int dat, int* err_code);
int PopRs(int* err_code);
void DispRs(void);



#endif

