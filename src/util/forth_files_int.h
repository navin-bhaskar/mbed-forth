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


#ifndef __FORTH_FILES_INT_H
#define __FORTH_FILES_INT_H

#include "SDFileSystem.h"
#include "interprter.h"
#include "utils.h"
#include "forth_files.h"
#include "gui.h"

#define  INIT_FILE         "/sd/init"     /*< The init file name which should list the scripts to be loaded at boot time */
#define  MAX_EXEC_FILES        4          /*< Maximum files allowed in init file */

#define  FILE_NOT_FOUND         1     /*< Error code if file was not found */
#define  FILE_FOUND             2     /*< To indicate file found condition */
#define  MAX_FILE_NAME         20     /*< Maximum length of the file name string */

#define INIT_F_CLR              RED    /*< Foreground color for displaying the script file names */
#define INIT_B_CLR             WHITE   /*< Background color for displaying the scripts file name */

int ExecFromFile(char* file_name);

#endif
