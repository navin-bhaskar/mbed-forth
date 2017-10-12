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
*  @file    forth_files.c
*  @brief   This file contains functions realted to handling of Forth files stored
*           in a SD card
*/

#include "forth_files_int.h"

SDFileSystem sd(p5, p6, p7, p9, "sd");

extern int CmdPos;
extern char CmdBuff[];


/**
*  Given a file name, this function loads the forth code found in the file
*  and executed it.
*
*  @param    file_name    name of the Forth script
*  @param       x         x location for the bit map
*  @param       y         y location for the bit map
*
*  @return   EXECUTION_ERROR on execution error, FILE_NOT_FOUND if file could not be located,
*            EXECUTION_COMPLETE if execution was completed
*/

int ExecFromFile(char* file_path) {
    char err_flag=FALSE;
    char file_name[60];
    int saved_CmdPos, res;
    char saved_CmdBuff[BUFFER_SIZE];
    FILE* fp;
    char abs_file_name[60];                // absolute file name

    stop_TS();

    // Remove spaces from file name that can cause problems
    RemoveSpaces(file_name, file_path);

    strcpy(abs_file_name, "/sd/");
    strcat(abs_file_name, file_name);
    printf ("Executing from file %s \n", file_name);

    fp = fopen(abs_file_name, "r");

    if (fp == NULL) {
        printf ("Could not open file \n");
        return FILE_NOT_FOUND;
    }

    saved_CmdPos = CmdPos;
    strcpy(saved_CmdBuff, CmdBuff);

    CmdPos = 0;                        // reset command buffer

    while (feof(fp)==0) {
        stop_TS();                      // do not curropt the SPI bus
        fgets(CmdBuff, BUFFER_SIZE, fp);
        start_TS();
        printf ("%s", CmdBuff);
        ToUp(CmdBuff);

        res = CONTINUE_FORTH_INTERPRET;

        while (CmdBuff[CmdPos] != '\0') {
            res = Interpret();
            if (res == COMPILE_ERROR || res == STOP_FORTH_INTERPRET ) {
                err_flag = TRUE;
                break;
            }
        }

        RESET_CMDPOS;
        if ( res != CONTINUE_FORTH_COMPILE) {
            //printf ( " OK \n");

        }

    }
    fclose(fp);
    CmdPos = saved_CmdPos;
    strcpy(CmdBuff, saved_CmdBuff);

    if (err_flag == TRUE) {
        return EXECUTION_ERROR;
    } else {
        return EXECUTION_COMPLETE;
    }

}

/**
* This function draws a bitmap image on LCD.
*
* @param     file_name      Image location of the file
*
* @return    FILE_NOT_FOUND on error or else FILE_FOUND on success
*/

int DrawBMP(int x, int y, char *file_name) {
    char abs_path[50] = "/sd/";
    unsigned int i;
    int height, width;
    unsigned short temp;
    int r,g,b;
    r = g = b = 0;
    stop_TS();

    strcat(abs_path, file_name);
    FILE *fp = fopen(abs_path, "r");
    if (fp == NULL) {
        start_TS();
        return FILE_NOT_FOUND;
    } else {
        // get height and width of the bit map
        fseek(fp, 16, SEEK_SET);
        width = 0;
        char ch;
        for (i=0; i<2; i++) {
            ch=fgetc(fp);
            width = ch;
            ch=fgetc(fp);
            width = (ch << 8)| width;
        }
        height = 0;
        for (i=0; i<2; i++) {
            ch=fgetc(fp);
            height = ch;
            ch=fgetc(fp);
            height = ch << 8| height;
        }


        fseek(fp, 0x36, SEEK_SET);
        LCD_WR_REG(0x0003,0x1038);  // set entry mode


        LCD_WR_REG(0x0020,0x0000);
        LCD_WR_REG(0x0021,00000);


        LCD_SetBox(x, y, height, width);   // select the area for drawing


        LCD_WR_REG16(0x0022);


        for (i=0;r!=EOF;i++) {
            b=fgetc(fp);
            g=fgetc(fp);
            r=fgetc(fp);
            temp=RGB(r,g,b);
            LCD_WR_DATA16(temp);
        }

        LCD_WR_REG(0x0003,0x1028);      // restore the Entry mode so that fonts behave as expected
        LCD_WR_REG(0x0020,0x0000);
        LCD_WR_REG(0x0021,0x0000);
        start_TS();
        return FILE_FOUND;
    }
}


/**
* This function looks out for the init script in the SD card
* and if it finds one, the script will be executed.
* The init script consists of Forth scripts that can be loaded
* into the system.
*
* @return FILE_NOT_FOUND if file could not be located or state of the
*         execution of words from the file as specified in \a INIT_FILE
*         file
*/

int InitExec(void) {
    FILE *fp=NULL;
    char script[MAX_EXEC_FILES];
    char buff[90]="Executing from file ", temp[40];
    int ret;

    stop_TS();
    // try and open the init file
    fp = fopen(INIT_FILE, "r");

    if (fp == NULL) {
        return FILE_NOT_FOUND;
    }

    // get the files


    fgets(temp, MAX_FILE_NAME, fp);
    if (buff[0] == '\r' || buff[0] == '\n' || buff[0] == '\0') {
        return FILE_NOT_FOUND;
    }
    RemoveSpaces(script, temp);
    strcat(buff, script);
    strcat(buff, " ...");
    LCD_write_string(10, 20, (unsigned char*)buff, RED, WHITE);
    wait(1);
    LCD_Clear(WHITE);
    ret = ExecFromFile(script);
    if (ret == FILE_NOT_FOUND) {
        printf ("\nCould not locate file %s \n", buff);
        LCD_Clear(RED);
        return FILE_NOT_FOUND;
    }

    return ret;                      // let the main know error conditions in script if any
}



