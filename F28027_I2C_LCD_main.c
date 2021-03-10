//#############################################################################
//
//  File:   F28027_LCD_I2C_main.c
//
//  Title:  F28027 connect to LCD via I2C main file
//
//  Created on: 2020Äê03ÔÂ09ÈÕ
//
//  Author: Jie Kang
//
//#############################################################################

#define _FLASH

#include "DSP28x_Project.h"
#include "clk.h"
#include "cpu.h"
#include "flash.h"
#include "pie.h"
#include "pll.h"
#include "wdog.h"
#include "gpio.h"
#include "adc.h"
#include "timer.h"
#include "i2c.h"
// Create handles for drivers
CLK_Handle myClk;
FLASH_Handle myFlash;
GPIO_Handle myGpio;
PIE_Handle myPie;
CPU_Handle myCpu;
PLL_Handle myPll;
WDOG_Handle myWDog;

void Setup_handles();
void Init_system();
void init_I2C(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows);
void init_LCD();
void backlight();
void cursor();
void blink();
void setCursor(uint8_t, uint8_t);
void writeNum(uint8_t);
void writeStr(const char *str);

void main(void)
{
    Setup_handles();
    Init_system();
    init_I2C(0x27,20,4);
    init_LCD();
    backlight();
    cursor();
    blink();
    setCursor (0,0);  // go to the top left corner
    writeStr("F28027 LCD I2C");
    setCursor (0,1);
    writeStr("It's fully function");
    setCursor (0,2);
    writeStr("Can use in project");
    setCursor (0,3);
    writeNum(5);
}

void Setup_handles(void)
{
    // Initialize all the handles needed for this application
    myClk = CLK_init((void *)CLK_BASE_ADDR, sizeof(CLK_Obj));
    myCpu = CPU_init((void *)NULL, sizeof(CPU_Obj));
    myFlash = FLASH_init((void *)FLASH_BASE_ADDR, sizeof(FLASH_Obj));
    myGpio = GPIO_init((void *)GPIO_BASE_ADDR, sizeof(GPIO_Obj));
    myPie = PIE_init((void *)PIE_BASE_ADDR, sizeof(PIE_Obj));
    myPll = PLL_init((void *)PLL_BASE_ADDR, sizeof(PLL_Obj));
    myWDog = WDOG_init((void *)WDOG_BASE_ADDR, sizeof(WDOG_Obj));
}

void Init_system(void)
{
    // If running from flash copy RAM only functions to RAM
#ifdef _FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
#endif
    //disable watch dog
    WDOG_disable(myWDog);
    // Perform factory calibration
    CLK_enableAdcClock(myClk);
    (*Device_cal)();
    CLK_disableAdcClock(myClk);
    //Select the internal oscillator 1 as the clock source
    CLK_setOscSrc(myClk, CLK_OscSrc_Internal);
    // Setup the PLL for x10 /2 which will yield 50Mhz = 10Mhz * 10 / 2
    PLL_setup(myPll, PLL_Multiplier_10, PLL_DivideSelect_ClkIn_by_2);
    // Disable the PIE and all interrupts
    PIE_disable(myPie);
    PIE_disableAllInts(myPie);
    CPU_disableGlobalInts(myCpu);
    CPU_clearIntFlags(myCpu);
}
