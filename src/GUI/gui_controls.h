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
*  @file     gui_controls_mod.h
*  @brief    Private maodule header for internal use by
*            gui_controls_mod.h
*/

#ifndef __GUI_CONTROLS_H
#define __GUI_CONTROLS_H

#include "types.h"
#include "gui.h"
#include "lcd.h"

#ifndef ERR_ERROR
#define ERR_ERROR  -1      /**< Generic error code for */
#endif

#ifndef ERR_SUCESS
#define ERR_SUCCESS  0    /**< Generic error code for */
#endif

#define MAX             10
#define CAP_SIZE        15
#define MAX_TXT         30
#define MAX_WRD_SIZE    30

#define EVENT           1      /**< In case there is an event on the control */
#define NO_EVENT        0      /**< In case there are no pending events */



enum gui_elem_type {
    BUTTON,
    TEXT,
    PROGRESS,
    BMP
};


typedef struct gui_elem* gui_elem_ptr;


struct gui_elem {
    uint32_t id;              /*< Id of this element */
    char name[MAX];           /*< name of the GUI element */
    uint32_t x;                    /*< x coordinate of the element */
    uint32_t y;                    /*< y coordinate of the element */
    uint32_t  width;               /*< Width of the element */
    uint32_t  height;              /*< Height of the element */
    enum gui_elem_type gui_type;   /*< Type of the GUI element */
    void* gui_struct;         /*< info specific to this GUI type */
    gui_elem_ptr next;        /*< To hold the reference to the next type */
};
typedef struct gui_elem gui_elem;


struct p_bar {                /*< Progress bar struct */
    int prev_vol;             /*< Previous voulme */
    int vol;                  /*< Volume of the progress bar */
    uint8_t  created;         /*< Flag to indicate if the bar was drawn*/
};

typedef struct p_bar p_bar;

struct btn {
    char caption[CAP_SIZE];                 /*< Caption of the button */
    char call_back_word[MAX_WRD_SIZE];      /*< To hold call back word when event occurs on this button */
};

typedef struct btn btn;

struct static_text {
    char   txt[MAX_TXT];      /*< Array to hold the text */
    uint32_t f_color;         /*< Foreground color */
    uint32_t b_color;         /*< Background color */
    uint32_t prev_len;        /*< Length of the previous text */
};

typedef struct static_text static_text;



struct bmp_ctl {
    char file_loc[MAX_TXT];       /*< Bit map file on sd card */
};

typedef struct bmp_ctl  bmp_ctl;

#define  CLICKED            1   /**< To indicate clicked condtion */
#define  UNCLICKED          2   /**< To indicate unclicked condition */


#define BTN_HT    50          /*< Predefined button height */
#define BTN_WT    90          /*< Predefined button width */

#define P_BAR_HT  50         /*< Predefined progress bar height */
#define P_BAR_WT  200        /*< Predefined progress bar width */

void GuiButton (uint32_t x, uint32_t y, uint32_t width,
                uint32_t height, char* caption, uint8_t clicked);
void GuiProgressBar(uint32_t x, uint32_t y, uint32_t width,
                    uint32_t height, struct p_bar* bar);
void GuiStaticText(uint32_t x, uint32_t y, static_text* st_txt);
void GuiClear(void);

#endif
