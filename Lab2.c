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
#define lcd_LineOne     0x00
#define lcd_LineTwo     0x40


// Function Prototypes
void button_interrupt (void);
void software_debouncing(void);
void complimentary_task(void);
void keypad(void);
void lcd_byte(uint8_t);
void lcd_command(uint8_t);
void lcd_character(uint8_t);
void lcd_string(uint8_t *);
void lcd_init_4bit(void);
void key_write(uint8_t);
void button_pressed(uint_least8_t);
uint8_t down_pressed(uint8_t, uint8_t);
uint8_t up_pressed(uint8_t, uint8_t);



uint8_t count = 0;
uint8_t flag = 1;
uint8_t text[] = "base string";
uint8_t cursor = 0;
uint8_t olda = 0;
uint8_t oldb = 0;
uint8_t newa = 0;
uint8_t newb = 0;
uint8_t lookup_index =0;


void keypadint(uint_least8_t index)
{
    printf("INTERRUPT CALLED \n");
    sleep(.60);

    int count=0;

    while(GPIO_read(index)==1 && flag == 0)
    {
        count++;
        if(GPIO_read(index)==1 && flag ==0)
        {
            sleep(0.001);
            if(GPIO_read(index)==1 && flag ==0)
            {
                count++;
                if(count>=3 && flag == 0)
                {
                    count=0;
                    flag=1;
                    decode_stroke(index);
                }
            }
            else
                count--;
            if(count>=0)
            {}
            else
                count=0;
        }

    }
    GPIO_write(Board_GPIO_ROW1,GPIO_ON);
    GPIO_write(Board_GPIO_ROW2,GPIO_ON);
    GPIO_write(Board_GPIO_ROW3,GPIO_ON);
    GPIO_write(Board_GPIO_ROW4,GPIO_ON);
    button_pressed(index);

}

void btn_interrupt(uint_least8_t index)
{
        count++;
}

void btn_interrupt2(uint_least8_t index)
{
    GPIO_clearInt(index);
    if(GPIO_read(index) ==1)
        flag =1;
}
void opto_int(uint_least8_t index)
{
    olda= newa;
    oldb= newb;
    newa =GPIO_read(Board_GPIO_COL2);
    newb =GPIO_read(Board_GPIO_COL3);

    flag = olda<<3|oldb<<2|newa<<1|newb<<0;
    lookup_index = flag;
    printf("new a %d \n new b %d \n index %d \n", newa, newb, lookup_index);
}

/*
 *  ======== mainThread ========
 */
void *mainThread2(void *arg0)
{
   // button_interrupt ();
    //software_debouncing();
    //keypad();
    complimentary_task();

}

