//#############################################################################
//
//  File:   f28027_I2C_LCD.h
//
//  Title:  F28027 connect to LCD via I2C library source file
//
//  Created on: 2021Äê03ÔÂ09ÈÕ
//
//  Author: Jie Kang
//
//#############################################################################

#include <F28027_I2C_LCD.h>

void DELAY_MS(uint16_t _ms)
{
    uint16_t A = _ms * 1000;
    DELAY_US(A);
}

void init_I2C(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows)
{
  _Addr = lcd_Addr;
  _cols = lcd_cols;
  _rows = lcd_rows;
  _backlightval = LCD_NOBACKLIGHT;
}

void init_LCD(void)
{
    GPIO_setPullUp(myGpio, GPIO_Number_32, GPIO_PullUp_Enable);
    GPIO_setPullUp(myGpio, GPIO_Number_33, GPIO_PullUp_Enable);
    GPIO_setQualification(myGpio, GPIO_Number_32, GPIO_Qual_ASync);
    GPIO_setQualification(myGpio, GPIO_Number_33, GPIO_Qual_ASync);
    GPIO_setMode(myGpio, GPIO_Number_32, GPIO_32_Mode_SDAA);
    GPIO_setMode(myGpio, GPIO_Number_33, GPIO_33_Mode_SCLA);
    CLK_enableI2cClock(myClk);
    I2caRegs.I2CMDR.bit.IRS = 0; // set I2C module into reset state
    // I2CCLK(Fmod) = SYSCLK/(I2CPSC+1)
    #if (CPU_FRQ_40MHZ||CPU_FRQ_50MHZ)
      I2caRegs.I2CPSC.all = 4;       // Prescaler - need 7-12 Mhz on module clk
    #endif
    #if (CPU_FRQ_60MHZ)
      I2caRegs.I2CPSC.all = 5;       // Prescaler - need 7-12 Mhz on module clk
    #endif
    // MSTCLK(Tmst) = [(ICCH+d)+(ICCL+d)]/(Fmod)
    I2caRegs.I2CCLKL = 45;           // NOTE: must be non zero
    I2caRegs.I2CCLKH = 45;            // NOTE: must be non zero
    I2caRegs.I2CMDR.bit.IRS = 1; // set I2C module out of reset state
    while (I2caRegs.I2CMDR.bit.STP != 0);    // wait for STOP condition

    _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    begin(_cols, _rows, LCD_5x8DOTS);
}

void begin(uint8_t cols, uint8_t lines, uint8_t dotsize)
{
    if (lines > 1) {
        _displayfunction |= LCD_2LINE;
    }
    _numlines = lines;

    // for some 1 line displays you can select a 10 pixel high font
    if ((dotsize != 0) && (lines == 1)) {
        _displayfunction |= LCD_5x10DOTS;
    }
    DELAY_MS(50);
    expanderWrite(_backlightval); // reset expanderand turn backlight off (Bit 8 =1)
    DELAY_MS(1000);
    write4bits(0x03 << 4);
    DELAY_US(4500);
    write4bits(0x03 << 4);
    DELAY_US(4500);
    write4bits(0x03 << 4);
    DELAY_US(150);
    write4bits(0x02 << 4); // finally, set to 4-bit interface 0010 0000
    command(LCD_FUNCTIONSET | _displayfunction);
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();
    clear();
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
    home();
}

void command(uint8_t value)
{
    send(value, 0);
}

void send(uint8_t value, uint8_t mode)
{
    uint8_t highnib = value & 0xf0;
    uint8_t lownib = (value<<4) & 0xf0;
    write4bits((highnib)|mode);
    write4bits((lownib)|mode);
}

void write(uint8_t value)
{
    send(value, Rs);
}
void writeNum(uint8_t value)
{
    value = value + 48;
    send(value, Rs);
}

void writeStr(const char *str)
{
    while(*str){
    send(*str, Rs);
    str++;
    }
}

void write4bits(uint8_t value)
{
    expanderWrite(value);
    pulseEnable(value);
}

void expanderWrite(uint8_t _data)
{
    while (I2caRegs.I2CMDR.bit.STP == 1);  // wait for STOP condition
    I2caRegs.I2CSAR = _Addr;     // load slave address
    while (I2caRegs.I2CSTR.bit.BB == 1);// Check if bus busy
    I2caRegs.I2CCNT = I2C_NUMBYTES;     // Setup number of bytes to send
    I2caRegs.I2CDXR = (_data | _backlightval);     // load data into the transmit register
    I2caRegs.I2CMDR.all = 0x6E20; // Send start as master transmitter
}

void pulseEnable(uint8_t _data)
{
    expanderWrite(_data | En);  // En high 0000 0100
    DELAY_US(1);       // enable pulse must be >450ns

    expanderWrite(_data & ~En); // En low 1111 1011
    DELAY_US(50);      // commands need > 37us to settle
}

void clear(void)
{
    command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
    DELAY_US(2000);  // this command takes a long time!
}

void home(void)
{
    command(LCD_RETURNHOME);  // set cursor position to zero
    DELAY_US(2000);  // this command takes a long time!
}

void noDisplay(void)
{
    _displaycontrol &= ~LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void display(void)
{
    _displaycontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void noBlink(void)
{
    _displaycontrol &= ~LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void blink(void)
{
    _displaycontrol |= LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void noCursor(void)
{
    _displaycontrol &= ~LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void cursor(void)
{
    _displaycontrol |= LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void scrollDisplayLeft(void)
{
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void scrollDisplayRight(void)
{
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void leftToRight(void)
{
    _displaymode |= LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void rightToLeft(void)
{
    _displaymode &= ~LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// Turn the (optional) backlight off/on
void noBacklight(void)
{
    _backlightval=LCD_NOBACKLIGHT;
    expanderWrite(0);
}

void backlight(void)
{
    _backlightval=LCD_BACKLIGHT;
    expanderWrite(0);
}

// This will 'right justify' text from the cursor
void autoscroll(void) {
    _displaymode |= LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void noAutoscroll(void) {
    _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

void setCursor(uint8_t col, uint8_t row)
{
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if ( row > _numlines ) {
        row = _numlines-1;    // we count rows starting w/0
    }
    command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}
