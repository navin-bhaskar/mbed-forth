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


#include "mbed.h"
#include "gui.h"
#include "lcd.h"
#include "ts.h"
#include "forthFunctions.h"
#include "interprter.h"
#include "utils.h"
#include "forth_files.h"





Serial pc(USBTX, USBRX);


extern int CmdPos;
extern char CmdBuff[];


DigitalOut myled(LED1);

/**
* This function simply reads the input strings from the console.
*
* @param      ip     buffer to hold the input string
* @param     size    The maximum number of bytes ip can hold
*
*/

void GetStr(char* ip, int size) {
    int i;
    i = 0;
    char temp;

    while (1) {
        if (pc.readable()) {
            temp = pc.getc();
            pc.putc(temp);
            if (temp == 0x0d) {
                break;
            }

            if (temp == 8 ) {  // back space
                if (i > 0) {
                    i--;
                }
                continue;
            }

            ip[i] = temp;
            i++;
        }
    }
    ip[i] = '\0';
}


int main() {
    char msg[] = "Reconfigurable computing \nmbed design challenge entry: NXP3878 \n";
    int res;
    init_dictionary();
    RESET_CMDPOS;
    printf (msg);
    LCD_Init();
    LCD_Clear(WHITE);
    TS_init();


    InitExec();

    while (1) {

        GetStr(CmdBuff, 50);
        ToUp(CmdBuff);

        res = CONTINUE_FORTH_INTERPRET;

        while (CmdBuff[CmdPos] != '\0') {
            res = Interpret();
            if (res == COMPILE_ERROR || res == STOP_FORTH_INTERPRET ) {
                break;
            }
        }

        RESET_CMDPOS;
        if ( res != CONTINUE_FORTH_COMPILE) {
            printf (" OK \r\n");
        }
    }
}