void lcd_init_4bit(void)
{

    GPIO_setConfig(Board_GPIO_28, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D7    p18
    GPIO_setConfig(Board_GPIO_17, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D6    p08
    GPIO_setConfig(Board_GPIO_16, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D5    p07
    GPIO_setConfig(Board_GPIO_15, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //D4    p06
    GPIO_setConfig(Board_GPIO_22, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //RS    p21
    GPIO_setConfig(Board_GPIO_25, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //E     p15

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
    GPIO_write(Board_GPIO_22, GPIO_ON);               // Data Register

    lcd_byte(character);                                // write the data
    lcd_byte(character<<4);
}

void lcd_command(uint8_t command)
{
    GPIO_write(Board_GPIO_22, GPIO_OFF);              // Instruction Register

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
    GPIO_write(Board_GPIO_25,GPIO_ON);
    usleep(.50);
    GPIO_write(Board_GPIO_25,GPIO_OFF);
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
    GPIO_setConfig(Board_GPIO_COL1, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_RISING);
    GPIO_setConfig(Board_GPIO_COL2, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_RISING);
    GPIO_setConfig(Board_GPIO_COL3, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_RISING);
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

void key_write(uint8_t number)
{
    ltoa(number,text);
    lcd_string(text);
}

void button_pressed(uint_least8_t index){
    while(GPIO_read(index) ==1)
    {flag =1;}
    flag = 0;
}

void decode_stroke(uint_least8_t index){
    usleep(40);
    {
        GPIO_write(Board_GPIO_ROW1,GPIO_ON);
        GPIO_write(Board_GPIO_ROW2,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW3,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW4,GPIO_OFF);
        if(GPIO_read(Board_GPIO_COL1) == 1) key_write(1);
        if(GPIO_read(Board_GPIO_COL2) == 1) key_write(2);
        if(GPIO_read(Board_GPIO_COL3) == 1) key_write(3);
    }
    {
        GPIO_write(Board_GPIO_ROW1,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW2,GPIO_ON);
        GPIO_write(Board_GPIO_ROW3,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW4,GPIO_OFF);
        if(GPIO_read(Board_GPIO_COL1) == 1) key_write(4);
        if(GPIO_read(Board_GPIO_COL2) == 1) key_write(5);
        if(GPIO_read(Board_GPIO_COL3) == 1) key_write(6);
    }
    {
        GPIO_write(Board_GPIO_ROW1,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW2,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW3,GPIO_ON);
        GPIO_write(Board_GPIO_ROW4,GPIO_OFF);
        if(GPIO_read(Board_GPIO_COL1) == 1) key_write(7);
        if(GPIO_read(Board_GPIO_COL2) == 1) key_write(8);
        if(GPIO_read(Board_GPIO_COL3) == 1) key_write(9);
    }
    {
        GPIO_write(Board_GPIO_ROW1,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW2,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW3,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW4,GPIO_ON);
        if(GPIO_read(Board_GPIO_COL1) == 1) {
            lcd_command(Clear);
            cursor =0;
        }
        if(GPIO_read(Board_GPIO_COL2) == 1) key_write(0);
        if(GPIO_read(Board_GPIO_COL3) == 1) toggle_cursor();;
    }
}

void toggle_cursor(void)
{
    if(cursor ==1) {
        lcd_command(lcd_SetCursor|lcd_LineOne);
        cursor =0;
    }
    else{
        lcd_command(lcd_SetCursor|lcd_LineTwo);
        cursor =1;
    }
}

void complimentary_task(void)
{
    GPIO_init();
    const int STRINGS = 16;
    uint8_t text[STRINGS][16] = {"1. Maria","2. Alejandra","3. Marrero", "4. Ortiz",
                                 "5. Yamil", "6. Jose", "7. Gonzalez", "8. Zuaznabar",
                                 "9. Diego", "10. Jose","11. Amador", "12. Bonilla",
                                 "13. Christian", "14. Antonio", "15. Santiago", "16. Berio"};
    uint8_t positions[] = {0,1};
    uint8_t lookup_table[] = {0,1,2,0,2,0,0,1,1,0,0,2,0,2,1,0};
    GPIO_setConfig(Board_GPIO_COL2, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_BOTH_EDGES);
    GPIO_setConfig(Board_GPIO_COL3, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_BOTH_EDGES);
    GPIO_setCallback(Board_GPIO_COL2, opto_int);
    GPIO_setCallback(Board_GPIO_COL3, opto_int);
    GPIO_enableInt(Board_GPIO_COL2);
    GPIO_enableInt(Board_GPIO_COL3);

    lcd_init_4bit();
    lcd_init_4bit();
    usleep(50);
    lcd_string(text[positions[0]]);
    usleep(50);
    lcd_command(lcd_SetCursor|lcd_LineTwo);
    usleep(50);
    lcd_string(text[positions[1]]);

    while(1){
        if(lookup_table[lookup_index] == 1){          //moved forward
            lcd_command(Clear);
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
            lookup_index =0;

        }
        if(lookup_table[lookup_index] == 2){          //moved backward
            lcd_command(Clear);
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
            lookup_index=0;
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










