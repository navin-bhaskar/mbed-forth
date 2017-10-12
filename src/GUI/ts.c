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
#include "lcd.h"
#include "ts_int.h"



ts_event event_table[MAX_EVTS];           /*< The events table which will hold the events data */
int evt_head;                              /*< Head pointer for evt table */
int evt_tail;                              /*< Tail pointer for evt table */


Ticker T_check;
Ticker bounce;
int ts_flag = 0;
SPI spi(p5, p6, p7); // mosi, miso, sclk
DigitalOut cs(p8);
DigitalIn PEN_IRQ(p30);



unsigned char xmin_offset=8,xmax_offset=116;
unsigned char ymin_offset=9,ymax_offset=113;


unsigned char TPReadX(void) {
    unsigned char temp=0;
    cs = 0;
    wait_us(10);
    spi.write(0x9C);
    wait_us(10);
    temp=spi.write(0x00);
    cs = 1;
    return (temp);
}

void set_bounce(void) {
    ts_flag = 0;
}

unsigned char TPReadY(void) {
    unsigned char temp=0;
    cs = 0;
    wait_us(10);
    spi.write(0xDC);
    wait_us(10);
    temp=spi.write(0x00);
    cs = 1;
    return (temp);
}

void TS_init(void) {
    spi.format(8,3);
    spi.frequency(100000);
    cs=1;
    T_check.attach(&PEN_IRQ_check,0.02);
    bounce.attach(&set_bounce, 01);
}

void touch_IRQ(void) {
    unsigned int y_axis=0,x_axis=0,i;
    if (ts_flag == 1) {
        return ;
    }
    for (i=0;i<10;i++) {
        x_axis+=TPReadX();
        y_axis+=TPReadY();
    }
    x_axis/=10;
    y_axis/=10;
    x_axis=(x_axis-xmin_offset+3)*320/(xmax_offset-xmin_offset);
    y_axis=(y_axis-ymin_offset)*240/(ymax_offset-ymin_offset);
    if (x_axis>320)
        x_axis=320;
    if (y_axis>240)
        y_axis=240;
    //LCD_Draw_Point(240-y_axis,320-x_axis,BLACK);
    add_evt(240-y_axis,320-x_axis);
    //printf("X = %d         Y = %d   ",240-y_axis, 320-x_axis);
    ts_flag = 1;

}

void PEN_IRQ_check(void) {
    if (!PEN_IRQ)
        touch_IRQ();
}


/**
 * This function pushes the event to the the event table
 *
 * @param     x    The x coordinate
 * @param     y    The y coordinate
 *
 * @note  The new values will overwrite the older ones
 */

void add_evt(int x, int y) {
    if (evt_head == MAX_EVTS - 1) {
        evt_head = 0;              // reset
    }
    event_table[evt_head].x = x;
    event_table[evt_head].y = y;
    evt_head++;
}

/**
 * This function returns the latest event on top of the event table
 *
 * @return    The members x,y of the struct will be filled with -1 if
 *            event table is empty or else the coordinate is placed
 *            in the member elements of the struct.
 */

void get_evt(ts_event *evt) {


    if (evt_head==evt_tail) {
        evt->x = -1;
        evt->y = -1;
        return;
    }

    if (evt_tail == MAX_EVTS - 1) {
        evt_tail = 0;
    }

    evt->x = event_table[evt_tail].x;
    evt->y = event_table[evt_tail].y;
    evt_tail++;

}

/**
* This function dettaches the touch screen interrupt
*/

void stop_TS(void) {
    T_check.detach();
}


/**
* This function attaches the touch screen deamon back
*/
void start_TS(void) {
    T_check.attach(&PEN_IRQ_check,0.02);
}

