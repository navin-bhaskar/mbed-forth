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
*  @file     gui_controls.c
*  @brief    Has functions that construct the basic GUI elements.
*            all functions currently do not use any bitmaps to display
*            GUI controls. All GUI are drawn using primitive objects.
*/

#include "gui_controls_mod.h"
#include "gui.h"
#include "lcd.h"



/**
 * This function draws the button.
 * Using the primitives provided by the lcd.c file, we lay ou this
 * control.
 *
 * @param[in]   x           the x coordinate
 * @param[in]   y           the y coordinate
 * @param[in]   width       width of the button
 * @param[in]   height      height of the button
 * @param[in]   caption     Label of the button
 * @param[in]   clicked     Indicates if the button has to be drawn in
 *                          clciked position
 *
 */

void GuiButton (uint32_t x, uint32_t y, uint32_t width,
                uint32_t height, char* caption, uint8_t clicked) {
    uint32_t len = strlen(caption);
    uint32_t mid_x, mid_y, len_x, len_y;
    char temp_str[40];
    uint32_t i;

    /** We need to calculate the space requirements to place the caption
     *  text in the center of the button */
    len_y = (len)*8;          // total width the text would require
    //len_y =0;
    //mid_y = ((y+width)-len_y)/2;                      // the mid point

    if (len < MAX_CHARS_BTN) {
        len = MAX_CHARS_BTN - len;
        len = len>>1;   // divide by two
        len = len ;
        i = 0;
        while (len--) {
            temp_str[i] = ' ';
            i++;
        }
        temp_str[i] = '\0';
        strcat(temp_str, caption);
    }
    mid_y = (y+width-88);

    len_x = FONT_HEIGHT;
    mid_x = height + x - 30;



    /** To draw the button, we draw a black border with given parameters.
     *  Then fill the box with required color and text */

    if (clicked == UNCLICKED) {
        LCD_Draw_Rect(x, y, x+height, y+width, NO_FILL, BUTTON_BORDER_COLOR);
        // for the shadow effect
        LCD_Draw_Line(x+height-1, y+width-1, x+height-1, y, BUTTON_BORDER_COLOR);
        LCD_Draw_Line(x, y+width-1, x+height, y+width-1, BUTTON_BORDER_COLOR);
        LCD_Draw_Line(x+1, y+1, x+1, y+width-1, GUI_BACKGROUND_COLOR);
        LCD_Draw_Line(x+1, y+1, x+height-1, y+1, GUI_BACKGROUND_COLOR);
        LCD_Draw_Rect(x+2, y+2, x+height-1, y+width-1, FILL, BUTTON_COLOR);

        //LCD_Draw_Rect(mid_x-BUTTON_CLICK_OFFSET_X, mid_y-BUTTON_CLICK_OFFSET_Y,
        //          (mid_x-BUTTON_CLICK_OFFSET_X)+len_x, mid_y+(mid_y-BUTTON_CLICK_OFFSET_Y)+10, FILL, BUTTON_COLOR);

        // draw the string
        LCD_write_string(mid_x, mid_y, (unsigned char*)temp_str, BUTTON_TEXT_COLOR, BUTTON_COLOR);
    } else {
        // erase the border
        LCD_Draw_Rect(x, y, x+height, y+width, NO_FILL, GUI_BACKGROUND_COLOR);
        LCD_Draw_Line(x+height-1, y+width-1, x+height-1, y, GUI_BACKGROUND_COLOR);
        LCD_Draw_Line(x, y+width-1, x+height, y+width-1, GUI_BACKGROUND_COLOR);

        LCD_Draw_Rect(x+2, y+2, x+height-2, y+width-2, NO_FILL, BUTTON_BORDER_COLOR);

        LCD_Draw_Rect(mid_x, mid_y+30, mid_x+len_x, mid_y+len_y+10, FILL, BUTTON_COLOR);

        LCD_write_string(mid_x-BUTTON_CLICK_OFFSET_X, mid_y-BUTTON_CLICK_OFFSET_Y,
                         (unsigned char*)temp_str, BUTTON_TEXT_COLOR, BUTTON_COLOR);
    }
}


