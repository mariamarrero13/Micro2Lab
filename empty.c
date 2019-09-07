
/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>

/* Board Header file */
#include "Board.h"

#define F_CPU 16000000UL

/*********************** Modify *********/
// LCD module information
#define lcd_LineOne     0x00                    // start of line 1
#define lcd_LineTwo     0x40                    // start of line 2


// LCD instructions
#define Clear           0b00000001
#define lcd_Home        0b00000010
#define lcd_EntryMode   0b00000110
#define Display_off     0b00001000
#define Display_on      0b00001100
#define FunctionReset   0b00110000
#define FunctionSet     0b00111000
#define lcd_SetCursor   0b10000000

// Function Prototypes
void lcd_byte(uint8_t);
void lcd_command(uint8_t);
void lcd_character(uint8_t);
void lcd_string(uint8_t *);
void lcd_init_8d(void);

/****************************Modify ***********/
/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    uint8_t text[]   = "Hello Moto";
    /* Call driver init functions */
    GPIO_init();

    /* Configure LED pin and PB1 */
    GPIO_setConfig(Board_GPIO_07, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_06, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setConfig(Board_GPIO_28, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D7
    GPIO_setConfig(Board_GPIO_17, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D6
    GPIO_setConfig(Board_GPIO_31, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D5
    GPIO_setConfig(Board_GPIO_16, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D4
    GPIO_setConfig(Board_GPIO_15, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D3
    GPIO_setConfig(Board_GPIO_25, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D2
    GPIO_setConfig(Board_GPIO_01, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D1
    GPIO_setConfig(Board_GPIO_22, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D0
    GPIO_setConfig(Board_GPIO_23, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //RS
    GPIO_setConfig(Board_GPIO_24, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //E
//    /* Turn on user LED */
//    GPIO_write(Board_GPIO_07, Board_GPIO_LED_ON);
//
//    while (1) {
//        if(GPIO_read(Board_GPIO_06) == 0)
//            GPIO_write(Board_GPIO_07,Board_GPIO_LED_ON);
//        else
//            GPIO_write(Board_GPIO_07,Board_GPIO_LED_OFF);
//    }
    lcd_init_8d();
    lcd_string(text);
}


/*============================== 8-bit LCD Functions ======================*/
/*
  Name:     lcd_init_8d
  Purpose:  initialize the LCD module for a 8-bit data interface
  Entry:    equates (LCD instructions) set up for the desired operation
  Exit:     no parameters
  Notes:    uses time delays rather than checking the busy flag
*/
void lcd_init_8d(void)
{

// Power-up delay
    sleep(.040);                                 // initial 40 mSec delay

//Initialization
    lcd_command(FunctionSet);
    usleep(40);

    lcd_command(FunctionSet);
    usleep(40);

    lcd_command(Display_off);
    usleep(40);

    lcd_command(Clear);
    sleep(.002);

    lcd_command(lcd_EntryMode);
    usleep(40);

//End of Init
    lcd_command(Display_on);
    usleep(40);
}


void lcd_string(uint8_t theString[])
{
    volatile int i = 0;
    while (theString[i] != 0)
    {
        lcd_character(theString[i]);
        i++;
        usleep(40);
    }
}

void lcd_character(uint8_t theData)
{
    GPIO_write(Board_GPIO_23, 1);               //Select Data Register
    GPIO_write(Board_GPIO_24,0);

    lcd_byte(theData);                           // write the data
}

void lcd_command(uint8_t theInstruction)
{
    GPIO_write(Board_GPIO_23, 0);  //Select Instruction Register
    GPIO_write(Board_GPIO_24,0);

    lcd_byte(theInstruction);                    // write the instruction
}

void lcd_byte(uint8_t theByte)
{
    GPIO_write(Board_GPIO_28,theByte & 1<<7);
    GPIO_write(Board_GPIO_17,theByte & 1<<6);
    GPIO_write(Board_GPIO_31,theByte & 1<<5);
    GPIO_write(Board_GPIO_16,theByte & 1<<4);
    GPIO_write(Board_GPIO_15,theByte & 1<<3);
    GPIO_write(Board_GPIO_25,theByte & 1<<2);
    GPIO_write(Board_GPIO_01,theByte & 1<<1);
    GPIO_write(Board_GPIO_22,theByte & 1<<0);


// write the data
    usleep(0.04);
    GPIO_write(Board_GPIO_24,0);
    usleep(0.08);
    GPIO_write(Board_GPIO_24,0);
    usleep (0.5);
}
