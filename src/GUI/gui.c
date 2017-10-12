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
*  @file       gui.c
*  @brief      This file contains the GUI management functions.
*              A linked list containing information required to draw
*              GUI elements is created and maintained here.
*/

#include "gui_int.h"


gui_elem_ptr GUI_FIRST;        /*< The head ptr */

/**
 *  This function adds a gui_elem node to the linked list.
 * gui_elem is a generic node which is added to the list.
 *
 * @param[in]      id      id of this element
 * @param[in]     name     name of the GUI element
 * @param[in]      x       x location of the GUI element
 * @param[in]      y       y location of the GUI element
 * @param[in]     type     GUI element type
 *
 * @return        address of the newly added item
 */

gui_elem_ptr AddGuiElem(uint32_t id, char* name, int x, int y, enum gui_elem_type type) {
    gui_elem_ptr temp, mid;
    mid = (gui_elem_ptr)malloc(sizeof(gui_elem));
    if (mid == NULL) {
        return NULL;     // error
    }

    // fill up the members of this struct
    strcpy(mid->name, name);
    mid->x = x;
    mid->y = y;
    mid->gui_type = type;
    mid->gui_struct = NULL;     // initialize the generic pointer to NULL
    mid->id = id;

    if (GUI_FIRST == NULL) {
        // first element ever
        mid->next = NULL;
        GUI_FIRST = mid;
        return mid;
    } else {
        temp = GUI_FIRST;

        while (temp->next != NULL) {
            // traverse to the end
            temp = temp->next;
        }
        temp->next = mid;
        mid->next = NULL;
        return mid;
    }
}


/**
 * Removes a gui element from the list.
 * The reference for the GUI element is removed from the list and
 * memory is freed.
 *
 * @param    name    name of the GUI element
 *
 * @note     if 'gui_struct' is non NULL, then the memory allocated to
 *           is freed by this rotuine.
 *
 */

int RmGuiElem(char* name) {
    gui_elem_ptr temp1, temp2;

    temp1 = temp2 = GUI_FIRST;         // start from the head of the list

    while (temp1 != NULL) {
        if (strcmp(temp1->name, name) == 0) {
            if (temp1 == GUI_FIRST) {
                GUI_FIRST = temp1->next;
            } else {
                temp2->next = temp1->next;    // rearrange the pointers
            }
            if (temp1->gui_struct != NULL) {
                free(temp1->gui_struct);     // free memory allocated to gui_struct
            }
            free(temp1);
            return ERR_SUCCESS;
        }
        temp2 = temp1;
        temp1 = temp1->next;
    }
    return ERR_ERROR;     // element not found, return error
}

/**
* This function adds a button element to the list with location (x,y)
* and a name.
*
* @param[in]    id     id of the button
* @param[in]   name    name of the button
* @param[in]  caption  caption of the button
* @param[in]     x     The x axis of the button
* @param[in]     y     The y axis of the button
*
* @return       ERR_ERRROR on error and ERR_SUCCESS on sucess
*
*/

int AddButton(uint32_t id, char* name, char* caption, char* cb_word, int x, int y) {
    gui_elem_ptr gui_ptr;
    btn         *temp_btn;

    gui_ptr=AddGuiElem(id, name, x, y, BUTTON);
    if (gui_ptr == NULL) {
        return ERR_ERROR;
    }

    temp_btn = (btn*)malloc(sizeof(btn));

    if (temp_btn == NULL) {
        RmGuiElem(name);
        return ERR_ERROR;
    }
    strcpy(temp_btn->caption, name);
    strcpy (temp_btn->call_back_word, cb_word);
    gui_ptr->height = BTN_HT;
    gui_ptr->width  = BTN_WT;
    gui_ptr->gui_struct = (void*)temp_btn;      // store the info related to btn
    return ERR_SUCCESS;
}

/**
 * Adds progress bar to the list.
 *
 * @param     id     id of P BAR
 * @param    name    name of this progress bar
 * @param     x      x axis location
 * @param     y      y axis location
 *
 * @return   ERR_SUCCESS on success and ERR_ERROR on error
 *
 */

int AddPBar(uint32_t id, char* name, int x, int y) {
    gui_elem_ptr gui_ptr;
    p_bar        *prog_bar;

    gui_ptr = AddGuiElem(id, name, x, y, PROGRESS);
    if (gui_ptr == NULL) {
        return ERR_ERROR;
    }

    gui_ptr->width = P_BAR_WT;
    gui_ptr->height = P_BAR_HT;

    prog_bar = (p_bar*)malloc(sizeof(p_bar));
    if (prog_bar == NULL) {
        RmGuiElem(name);
        return ERR_ERROR;
    }

    prog_bar->created = FALSE;
    gui_ptr->gui_struct = (void*)prog_bar;
    return ERR_SUCCESS;
}

