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

#ifndef        _LCD_H_
#define        _LCD_H_


#define BLACK                       RGB(0x00, 0x00, 0x00)
#define WHITE                       RGB(0xFF, 0xFF, 0xFF)
#define RED                         RGB(0xFF, 0x00, 0x00)
#define GREEN                       RGB(0x00, 0xFF, 0x00)
#define BLUE                        RGB(0x00, 0x00, 0xFF)
#define YELLOW                      RGB(0xFF, 0xFF, 0x00)
#define MAGENTA                     RGB(0xFF, 0x00, 0xFF)
#define ORANGE                      RGB(0xFF, 0x99, 0x00)
#define CYAN                        RGB(0x00, 0xFF, 0xFF)
#define GRAY                        RGB(0x80, 0x80, 0x40)
#define SILVER                      RGB(0xA0, 0xA0, 0x80)
#define GOLD                        RGB(0xA0, 0xA0, 0x40)




#define RGB(red, green, blue)    ((unsigned int)( (( red >> 3 ) << 11 ) | \
                                (( green >> 2 ) << 5  ) | \
                                ( blue  >> 3 )))




#define  FILL            1
#define  NO_FILL         0


void delay_us(unsigned int time);
void delay_ms(unsigned int time);
void LCD_WR_DATA16(unsigned int data);
void LCD_WR_REG16(unsigned int index);
void LCD_WR_REG(unsigned int index,unsigned int data);
void LCD_Clear(unsigned int Color);
void LCD_Init(void);
void LCD_SetCursor(unsigned int Xpos, unsigned int Ypos);
void LCD_SetBox(unsigned int xStart,unsigned int yStart,unsigned int xLong,unsigned int yLong);
void LCD_write_english(unsigned char data,unsigned int color,unsigned int xcolor);
void LCD_write_string(unsigned int x,unsigned int y,unsigned char *s,unsigned int color,unsigned int xcolor);
void LCD_write_logo(unsigned char data,unsigned int color,unsigned int xcolor);
void LCD_write_logo_string(unsigned int x,unsigned int y,unsigned char *s,unsigned int color,unsigned int xcolor);
void LCD_write_SUM(unsigned int x,unsigned int y,unsigned char SUM,unsigned int color,unsigned int xcolor);
void  LCD_Draw_Point(unsigned int x, unsigned int y,unsigned int color);
void LCD_Draw_Line(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, int color);
void LCD_Draw_Rect(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, char fill, int color);
#endif
