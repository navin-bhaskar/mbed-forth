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
#include "english_16x8.h"
#include "lcd.h"



//int color_table[] ={BLACK, WHITE, RED, GREEN, BLUE, YELLOW, MAGENTA, ORANGE, CYAN, GRAY, SILVER, GOLD};

#define ID_AM         0111


PortOut LCD_PORT1(Port2,0x3F);
PortOut LCD_PORT2(Port1,0xC0000000);

DigitalOut CS(p29);
DigitalOut RS(p28);
DigitalOut WR(p27);



#define  LCD_CS_H()              CS=1
#define  LCD_CS_L()              CS=0
#define  LCD_RS_H()              RS=1
#define  LCD_RS_L()              RS=0
#define  LCD_WR_H()              WR=1
#define  LCD_WR_L()              WR=0



void delay_us(unsigned int time) {
    while (time--);
}

void delay_ms(unsigned int time) {
    while (time--) delay_us(100);
}

void LCD_WR_DATA16(unsigned int data) {
    LCD_RS_H();
    LCD_CS_L();
    LCD_WR_L();

    LCD_PORT1=(data>>8)&0x3F;
    LCD_PORT2=((data)&0xC000)<<16;
    LCD_WR_H();
    LCD_WR_L();
    LCD_PORT1=(data)&0x3F;
    LCD_PORT2=((data)&0xC0)<<24;
    LCD_WR_H();
    LCD_CS_H();

}
void LCD_WR_REG16(unsigned int index) {
    LCD_RS_L();
    LCD_CS_L();
    LCD_WR_L();
    LCD_PORT1=0x00;
    LCD_PORT2=0x00;
    LCD_WR_H();
    LCD_WR_L();
    LCD_PORT1=(index)&0x3F;
    LCD_PORT2=(index<<24)&0xC0000000;
    LCD_WR_H();
    LCD_CS_H();
}

void LCD_WR_REG(unsigned int index,unsigned int data) {
    LCD_WR_REG16(index);
    LCD_WR_DATA16(data);
}

void LCD_Clear(unsigned int Color) {
    unsigned long index = 0;

    LCD_WR_REG(0x0020,0x0000);
    LCD_WR_REG(0x0021,00000);

    LCD_WR_REG(0x0050,0x00);
    LCD_WR_REG(0x0051,239);
    LCD_WR_REG(0x0052,0x00);
    LCD_WR_REG(0x0053,319);
    LCD_WR_REG16(0x0022);

    LCD_RS_H();
    LCD_CS_L();
    for (index = 0; index < 76800; index++) {
        LCD_WR_L();

        LCD_PORT1=(Color>>8)&0x3F;
        LCD_PORT2=((Color)&0xC000)<<16;
        LCD_WR_H();
        LCD_WR_L();
        LCD_PORT1=(Color)&0x3F;
        LCD_PORT2=((Color)&0xC0)<<24;
        LCD_WR_H();


    }
    LCD_CS_H();
}

