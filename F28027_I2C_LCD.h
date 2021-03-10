//#############################################################################
//
//  File:   f28027_I2C_LCD.h
//
//  Title:  F28027 connect to LCD via I2C library header file
//
//  Created on: 2021Äê03ÔÂ09ÈÕ
//
//  Author: Jie Kang
//
//#############################################################################

#ifndef F28027_I2C_LCD_H_
#define F28027_I2C_LCD_H_

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80
// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00
// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00
// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00
// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00
// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00
#define En 0x04  // Enable bit
#define Rw 0x02  // Read/Write bit
#define Rs 0x01  // Register select bit
#define I2C_NUMBYTES  1 // Number of bytes to transfer

#include "DSP28x_Project.h"
#include "clk.h"
#include "flash.h"
#include "pie.h"
#include "pll.h"
#include "wdog.h"
#include "gpio.h"
// Link external handles for drivers
extern CLK_Handle myClk;
extern FLASH_Handle myFlash;
extern GPIO_Handle myGpio;
extern PIE_Handle myPie;
extern CPU_Handle myCpu;
extern PLL_Handle myPll;
extern WDOG_Handle myWDog;

uint8_t _Addr;
uint8_t _displayfunction;
uint8_t _displaycontrol;
uint8_t _displaymode;
uint8_t _numlines;
uint8_t _cols;
uint8_t _rows;
uint8_t _backlightval;

void DELAY_MS(uint16_t _ms);
void init_I2C(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows);
void init_LCD(void);
void begin(uint8_t cols, uint8_t rows, uint8_t dotsize);
void command(uint8_t);
void send(uint8_t, uint8_t);
void write(uint8_t);
void writeNum(uint8_t);
void writeStr(const char *str);
void write4bits(uint8_t);
void expanderWrite(uint8_t);
void pulseEnable(uint8_t);
void clear();
void home();
void noDisplay();
void display();
void noBlink();
void blink();
void noCursor();
void cursor();
void scrollDisplayLeft();
void scrollDisplayRight();
void leftToRight();
void rightToLeft();
void noBacklight();
void backlight();
void autoscroll();
void noAutoscroll();
void setCursor(uint8_t, uint8_t);

#endif /* F28027_I2C_LCD_H_ */
