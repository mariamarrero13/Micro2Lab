
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

// LCD instructions
#define Clear           0b00000001
#define lcd_EntryMode   0b00000110
#define Display_off     0b00001000
#define Display_on      0b00001111
#define FunctionReset   0b00110000
#define FunctionSet     0b00101100
#define lcd_SetCursor   0b10000000
#define lcd_LineOne     0x00
#define lcd_LineTwo     0x40
#define lcd_Home        0b00000010

// Function Prototypes
void lcd_basic(void);
void circular_list(void);
uint8_t down_pressed(uint8_t, uint8_t);
uint8_t up_pressed(uint8_t, uint8_t);
void lcd_byte(uint8_t);
void lcd_command(uint8_t);
void lcd_command_simple(uint8_t);
void lcd_character(uint8_t);
void lcd_string(uint8_t *);
void lcd_init_4bit(void);

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    /* Call driver init functions */
    GPIO_init();

    /* Configure LED pin and PB1 */
    //Comment these two lines if using LCD
   GPIO_setConfig(Board_GPIO_07, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //LED   p62
   GPIO_setConfig(Board_GPIO_06, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING); // PUSHBUTTON p51


    GPIO_setConfig(Board_GPIO_28, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D7    p18
    GPIO_setConfig(Board_GPIO_17, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D6    p08
    GPIO_setConfig(Board_GPIO_16, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D5    p07
    GPIO_setConfig(Board_GPIO_15, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D4    p06

    GPIO_setConfig(Board_GPIO_22, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //E    p15
    GPIO_setConfig(Board_GPIO_25, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //RS    p21


//    lcd_basic();
   circular_list();

}
void lcd_basic(void){
    uint8_t text[] = "Yamil";
    uint8_t text2[] = "Gonzalez";
    usleep(400000);
    lcd_init_4bit();

    lcd_string(text);
    usleep(500);

    lcd_command(lcd_SetCursor|lcd_LineTwo);
    usleep(50);
    lcd_string(text2);
    while (1){}
}

void lcd_init_4bit(void)
{

// Power-up delay
    sleep(0.015);
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

void lcd_command_simple(uint8_t command)
{
    GPIO_write(Board_GPIO_25, GPIO_OFF);              // Instruction Register
    lcd_byte(command);                         // write the instruction
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


void circular_list(void){
    const int STRINGS = 16;
      uint8_t text[STRINGS][16] = {"Maria","Alejandra","Marrero", "Ortiz",
                                   "Yamil", "Jose", "Gonzalez", "Zuaznabar",
                                   "Diego", "Jose","Amador", "Bonilla",
                                   "Christian", "Antonio", "Santiago", "Berio"};
      uint8_t positions[] = {0,1};
      GPIO_setConfig(Board_GPIO_07, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING); //LED
      GPIO_setConfig(Board_GPIO_06, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING); // PUSHBUTTON

      lcd_init_4bit();
      usleep(50);
      lcd_string(text[positions[0]]);
      usleep(50);
      lcd_command(lcd_Home);
      lcd_command(lcd_SetCursor|lcd_LineTwo);
      usleep(50);
      lcd_string(text[positions[1]]);
while(1){
//    //if pushdown
      if(GPIO_read(Board_GPIO_07) == 1){          //button down pressed --CHANGE GPIO
          lcd_command(Clear);
          lcd_command(lcd_Home);
          usleep(50);
          int oldpos = positions[1];
          positions[0] = positions[1];
          positions[1] = down_pressed(oldpos,STRINGS);
          lcd_string(text[positions[0]]);
          usleep(50);
          lcd_command(lcd_SetCursor|lcd_LineTwo);
          usleep(50);
          lcd_string(text[positions[1]]);
          sleep(0.5);

      }
      if(GPIO_read(Board_GPIO_06) == 0){          //button up pressed -CHANGE GPIO
          lcd_command(Clear);
          lcd_command(lcd_Home);
          usleep(50);
          int oldpos = positions[0];
          positions[0] = up_pressed(oldpos,STRINGS);
          positions[1] = oldpos;
          lcd_string(text[positions[0]]);
          usleep(50);
          lcd_command(lcd_SetCursor|lcd_LineTwo);
          usleep(50);
          lcd_string(text[positions[1]]);
          sleep(0.5);
      }

}
}
uint8_t down_pressed(uint8_t index,uint8_t size){
        if(index == ( size -1))
            return 0;
        else
            return index +1;
    }

uint8_t up_pressed(uint8_t index, uint8_t size){
        if(index == 0)
            return size-1;
        else
            return index -1;
    }


