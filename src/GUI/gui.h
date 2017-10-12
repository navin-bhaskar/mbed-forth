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
*  @file     gui.h
*  @brief    Definations required by gui.c and files that use gui.c
*/


#ifndef __GUI_H
#define __GUI_H

#include "types.h"
#include "gui_controls.h"
#include "ts.h"
#include "lcd.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct gui_elem* gui_elem_ptr;

typedef struct gui_elem gui_elem;
gui_elem_ptr AddGuiElem(uint32_t id, char* name, int x, int y, enum gui_elem_type type);
int RmGuiElem(char* name);
int AddButton(uint32_t id, char* name, char* caption, char* cb_word, int x, int y);
int AddPBar(uint32_t id, char* name, int x, int y);
int AddStText (uint32_t id, char *name, uint32_t x, uint32_t y, char* text,
               uint32_t f_color, uint32_t b_color);
gui_elem_ptr FindGuiElem(uint32_t id);
int SetPBar(uint32_t id, int vol);
int SetStText(uint32_t id, char* text);
int SetStColor(uint32_t id, uint32_t fg, uint32_t bg) ;
void DrawControls(void);
int AddBMP(uint32_t id, char* name, uint32_t x, uint32_t y, char* file_path);
int UpdateBMP(uint32_t id, char *file_name);
int Dispatcher(char* cb_wrd, int *id);
void RmGuiElemAll(void);
#endif

