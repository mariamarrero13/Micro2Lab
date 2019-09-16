/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>

/* Board Header file */
#include "Board.h"

#define F_CPU 16000000UL

// LCD instructions
#define Clear           0b00000001
#define lcd_EntryMode   0b00000110
#define Display_off     0b00001000
#define Display_on      0b00001111
#define FunctionReset   0b00110000
#define FunctionSet     0b00101100
#define lcd_SetCursor   0b10000000
#define lcd_LineTwo     0x40


// Function Prototypes
void button_interrupt (void);
void software_debouncing(void);
void keypad(void);
void lcd_byte(uint8_t);
void lcd_command(uint8_t);
void lcd_character(uint8_t);
void lcd_string(uint8_t *);
void lcd_init_4bit(void);
void gpio_on(void);
void gpio_of(void);

uint8_t count = 0;
uint8_t flag = -1;
uint8_t text[] = "base string";



void keypadint(uint_least8_t index)
{
    {
        GPIO_write(Board_GPIO_ROW1,GPIO_ON);
        GPIO_write(Board_GPIO_ROW2,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW3,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW4,GPIO_OFF);
        if(GPIO_read(Board_GPIO_COL1) == 1) {
            ltoa(1,text);
            lcd_string(text);
        }
        if(GPIO_read(Board_GPIO_COL2) == 1) {
            ltoa(2,text);
            lcd_string(text);
        }
        if(GPIO_read(Board_GPIO_COL3) == 1) {
            ltoa(3,text);
            lcd_string(text);
        }
    }
    {
        GPIO_write(Board_GPIO_ROW1,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW2,GPIO_ON);
        GPIO_write(Board_GPIO_ROW3,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW4,GPIO_OFF);
        if(GPIO_read(Board_GPIO_COL1) == 1) {
            ltoa(4,text);
            lcd_string(text);
        }
        if(GPIO_read(Board_GPIO_COL2) == 1) {
            ltoa(5,text);
            lcd_string(text);
        }
        if(GPIO_read(Board_GPIO_COL3) == 1) {
            ltoa(6,text);
            lcd_string(text);
        }
    }
    {
        GPIO_write(Board_GPIO_ROW1,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW2,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW3,GPIO_ON);
        GPIO_write(Board_GPIO_ROW4,GPIO_OFF);
        if(GPIO_read(Board_GPIO_COL1) == 1) {
            ltoa(7,text);
            lcd_string(text);
        }
        if(GPIO_read(Board_GPIO_COL2) == 1) {
            ltoa(8,text);
            lcd_string(text);
        }
        if(GPIO_read(Board_GPIO_COL3) == 1) {
            ltoa(9,text);
            lcd_string(text);
        }
        {
            GPIO_write(Board_GPIO_ROW1,GPIO_OFF);
            GPIO_write(Board_GPIO_ROW2,GPIO_OFF);
            GPIO_write(Board_GPIO_ROW3,GPIO_OFF);
            GPIO_write(Board_GPIO_ROW4,GPIO_ON);
            if(GPIO_read(Board_GPIO_COL1) == 1) {
                lcd_command(Clear);
            }
            if(GPIO_read(Board_GPIO_COL2) == 1) {
                ltoa(0,text);
                lcd_string(text);
            }
            if(GPIO_read(Board_GPIO_COL3) == 1) {
                lcd_command(Clear);
            }
        }


        GPIO_write(Board_GPIO_ROW1,GPIO_ON);
        GPIO_write(Board_GPIO_ROW2,GPIO_ON);
        GPIO_write(Board_GPIO_ROW3,GPIO_ON);
        GPIO_write(Board_GPIO_ROW4,GPIO_ON);
    }
}



void btn_interrupt(uint_least8_t index)
{
    if(GPIO_read(index) ==1)
        count++;
}

void btn_interrupt2(uint_least8_t index)
{
    GPIO_clearInt(index);
    if(GPIO_read(index) ==1)
        flag =1;
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    //button_interrupt ();
    // software_debouncing();
    keypad();

}

