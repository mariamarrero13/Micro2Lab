
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
#define Display_off     0b00001011
#define Display_on      0b00001111
#define FunctionSet     0b00111000
#define lcd_SetCursor   0b10000000

// Function Prototypes
void lcd_byte(uint8_t);
void lcd_command(uint8_t);
void lcd_character(uint8_t);
void lcd_string(uint8_t *);
void lcd_init(void);

/****************************Modify ***********/
/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    uint8_t text[]   = "Hello World";
    uint8_t newtext[]   = "This is Maria";
    /* Call driver init functions */
    GPIO_init();

    /* Configure LED pin and PB1 */
    GPIO_setConfig(Board_GPIO_07, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_06, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setConfig(Board_GPIO_28, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D7
    GPIO_setConfig(Board_GPIO_17, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D6
    GPIO_setConfig(Board_GPIO_30, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D5
    GPIO_setConfig(Board_GPIO_16, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D4
    GPIO_setConfig(Board_GPIO_15, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D3
    GPIO_setConfig(Board_GPIO_25, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D2
    GPIO_setConfig(Board_GPIO_00, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D1
    GPIO_setConfig(Board_GPIO_22, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D0
    GPIO_setConfig(Board_GPIO_08, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //RS
    GPIO_setConfig(Board_GPIO_09, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //E

   GPIO_write(Board_GPIO_28, GPIO_OFF); //D7
   GPIO_write(Board_GPIO_17, GPIO_OFF); //D6
   GPIO_write(Board_GPIO_30, GPIO_OFF); //D5
   GPIO_write(Board_GPIO_16, GPIO_OFF); //D4
   GPIO_write(Board_GPIO_15, GPIO_OFF); //D3
   GPIO_write(Board_GPIO_25, GPIO_OFF); //D2
   GPIO_write(Board_GPIO_00, GPIO_OFF); //D1
   GPIO_write(Board_GPIO_22, GPIO_OFF); //D0
   GPIO_write(Board_GPIO_08, GPIO_OFF); //RS
   GPIO_write(Board_GPIO_09, GPIO_OFF); //E

//    /* Turn on user LED */
//   GPIO_write(Board_GPIO_07, Board_GPIO_LED_ON);
//
//    while (1) {
//        if(GPIO_read(Board_GPIO_06) == 0)
//            GPIO_write(Board_GPIO_07,Board_GPIO_LED_ON);
//        else
//            GPIO_write(Board_GPIO_07,Board_GPIO_LED_OFF);
//    }

   lcd_init();
   lcd_string(newtext);
   usleep(50);
   lcd_command(lcd_SetCursor|lcd_LineTwo);
   usleep(50);
   lcd_string(text);

   while (1) {}

}


void lcd_init(void)
{

// Power-up delay
    sleep(0.015);

//Initialization
    lcd_command(FunctionSet);
    usleep(50);

    lcd_command(FunctionSet);
    usleep(50);

    lcd_command(FunctionSet);
    usleep(50);

    lcd_command(Display_off);
    usleep(50);

    lcd_command(Clear);
    usleep(50);

    lcd_command(lcd_EntryMode);
    usleep(50);

    lcd_command(Display_on);
    usleep(50);


}


void lcd_string(uint8_t theString[])
{
    volatile int i = 0;
    while (theString[i] != 0)
    {
        lcd_character(theString[i]);
        i++;
        sleep(.1);
    }
}

void lcd_character(uint8_t theData)
{
    GPIO_write(Board_GPIO_08, GPIO_ON);               // Data Register

    lcd_byte(theData);                                // write the data
}

void lcd_command(uint8_t theInstruction)
{
    GPIO_write(Board_GPIO_08, GPIO_OFF);              // Instruction Register

    lcd_byte(theInstruction);                         // write the instruction
}

void lcd_byte(uint8_t theByte)
{

    GPIO_write(Board_GPIO_28,(unsigned int)(1 & (theByte >> 7 )));
    GPIO_write(Board_GPIO_17,(unsigned int)(1 & (theByte >> 6 )));
    GPIO_write(Board_GPIO_30,(unsigned int)(1 & (theByte >> 5 )));
    GPIO_write(Board_GPIO_16,(unsigned int)(1 & (theByte >> 4 )));
    GPIO_write(Board_GPIO_15,(unsigned int)(1 & (theByte >> 3 )));
    GPIO_write(Board_GPIO_25,(unsigned int)(1 & (theByte >> 2 )));
    GPIO_write(Board_GPIO_00,(unsigned int)(1 & (theByte >> 1 )));
    GPIO_write(Board_GPIO_22,(unsigned int)(1 & (theByte >> 0 )));


// write the data
    usleep(50);
    GPIO_write(Board_GPIO_09,GPIO_ON);
    usleep(50);
    GPIO_write(Board_GPIO_09,GPIO_OFF);
    usleep (50);
   //printf("The byte = %d %d %d %d %d %d %d %d \n", (unsigned int) 1 & (theByte >> 7 ), (unsigned int)1 & (theByte >> 6 ), (unsigned int) 1 & (theByte >> 5),  1 & (theByte >> 4 ), 1 & (theByte >> 3 ),  1 & (theByte >> 2 ),  1 & (theByte >> 1),  1 & (theByte >> 0 ));
}

