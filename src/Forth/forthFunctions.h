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
 * \file       forthFunctions.h
 * \brief      This file contains function declerations required by forthFunctions.h
 *
 *
 *
 */


#ifndef __FORTH_FUNC_H
#define __FORTH_FUNC_H

#define ARCHITECTURE      4                   /**< 32 bit architecture */
#define FORTH_TRUE         -1                   /**< True is -1 in Forth */
#define FORTH_FALSE          0                      /**< False is 0 in Forth */

#define  PORT_OFFSET        5                  /**< Port offset for indexing port names */



void ColonFunc(void);
int init_dictionary(void);
void Add(void);
void DotS(void);
void Exit(void);
void Sub(void);
void Lit(void);
void Dot(void);
void Mul(void);
void Div(void);
void Swap(void);
void Dup(void);
void Over(void);
void DummyBus(void);
void BaseSet(void);
void Create(void);
void Read(void);
void Write (void);
void QueryBase(void);
void CondBranch(void);
void UnCondBranch(void);
void If(void);
void Then(void);
void Else(void);
void Begin(void);
void Until(void);
void Equal (void);
void GT (void);
void LT (void);
void LTE (void);
void GTE (void);
void Drop(void);
void StopCompile(void);
void DotStr(void);
void DispStr(void);
void SkipComment1(void);
void SkipComment2(void);
void Not(void);
void And(void);
void Or(void);
void Xor(void);
void BitSet(void);
void BitClear(void);
void Cr(void);



void DelayInSec(void);



void MainLoop(void);
void AddTicker(void);
void CreateBtn(void);
void ShowWidgets(void);
void CreatePBar(void);
void SetPBar(void) ;
void CreateStTxt(void);
void SetStTxt(void);
void SetStClr(void);
void SetPort(void);
void ReadPort(void);
void AnalogRead(void);
void AnalogWrite(void);
void ExitMainLoop(void);
void Fload(void);
void AddBmp(void);
void SetBmp(void);
void ClearGui(void);
void SpiWrite(void);
#endif


