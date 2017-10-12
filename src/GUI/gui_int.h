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
*  @file     gui_int.h
*  @brief    Internal header file for gui.c
*/

#ifndef __GUI_INT_H
#define __GUI_INT_H

#include "gui.h"
#include "gui_controls.h"
#include "forth_files.h"
#include "mbed.h"

gui_elem_ptr add_gui_elem(uint32_t id, char* name, int x, int y, enum gui_elem_type type);
int rm_gui_elem(char* name);
int add_button(uint32_t id, char* name, char* caption, int x, int y);
int add_p_bar(uint32_t id, char* name, int x, int y);
int add_st_text (uint32_t id, char *name, uint32_t x, uint32_t y, char* text,
                 uint32_t f_color, uint32_t b_color);
gui_elem_ptr find_gui_elem(uint32_t id);
int set_p_bar(uint32_t id, int vol);
int set_st_text(uint32_t id, char* text);
int set_st_color(uint32_t id, uint32_t fg, uint32_t bg);
void draw_controls(void);
int disptacher(void);
int update_bmp(uint32_t id, char *file_name);
#endif