/**
 * Adds a static text to the GUI controls list.
 * You can change the foreground, background text or text of this
 * control as required.
 *
 * @param      id      id of the static text control
 * @param     name     name of the GUI control
 * @param      x       x axis location
 * @param      y       y axis location
 * @param     text     text of of the static text control
 * @param    f_color   foreground text color
 * @param    b_color   background text color
 *
 * @return   ERR_ERROR on error and ERR_SUCCESS on success
 *
 */

int AddStText (uint32_t id, char *name, uint32_t x, uint32_t y, char* text,
               uint32_t f_color, uint32_t b_color) {
    gui_elem_ptr   gui_ptr;
    static_text    *st_txt;

    gui_ptr = AddGuiElem(id, name, x, y, TEXT);
    if (gui_ptr == NULL) {
        return ERR_ERROR;
    }

    st_txt = (static_text*)malloc(sizeof(static_text));

    if (st_txt == NULL) {
        RmGuiElem(name);
        return ERR_ERROR;
    }
    st_txt->prev_len = 0;
    st_txt->f_color = f_color;
    st_txt->b_color = b_color;
    strcpy(st_txt->txt, text);
    gui_ptr->gui_struct = (void*)st_txt;
    return ERR_SUCCESS;
}


/**
 * Given an id, this function returns the GUI struct of the id.
 * Starting from the FIRST_GUI pointer, this function looks out
 * for the element with the given id. Ideally, ids must be unique but
 * if there are two or more elements with the same id, then the first
 * element with given id is returned.
 *
 * @param     id    id of the element
 *
 * @return    Pointer to the gui element if found or else NULL pointer
 *
 */

gui_elem_ptr FindGuiElem(uint32_t id) {
    gui_elem_ptr temp;

    temp = GUI_FIRST;

    while (temp != NULL) {
        if (temp->id == id) {
            return temp;
        }
        temp = temp->next;
    }

    return NULL;
}


/**
 * Controls the progress bar with given id.
 * This function takes in the id and finds the element in GUI list and
 * sets the appropriate members of the struct updates the display
 *
 * @param     id     id of the progress bar
 * @param     vol    the volume with which the progress bar is to b set
 *
 * @return    ERR_SUCCESS on success and ERR_ERROR on error
 *
 */

int SetPBar(uint32_t id, int vol) {
    gui_elem_ptr temp;
    struct p_bar        *prog_bar;

    temp = FindGuiElem(id);

    if (temp == NULL) {
        return ERR_ERROR;
    }

    if (temp->gui_type != PROGRESS) {
        return ERR_ERROR;         // not a progress bar
    }

    prog_bar = (p_bar*)(temp->gui_struct);

    if (vol >= 0) {
        prog_bar->vol = (uint32_t)vol;  // if positive, update volume
        GuiProgressBar(temp->x, temp->y, temp->width, temp->height, prog_bar);
    } else {
        // else, redraw the progress bar
        prog_bar->created = FALSE;
        GuiProgressBar(temp->x, temp->y, temp->width, temp->height, prog_bar);
        prog_bar->created = TRUE;
    }

    return ERR_SUCCESS;
}

/**
 * This function sets the text for static text control.
 * This function also updates the LCD with the new text
 *
 * @param     id      id of the static text box
 * @param    name     name of the control
 * @param    text     The new text you want to set
 *
 * @return   ERR_SUCCESS on success and ERR_ERRROR on error
 */

int SetStText(uint32_t id, char* text) {
    gui_elem_ptr temp;
    static_text* st_txt;

    temp = FindGuiElem(id);

    if (temp == NULL) {
        return ERR_ERROR;
    }

    if (temp->gui_type != TEXT) {
        return ERR_ERROR;
    }
    st_txt = (static_text*)temp->gui_struct;
    strcpy(st_txt->txt, text);
    GuiStaticText(temp->x, temp->y, st_txt);

    return ERR_SUCCESS;
}

/**
 * This function sets the foreground and background color for the text
 * control. The control is redrawn with the new color settings.
 *
 * @param     id      id of the static text
 * @param     fg      foreground color
 * @param     bg      backgroun color
 *
 * @return   ERR_SUCCESS on succcess ERR_ERROR if element not found
 */

int SetStColor(uint32_t id, uint32_t fg, uint32_t bg) {
    gui_elem_ptr temp;
    static_text* st_txt;

    temp = FindGuiElem(id);

    if (temp == NULL) {
        return ERR_ERROR;
    }

    if (temp->gui_type != TEXT) {
        return ERR_ERROR;
    }
    st_txt = (static_text*)temp->gui_struct;
    st_txt->f_color = fg;
    st_txt->b_color = bg;

    GuiStaticText(temp->x, temp->y, st_txt);
    return ERR_SUCCESS;
}




/**
 * This function traverses the list and lays out the GUI elements.
 *
 */

