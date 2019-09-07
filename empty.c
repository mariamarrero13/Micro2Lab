
/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

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
#define lcd_Clear           0b00000001          // replace all characters with ASCII 'space'
#define lcd_Home            0b00000010          // return cursor to first position on first line
#define lcd_EntryMode       0b00000110          // shift cursor from left to right on read/write
#define lcd_DisplayOff      0b00001000          // turn display off
#define lcd_DisplayOn       0b00001100          // display on, cursor off, don't blink character
#define lcd_FunctionReset   0b00110000          // reset the LCD
#define lcd_FunctionSet8bit 0b00111000          // 8-bit data, 2-line display, 5 x 7 font
#define lcd_SetCursor       0b10000000          // set cursor position


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

    /* Call driver init functions */
    GPIO_init();
    // I2C_init();
    // SPI_init();
    // UART_init();
    // Watchdog_init();

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
    /* Turn on user LED */
    GPIO_write(Board_GPIO_07, Board_GPIO_LED_ON);

    while (1) {
        if(GPIO_read(Board_GPIO_06) == 0)
            GPIO_write(Board_GPIO_07,Board_GPIO_LED_ON);
        else
            GPIO_write(Board_GPIO_07,Board_GPIO_LED_OFF);
    }



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
    sleep(100);                                 // initial 40 mSec delay

// Reset the LCD controller
    lcd_command(lcd_FunctionReset);    // first part of reset sequence
    sleep(10);                                  // 4.1 mS delay (min)

    lcd_command(lcd_FunctionReset);    // second part of reset sequence
    sleep(200);                                 // 100uS delay (min)

    lcd_command(lcd_FunctionReset);    // third part of reset sequence
    sleep(200);                                 // this delay is omitted in the data sheet

// Function Set instruction
    lcd_command(lcd_FunctionSet8bit);  // set mode, lines, and font
    sleep(80);                                  // 40uS delay (min)

// The next three instructions are specified in the data sheet as part of the initialization routine,
//  so it is a good idea (but probably not necessary) to do them just as specified and then redo them
//  later if the application requires a different configuration.

// Display On/Off Control instruction
    lcd_command(lcd_DisplayOff);       // turn display OFF
    sleep(80);                                  // 40 uS delay (min)

// Clear Display instruction
    lcd_command(lcd_Clear);            // clear display RAM
    sleep(4);                                   // 1.64 mS delay (min)

// ; Entry Mode Set instruction
    lcd_command(lcd_EntryMode);        // set desired shift characteristics
    sleep(80);                                  // 40 uS delay (min)

// This is the end of the LCD controller initialization as specified in the data sheet, but the display
//  has been left in the OFF condition.  This is a good time to turn the display back ON.

// Display On/Off Control instruction
    lcd_command(lcd_DisplayOn);        // turn the display ON
    sleep(80);                                  // 40 uS delay (min)
}

/*...........................................................................
  Name:     lcd_write_string_8d
; Purpose:  display a string of characters on the LCD
  Entry:    (theString) is the string to be displayed
  Exit:     no parameters
  Notes:    uses time delays rather than checking the busy flag
*/
void lcd_string(uint8_t theString[])
{
    volatile int i = 0;                             // character counter*/
    while (theString[i] != 0)
    {
        lcd_character(theString[i]);
        i++;
        sleep(80);                              // 40 uS delay (min)
    }
}

/*...........................................................................
  Name:     lcd_write_character_8d
  Purpose:  send a byte of information to the LCD data register
  Entry:    (theData) is the information to be sent to the data register
  Exit:     no parameters
  Notes:    does not deal with RW (busy flag is not implemented)
*/
void lcd_character(uint8_t theData)
{
    GPIO_write(Board_GPIO_23, 1);               //Select Data Register
    GPIO_write(Board_GPIO_24,0);

    lcd_byte(theData);                           // write the data
}

/*...........................................................................
  Name:     lcd_write_instruction_8d
  Purpose:  send a byte of information to the LCD instruction register
  Entry:    (theInstruction) is the information to be sent to the instruction register
  Exit:     no parameters
  Notes:    does not deal with RW (busy flag is not implemented)
*/
void lcd_command(uint8_t theInstruction)
{
    GPIO_write(Board_GPIO_23, 0);  //Select Instruction Register
    GPIO_write(Board_GPIO_24,0);

    lcd_byte(theInstruction);                    // write the instruction
}

/*...........................................................................
  Name:     lcd_write_8
  Purpose:  send a byte of information to the LCD module
  Entry:    (theByte) is the information to be sent to the desired LCD register
            RS is configured for the desired LCD register
            E is low
            RW is low
  Exit:     no parameters
  Notes:    use either time delays or the busy flag
*/
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
                                                    // 'Address set-up time' (40 nS)
    GPIO_write(Board_GPIO_24,0); // Enable pin high
    sleep(1);                                   // implement 'Data set-up time' (80 nS) and 'Enable pulse width' (230 nS)
    GPIO_write(Board_GPIO_24,0);                  // Enable pin low
    sleep (1);                                   // implement 'Data hold time' (10 nS) and 'Enable cycle time' (500 nS)
}