void lcd_init_4bit(void)
{

    GPIO_setConfig(Board_GPIO_28, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D7    p18
    GPIO_setConfig(Board_GPIO_17, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D6    p08
    GPIO_setConfig(Board_GPIO_16, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D5    p07
    GPIO_setConfig(Board_GPIO_15, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D4    p06
    GPIO_setConfig(Board_GPIO_22, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //E     p15
    GPIO_setConfig(Board_GPIO_25, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //RS    p21

    // Power-up delay
    usleep(40000);

    //Initialization
    lcd_byte(FunctionReset);
    usleep(50);

    lcd_command(FunctionSet);
    usleep(50);

    lcd_command(FunctionSet);
    usleep(50);

    lcd_command(Display_off);
    usleep(50);

    lcd_command(Clear);
    usleep(1600);

    lcd_command(lcd_EntryMode);
    usleep(50);

    lcd_command(Display_on);
    usleep(50);


}


void lcd_string(uint8_t word[])
{
    volatile int i = 0;
    while (word[i] != 0)
    {
        lcd_character(word[i]);
        i++;
        usleep(40);
    }
}

void lcd_character(uint8_t character)
{
    GPIO_write(Board_GPIO_25, GPIO_ON);               // Data Register

    lcd_byte(character);                                // write the data
    lcd_byte(character<<4);
}

void lcd_command(uint8_t command)
{
    GPIO_write(Board_GPIO_25, GPIO_OFF);              // Instruction Register

    lcd_byte(command);                         // write the instruction
    lcd_byte(command << 4);                         // write the instruction
}

void lcd_byte(uint8_t byte)
{
    GPIO_write(Board_GPIO_28,(unsigned int)(1 & (byte >> 7 )));
    GPIO_write(Board_GPIO_17,(unsigned int)(1 & (byte >> 6 )));
    GPIO_write(Board_GPIO_16,(unsigned int)(1 & (byte >> 5 )));
    GPIO_write(Board_GPIO_15,(unsigned int)(1 & (byte >> 4 )));


    // write the data
    usleep(.50);
    GPIO_write(Board_GPIO_22,GPIO_ON);
    usleep(.50);
    GPIO_write(Board_GPIO_22,GPIO_OFF);
    usleep (.50);
}

void button_interrupt (void){

    GPIO_init();
    GPIO_setConfig(Board_GPIO_24, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_RISING); //  1 when pressed

    lcd_init_4bit();
    lcd_init_4bit();

    GPIO_setCallback(Board_GPIO_24, btn_interrupt);
    GPIO_enableInt(Board_GPIO_24);
    while(1){
        if(flag != count){
            lcd_command(Clear);
            usleep(50);
            ltoa(count,text);
            lcd_string(text);
            flag = count;
        }
    }
}

void software_debouncing(void)
{
    GPIO_init();
    GPIO_setConfig(Board_GPIO_24, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_RISING); //  1 when pressed
    lcd_init_4bit();
    lcd_init_4bit();

    GPIO_setCallback(Board_GPIO_24, btn_interrupt2);
    GPIO_enableInt(Board_GPIO_24);
    while(1){
        if(flag != 0){
            usleep(105000);
            lcd_command(Clear);
            usleep(50);
            ltoa(count++,text);
            lcd_string(text);
            flag=0;
        }
    }
}
void keypad(void){
    GPIO_init();
    GPIO_setConfig(Board_GPIO_COL1, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_RISING);
    GPIO_setConfig(Board_GPIO_COL2, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_RISING);
    GPIO_setConfig(Board_GPIO_COL3, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_RISING);
    GPIO_setConfig(Board_GPIO_ROW1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    GPIO_setConfig(Board_GPIO_ROW2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    GPIO_setConfig(Board_GPIO_ROW3, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);
    GPIO_setConfig(Board_GPIO_ROW4, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_HIGH);

    lcd_init_4bit();
    lcd_init_4bit();

    GPIO_setCallback(Board_GPIO_COL1, keypadint);
    GPIO_setCallback(Board_GPIO_COL2, keypadint);
    GPIO_setCallback(Board_GPIO_COL3, keypadint);
    GPIO_enableInt(Board_GPIO_COL1);
    GPIO_enableInt(Board_GPIO_COL2);
    GPIO_enableInt(Board_GPIO_COL3);

}









