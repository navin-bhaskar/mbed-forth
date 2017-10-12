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

#ifndef  __GUI_CONTROLS_MOD_H
#define  __GUI_CONTROLS_MOD_H

#include <string.h>
#include "lcd.h"             // LCD related functions
#include "types.h"
#include "gui_controls.h"



#define CLICK_OFFSET_X      2   /**< The offset for the text on x axis
when the button is clicked */

#define CLICK_OFFSET_Y      2   /**< The offset for the text on y axis
when the button is clicked */

#define FONT_WIDTH          8   /**< The number of pixels the font would 
require along the x axis */

#define FONT_HEIGHT         15  /**< The number of pixels the font would 
require along the y axis */

#define MAX_CHARS_BTN       10  /**< Maximum characters per button */


#define BUTTON_OFFSET_X     12
#define BUTTON_OFFSET_Y     12

#define BUTTON_CLICK_OFFSET_X    2
#define BUTTON_CLICK_OFFSET_Y    -1




#define GUI_BACKGROUND_COLOR   WHITE
#define BUTTON_BORDER_COLOR    BLACK
#define BUTTON_COLOR           ORANGE
#define BUTTON_TEXT_COLOR      WHITE


#define P_BAR_FG_COLOR         RED
#define P_BAR_BG_COLOR         WHITE
#define P_BAR_BORDER_COLOR     BLACK


#endif