void LCD_Init(void) {

    delay_ms(100);

    delay_ms(100);
    //initializing funciton 1
    LCD_WR_REG(0x00E3,0x3008);
    LCD_WR_REG(0x00E7,0x0012);
    LCD_WR_REG(0x00Ef,0x1231);     // Set the internal timing;
    LCD_WR_REG(0x00,0x0001);
    LCD_WR_REG(0x01, 0x0000); // set SS and SM bit
    LCD_WR_REG(0x02, 0x0700); // set 1 line inversion
    // LCD_WR_REG(0x03, 0x10B0); // set GRAM write direction and BGR=1.

    LCD_WR_REG(0x0003,0x1028);

    LCD_WR_REG(0x04, 0x0000); // Resize register
    LCD_WR_REG(0x08, 0x0202); // set the back porch and front porch
    LCD_WR_REG(0x09, 0x0000); // set non-display area refresh cycle ISC[3:0]
    LCD_WR_REG(0x0A, 0x0000); // FMARK function
    // LCD_WR_REG(0x0C, 0x0000); // RGB interface setting
    // LCD_WR_REG(0x0D, 0x0000); // Frame marker Position
    // LCD_WR_REG(0x0F, 0x0000); // RGB interface polarity
//Power On sequence //
    LCD_WR_REG(0x10, 0x0080); // SAP, BT[3:0], AP, DSTB, SLP, STB
    LCD_WR_REG(0x11, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
    LCD_WR_REG(0x12, 0x0000); // VREG1OUT voltage
    LCD_WR_REG(0x13, 0x0000); // VDV[4:0] for VCOM amplitude
    LCD_WR_REG(0x07, 0x0001); //

    delay_ms(2000); // Dis-charge capacitor power voltage

    LCD_WR_REG(0x10, 0x1290); // SAP, BT[3:0], AP, DSTB, SLP, STB
    LCD_WR_REG(0x11, 0x0527); // DC1[2:0], DC0[2:0], VC[2:0]
    delay_ms(1000); // Delay 50ms

    LCD_WR_REG(0x12, 0x0017); // Internal reference voltage= Vci;
    delay_ms(1000); // Delay 50ms
    LCD_WR_REG(0x13, 0x1200); // Set VDV[4:0] for VCOM amplitude
    LCD_WR_REG(0x29, 0x001f); // Set VCM[5:0] for VCOMH
    LCD_WR_REG(0x2B, 0x000D); // Set Frame Rate
    delay_ms(500); // Delay 50ms

#if   ID_AM==000
    LCD_WR_REG(0x0020,0x00ef);//GRAM
    LCD_WR_REG(0x0021,0x013f);
#elif ID_AM==001
    LCD_WR_REG(0x0020,0x00ef);
    LCD_WR_REG(0x0021,0x013f);
#elif ID_AM==010
    LCD_WR_REG(0x0020,0x0000);
    LCD_WR_REG(0x0021,0x013f);
#elif ID_AM==011
    LCD_WR_REG(0x0020,0x0000);
    LCD_WR_REG(0x0021,0x013f);
#elif ID_AM==100
    LCD_WR_REG(0x0020,0x00ef);
    LCD_WR_REG(0x0021,0x0000);
#elif ID_AM==101
    LCD_WR_REG(0x0020,0x00ef);
    LCD_WR_REG(0x0021,0x0000);
#elif ID_AM==110
    LCD_WR_REG(0x0020,0x0000);
    LCD_WR_REG(0x0021,0x0000);
#elif ID_AM==111
    LCD_WR_REG(0x0020,0x0000);
    LCD_WR_REG(0x0021,0x0000);
#endif
// ----------- Adjust the Gamma Curve ----------//
    LCD_WR_REG(0x0030,0x0107);
    LCD_WR_REG(0x0031,0x0306);
    LCD_WR_REG(0x0032,0x0207);
    LCD_WR_REG(0x0035,0x0206);
    LCD_WR_REG(0x0036,0x0408);
    LCD_WR_REG(0x0037,0x0106);
    LCD_WR_REG(0x0038,0x0102);
    LCD_WR_REG(0x0039,0x0207);
    LCD_WR_REG(0x003C,0x0504);
    LCD_WR_REG(0x003D,0x0503);



//------------------ Set GRAM area ---------------//
    LCD_WR_REG(0x0050, 0x0000); // Horizontal GRAM Start Address
    LCD_WR_REG(0x0051, 0x00EF); // Horizontal GRAM End Address
    LCD_WR_REG(0x0052, 0x0000); // Vertical GRAM Start Address
    LCD_WR_REG(0x0053, 0x013F); // Vertical GRAM Start Address
    LCD_WR_REG(0x0060, 0x2700); // Gate Scan Line
    LCD_WR_REG(0x0061, 0x0001); // NDL,VLE, REV
    LCD_WR_REG(0x006A, 0x0000); // set scrolling line
//-------------- Partial Display Control ---------//
    LCD_WR_REG(0x80, 0x0000);
    LCD_WR_REG(0x81, 0x0000);
    LCD_WR_REG(0x82, 0x0000);
    LCD_WR_REG(0x83, 0x0000);
    LCD_WR_REG(0x84, 0x0000);
    LCD_WR_REG(0x85, 0x0000);
//-------------- Panel Control -------------------//
    LCD_WR_REG(0x90, 0x0010);
    LCD_WR_REG(0x92, 0x0600);
    LCD_WR_REG(0x93, 0x0003);
    LCD_WR_REG(0x95, 0x0110);
    LCD_WR_REG(0x97, 0x0000);
    LCD_WR_REG(0x98, 0x0000);

    LCD_WR_REG(0x07, 0x0173); // 262K color and display ON   0133
    LCD_Clear(0);
    delay_ms(2000); // Delay 5ms



}





void LCD_SetCursor(unsigned int Xpos, unsigned int Ypos) {
// CLR_RS;
    LCD_WR_REG(0x20, Xpos);
    LCD_WR_REG(0x21, Ypos);
}

void LCD_SetBox(unsigned int xStart,unsigned int yStart,unsigned int xLong,unsigned int yLong) {

    LCD_SetCursor(xStart,yStart);

    LCD_WR_REG(0x0050,xStart);
    LCD_WR_REG(0x0051,xStart+xLong-1);
    LCD_WR_REG(0x0052,yStart);
    LCD_WR_REG(0x0053,yStart+yLong-1);
}



void LCD_write_english(unsigned char data,unsigned int color,unsigned int xcolor) {

    unsigned char avl,i,n;
    LCD_WR_REG16(0x0022);
    for (i=0;i<16;i++) {
        avl=(english[data-32][i]);
        for (n=0;n<8;n++) {
            if (avl&0x80) {
                LCD_WR_DATA16(color);
            } else {
                LCD_WR_DATA16(xcolor);
            }
            avl<<=1;
        }
    }
}
void LCD_write_string(unsigned int x,unsigned int y,unsigned char *s,unsigned int color,unsigned int xcolor) {
    unsigned int k=0;
    while (*s) {
        LCD_SetBox(x,y+k,15,8);
        LCD_write_english( *s,color,xcolor);
        k=k+8;
        s++;

    }

}

void LCD_write_logo(unsigned char data,unsigned int color,unsigned int xcolor) {

    unsigned char avl,i,j,n;
    LCD_WR_REG16(0x0022);
    for (i=0;i<16;i++) {
        for (j=0;j<2;j++) {
            if (data<=9)
                avl=(english[data+16][i]);
            else
                avl=(english[data-32][i]);
            for (n=0;n<8;n++) {
                if (avl&0x80) {
                    LCD_WR_DATA16(xcolor);
                    LCD_WR_DATA16(xcolor);
                    LCD_WR_DATA16(xcolor);
                    LCD_WR_DATA16(xcolor);

                } else {
                    LCD_WR_DATA16(color);
                    LCD_WR_DATA16(color);
                    LCD_WR_DATA16(color);
                    LCD_WR_DATA16(color);
                }
                avl<<=1;
            }
        }
    }
}


void LCD_write_logo_string(unsigned int x,unsigned int y,unsigned char *s,unsigned int color,unsigned int xcolor) {
    unsigned int k=0;
    while (*s) {
        LCD_SetBox(x,y+k,16,31);
        LCD_write_logo( *s,color,xcolor);
        k=k+16;
        s++;

    }

}
void LCD_write_SUM(unsigned int x,unsigned int y,unsigned char SUM,unsigned int color,unsigned int xcolor) {

    unsigned char avl,i,n;
    LCD_SetBox(y,x,15,8);
    LCD_WR_REG16(0x0022);
    for (i=0;i<16;i++) {
        avl=( english[SUM+16][i]);
        for (n=0;n<8;n++) {
            if (avl&0x80) LCD_WR_DATA16(color);
            else LCD_WR_DATA16(xcolor);

            avl<<=1;
        }
    }
}



void  LCD_Draw_Point(unsigned int x, unsigned int y,unsigned int color) {
    LCD_WR_REG(0x0020,x);
    LCD_WR_REG(0x0021,y);

    LCD_WR_REG16(0x0022);
    LCD_WR_DATA16(color);



}




void LCD_Draw_Line(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, int color) {
    int dy = y1 - y0;
    int dx = x1 - x0;
    int stepx, stepy;

    if (dy < 0) {
        dy = -dy;
        stepy = -1;
    } else {
        stepy = 1;
    }
    if (dx < 0) {
        dx = -dx;
        stepx = -1;
    } else {
        stepx = 1;
    }
    dy <<= 1;   // dy is now 2*dy
    dx <<= 1;   // dx is now 2*dx

    LCD_Draw_Point(x0, y0, color);

    if (dx > dy) {
        int fraction = dy - (dx >> 1); // same as 2*dy - dx
        while (x0 != x1) {
            if (fraction >= 0) {
                y0 += stepy;
                fraction -= dx; // same as fraction -= 2*dx
            }
            x0 += stepx;
            fraction += dy;  // same as fraction -= 2*dy
            LCD_Draw_Point(x0, y0, color);
        }
    } else {
        int fraction = dx - (dy >> 1);
        while (y0 != y1) {
            if (fraction >= 0) {
                x0 += stepx;
                fraction -= dy;
            }
            y0 += stepy;
            fraction += dx;
            LCD_Draw_Point(x0, y0, color);
        }
    }
}


void LCD_Draw_Rect(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, char fill, int color) {
    unsigned int xmin, xmax, ymin, ymax;
    unsigned int i;

    if (fill == FILL) {
        xmin = (x0 <= x1) ? x0 : x1;
        xmax = (x0 > x1) ? x0 : x1;
        ymin = (y0 <= y1) ? y0 : y1;
        ymax = (y0 > y1) ? y0 : y1;

        LCD_SetBox(xmin,ymin,xmax-xmin,ymax-ymin);
        LCD_WR_REG16(0x0022);
        //((xmax-xmin+1)*(ymax-ymin+1))
        for (i = 0; i < ((xmax-xmin)*(ymax-ymin)); i++) {
            LCD_WR_DATA16(color);
        }
    } else {
        LCD_Draw_Line(x0 , y0, x1, y0, color);
        LCD_Draw_Line(x0 , y1, x1, y1, color);
        LCD_Draw_Line(x0, y0, x0, y1, color);
        LCD_Draw_Line(x1, y0, x1, y1, color);
    }
}




