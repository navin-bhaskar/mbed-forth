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
*  @file     utils.c
*  @brief    general utility functions
*/

#include "utils.h"


/**
* This function removs quotes from a string which are at the starting
* and ending of a string
*
*  str[inout]    str    input/output string
*
*/

void ReplaceQuotes(char *str) {
    int n;
    char buff[30];
    if (str[0] == '\"') {
        strcpy(buff, ++str);
        str--;
        strcpy(str, buff);
    }
    n = strlen(str);

    if (str[n-1] == '\"') {
        str[n-1] = '\0';
    }
}


/**
*  This function converts all characters in a string to upper case except for
*  quoted string
*
*  @param[inout]    str   input/output string
*
*/

void ToUp(char *str) {
    int i=0;
    bool SkipFlag = FALSE;

    while (str[i] != '\0') {
        if (str[i] == '\"') {
            if (SkipFlag == FALSE) {
                SkipFlag = TRUE;
            } else {
                SkipFlag = FALSE;
            }
        }

        if (SkipFlag == FALSE) {
            if (str[i] >= 'a' && str[i] <= 'z') {
                str[i] = str[i] - 32;     // a-A
            }
        }
        i++;
    }
}

/**
* This function simply returns the color value for given index
* @param   clr     color index
* @return 16-bit color value
*/

unsigned int ColorVal(int clr) {
    switch (clr) {
        default:
        case 0:
            return BLACK;
        case 1:
            return WHITE;
        case 2:
            return RED;
        case 3:
            return GREEN;
        case 4:
            return BLUE;
        case 5:
            return YELLOW;
        case 6:
            return MAGENTA;
        case 7:
            return ORANGE;
        case 8:
            return CYAN;
        case 9:
            return GRAY;
        case 10:
            return SILVER;
        case 11:
            return GOLD;
    }
}

/**
* This function removes any spaces that are at front or at
* the end of a string. No spaces allowed in the string
*
* @param[in]     src    The source string
* @param[out]    dest   The destination string which will have the
*                       stripped version of the string on exit from
*                       this function.
*
*/
void RemoveSpaces(char *dest, char *src) {
    int i, j;
    char space_flag;

    space_flag = 0;
    i = j = 0;


    while (src[i] != '\0') {
        if (src[i] == ' ' && space_flag == 0) {
            i++;
        } else if (src[i] == ' ' && space_flag == 1) {
            dest[j] = '\0';
            break;
        } else if (src[i] == 0x0d || src[i] == 0x0a) { // skip \r and \n
            i++;
            continue;
        } else {
            space_flag = 1;
            dest[j++] = src[i++];
        }
    }
    dest[j] = '\0';


}

/**
* This function pushes -1 to indicate errors
*/
void ErrorCond(int val) {
    int cond;
    PushDs(val, &cond);  // if stack is full, nothing can be done :(
}