void DrawControls(void) {
    gui_elem_ptr  temp;

    // clear the LCD display
    GuiClear();

    temp = GUI_FIRST;

    while (temp != NULL) {
        if (temp->gui_type == BUTTON) {
            btn *btn_ptr;
            btn_ptr = (btn*)(temp->gui_struct);
            GuiButton(temp->x, temp->y, temp->width, temp->height, btn_ptr->caption, UNCLICKED);
        } else if (temp->gui_type == PROGRESS) {
            p_bar *p_bar_ptr;
            p_bar_ptr = (p_bar*)(temp->gui_struct);
            GuiProgressBar(temp->x, temp->y, temp->width, temp->height, p_bar_ptr);
            p_bar_ptr->created = TRUE;
        } else if (temp->gui_type == TEXT) {
            static_text *st_txt;
            st_txt = (static_text*)(temp->gui_struct);
            GuiStaticText(temp->x, temp->y, st_txt);

        } else if (temp->gui_type == BMP) {
            bmp_ctl *temp_bmp;
            temp_bmp = (bmp_ctl*)(temp->gui_struct);
            DrawBMP(temp->x, temp->y, temp_bmp->file_loc);
        }
        temp = temp->next;

    }
}

/**
* Adds a bit map control to the gui list
*
* @param    id          id of the bit map
* @param    name         name of the control
* @param     x           x axis location
* @param     y           y axis location
* @param    file_path    Path for locating the image on SD card
*
*  @return   ERR_SUCCESS on succcess ERR_ERROR on error
*
*/

int AddBMP(uint32_t id, char* name, uint32_t x, uint32_t y, char* file_path) {
    gui_elem_ptr   gui_ptr;
    bmp_ctl      *temp_bmp;

    gui_ptr = AddGuiElem(id, name, x, y, BMP);
    if (gui_ptr == NULL) {
        return ERR_ERROR;
    }

    temp_bmp = (bmp_ctl*)malloc(sizeof(bmp_ctl));

    if (temp_bmp == NULL) {
        RmGuiElem(name);
        return ERR_ERROR;
    }
    strcpy(temp_bmp->file_loc, file_path);
    // store the pointer
    gui_ptr->gui_struct = temp_bmp;
    return ERR_SUCCESS;
}

/**
*  This function sets an image for bitmap control.
*
* @param    id            id of the control
* @param    file_name     The new file location for fetching the bmp
*
* @return   ERR_SUCCESS on succcess ERR_ERROR on error(file not found, element not found
*
*/

int UpdateBMP(uint32_t id, char *file_name) {
    gui_elem_ptr   gui_ptr;
    bmp_ctl      *temp_bmp;

    gui_ptr = FindGuiElem(id);

    if (gui_ptr == NULL) {
        return ERR_ERROR;
    }

    if (gui_ptr->gui_type != BMP) {
        return ERR_ERROR;         // not BMP
    }
    temp_bmp = (bmp_ctl*)gui_ptr->gui_struct;
    strcpy(temp_bmp->file_loc, file_name);

    if (DrawBMP(gui_ptr->x, gui_ptr->y, temp_bmp->file_loc) == FILE_NOT_FOUND) {
        return ERR_ERROR;
    }
    //else
    return ERR_SUCCESS;
}


/**
 * This function dispatches a event to any control
 *
 * @param[out]   cb_wrd    on exit from this function this array will have call back
 *                         word if there was an event on any button
 *
 * @return   EVENT if there was an event on a button NO_EVENT if no event is pending
 */


int Dispatcher(char* cb_wrd, int *id) {
    gui_elem_ptr temp;
    ts_event evt;
    int a, b;

    // poll for an event to occur
    evt.x = -1;

    while (evt.x == -1) {
        get_evt(&evt);
    }

    temp = GUI_FIRST;

    while (temp  != NULL) {
        if (temp->gui_type == BUTTON) {
            a = temp->x+temp->height;
            b = temp->y+temp->width;
            if ((evt.x < a && evt.y < b) && (evt.x > temp->x &&  evt.y > temp->y) ) {
                //printf ("\rWe have an event on %s       ", temp->name);
                btn *btn_info = (btn*)temp->gui_struct;
                strcpy(cb_wrd, btn_info->call_back_word);
                *id = temp->id;
                GuiButton(temp->x, temp->y, temp->width, temp->height, btn_info->caption, CLICKED);          // for the click effect
                wait(0.2);
                GuiButton(temp->x, temp->y, temp->width, temp->height, btn_info->caption, UNCLICKED);
                return EVENT;
            }
        }
        temp = temp->next;
    }
    cb_wrd[0] = '\0';
    return NO_EVENT;
}


/**
* This function deletes all the elements in the GUI_LIST.
* This is the destructor for all the GUI elemnts.
*/

void RmGuiElemAll(void) {
    gui_elem_ptr temp, temp1;
    // clear the LCD display
    GuiClear();

    if (GUI_FIRST == NULL) {
        // nothing to delete no elements
        return;
    } else {
        temp = GUI_FIRST;

        while (temp != NULL) {
            temp1 = temp;
            temp = temp->next;
            // free the memory allocated to gui_struct
            if (temp1->gui_struct != NULL) {
                free(temp1->gui_struct);
            }
            // free the GUI node
            free(temp1);
        }

    }

    GUI_FIRST = NULL;
}