/**
* Draws the progress bar.
* While drawing the bar for the first time, set bar->created to FALSE.
*
* @param[in]   bar   structure containing information related to drawing the bar
*
* @note   When the progress bar is created for the first time, the progress text
*         in center is not diaplyed.
*
*/

void GuiProgressBar(uint32_t x, uint32_t y, uint32_t width,
                    uint32_t height, struct p_bar* bar) {
    float temp;
    uint32_t volume;
    int temp1, temp2;
    char buff[5];


    if (bar->created == FALSE) {
        LCD_Draw_Rect(x, y, x+height, y+width, NO_FILL, P_BAR_BORDER_COLOR);
        LCD_Draw_Line(x+1, y+1, x+1, y+width-1, GUI_BACKGROUND_COLOR);
        LCD_Draw_Line(x+1, y+1, x+height-1, y+1, GUI_BACKGROUND_COLOR);
        LCD_Draw_Line(x+height-1, y+1, x+height-1, y+width-1, P_BAR_BORDER_COLOR);
        LCD_Draw_Line(x+1, y+width-1, x+height-1, y+width-1, P_BAR_BORDER_COLOR);
        bar->created = TRUE;
        return ;         // do not display the string representing the progress numeral
    }

    if (bar->vol <= 0) {
        temp1 = y+2;
        LCD_Draw_Rect(x+2, temp1, x+height-2,y+width-2, FILL, P_BAR_BG_COLOR);
        bar->prev_vol = 0;
        bar->vol = 0;
        goto print_per;
    } else if (bar->vol >=100) {
        temp1 = y+2;
        LCD_Draw_Rect(x+2, temp1+1, x+height-2,y+width-2, FILL, P_BAR_FG_COLOR);
        bar->prev_vol = 100;
        bar->vol = 100;
        goto print_per;
    }

    temp = (width-4)/100.0;
    temp = temp*(bar->vol);
    volume = (int)(temp);



    if (volume >= bar->prev_vol) {

        if (bar->prev_vol == 0) {
            temp1 = y+3;
        } else {
            temp1 = y+3+(volume-bar->prev_vol);
        }
        LCD_Draw_Rect(x+2, temp1, x+height-2,y+volume, FILL, P_BAR_FG_COLOR);
    } else {
        if (bar->prev_vol == 100) {
            temp2 = y+width-2;
        } else {
            temp2 = y+bar->prev_vol;
        }
        temp1 = (bar->prev_vol - volume);
        temp1 = (y+bar->prev_vol)-temp1;
        LCD_Draw_Rect(x+2, temp1, x+height-2,temp2, FILL, P_BAR_BG_COLOR);
    }
    bar->prev_vol = volume;

print_per:
    sprintf (buff, "%d", bar->vol);
    strcat(buff, "%");
    if (bar->vol < 10 ) {
        // clear the extra '%'
        LCD_Draw_Rect((x + height/2)-10, (width+y)/2+10,
                      x + height/2 - 10, (width+y)/2+50, FILL, GREEN);

    }

    LCD_write_string ((x + height/2)-10, (width+y)/2+10, (unsigned char*)buff,
                      P_BAR_FG_COLOR, GUI_BACKGROUND_COLOR);

}


/**
 * This function draws static texts on to the LCD screen.
 * This function is simply calls LCD_write_string.
 *
 * @param[in]    x       x location for the text
 * @param[in]    y       y location for the text
 * @param[in]  st_txt    Static text contrtol struct
 *
 */

void GuiStaticText(uint32_t x, uint32_t y, static_text* st_txt) {
    uint32_t temp;

    if (st_txt->prev_len != 0) {
        temp = st_txt->prev_len;
        temp = FONT_WIDTH*temp;

        LCD_Draw_Rect(x, y, x+FONT_HEIGHT, y+temp, FILL, GUI_BACKGROUND_COLOR);
    }

    LCD_write_string(x, y, (unsigned char*)st_txt->txt, st_txt->f_color, st_txt->b_color);
    st_txt->prev_len = strlen(st_txt->txt);
}

/**
* This function clears the LCD to the set backgorund color
*/

void GuiClear(void) {
    LCD_Clear(GUI_BACKGROUND_COLOR);
}









