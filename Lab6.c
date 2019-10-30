/*
 * Lab6.c
 *
 *  Created on: Oct 27, 2019
 *      Author: maria
 */

#include <unistd.h>
#include <ctype.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/I2C.h>
#include "Board.h"
#include "lcd.h"
#define OPT_ADDR 0b1101000
#define SENSORS 0
void uart();
void lcdDisplay(UART_Handle handle, void *buf, size_t size);
void uart_on_lcd();
void i2c();


UART_Handle uart_handle;
I2C_Handle i2cHandle;
uint8_t string;
int flag = 0;
uint8_t set_time[9] = {0x00,0x00, 0x05, 0x15, 0x02, 0x29, 0x09, 0x19, 0x00};
uint8_t reading[8];
uint8_t readFrom[2] = {0x00, 0x00};
int bcdtodec(uint8_t bcd){
    int dec = (( (( bcd & 0xF0 )>>4) * 10 ) + ( bcd & 0x0F ));
    return dec;
}
void num_to_Day(int DAY){
    switch(DAY){
    case 0:
        lcd_string("Sun ");
        break;
    case 1:
        lcd_string("Mon ");
        break;
    case 2:
        lcd_string("Tue ");
        break;
    case 3:
        lcd_string("Wed ");
        break;
    case 4:
        lcd_string("Thu ");
        break;
    case 5:
        lcd_string("Fri ");
        break;
    case 6:
        lcd_string("Sat ");
        break;
    }
}

void num_to_Mon(int MONTH){
    switch(MONTH){
    case 0:
        lcd_string("Jan ");
        break;
    case 1:
        lcd_string("Feb ");
        break;
    case 2:
        lcd_string("Mar ");
        break;
    case 3:
        lcd_string("Apr ");
        break;
    case 4:
        lcd_string("May ");
        break;
    case 5:
        lcd_string("Jun ");
        break;
    case 6:
        lcd_string("Jul ");
        break;
    case 7:
        lcd_string("Aug ");
        break;
    case 8:
        lcd_string("Sep ");
        break;
    case 9:
        lcd_string("Oct ");
        break;
    case 10:
        lcd_string("Nov ");
        break;
    case 11:
        lcd_string("Dic ");
        break;
    }
}
void *mainThread(void *arg0){
    //rx is pin 04
    //tx is pin 03
//        uart();
    uart_on_lcd();
    //    i2c();

    return (NULL);
}

void lcdDisplay(UART_Handle handle, void *buf, size_t size){
    //    lcd_character(buf);
    flag = 1;


}

void uart(){
    UART_Params uartParams;
    char        input;
    const char  echoPrompt[] = "Hello World!! EOT\r\n";

    GPIO_init();
    UART_init();


    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_TEXT;
    uartParams.writeMode = UART_MODE_BLOCKING;
    uartParams.readMode = UART_MODE_BLOCKING;
    uartParams.readDataMode = UART_DATA_TEXT;
    uartParams.readReturnMode = UART_RETURN_NEWLINE;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 9600;

    uart_handle = UART_open(Board_UART0, &uartParams);


    UART_writePolling(uart_handle, echoPrompt, sizeof(echoPrompt));


    while (1) {
        UART_readPolling(uart_handle, &input, 1);
        UART_writePolling(uart_handle, &input, 1);
    }
}

void uart_on_lcd(){
    UART_Params uartParams;
    char input;
    int charcount;
    int line = 1;

    GPIO_init();
    UART_init();

    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    //    lcd_string("Hello World");
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_TEXT;
    uartParams.writeMode = UART_MODE_BLOCKING;
    uartParams.readMode = UART_MODE_CALLBACK;
    uartParams.readDataMode = UART_DATA_TEXT;
    uartParams.readReturnMode = UART_RETURN_NEWLINE;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 9600;
    uartParams.readCallback = lcdDisplay;

    uart_handle = UART_open(Board_UART0, &uartParams);
    while(1){
        UART_read(uart_handle, &input, 1);
        if(flag) {
            if(++charcount ==16){
                if (line == 1){
                    lcd_command(lcd_SetCursor|lcd_LineTwo);
                    line =2;
                    charcount = 0;
                }
                else{
                    lcd_command(lcd_SetCursor|lcd_LineOne);
                    line =1;
                    charcount=0;
                }
            }

            input= toupper(input);


            lcd_character(input);
            UART_write(uart_handle, &input, 1);
            flag = 0;

        }

    }
}

void i2c()
{
    int i;
    uint8_t SS[2];
    uint8_t MM[2];
    uint8_t HH[2];
    uint8_t DD[2];
    uint8_t YY[2];
    uint8_t DAY;
    uint8_t MONTH;

    //    uint8_t time[8];
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    I2C_init();
    I2C_Params params;
    I2C_Params_init(&params);
    params.bitRate = I2C_100kHz;

    I2C_Handle i2cHandle = I2C_open(CC3220S_LAUNCHXL_I2C0, &params);
    I2C_Transaction transaction = {0};

    transaction.slaveAddress = OPT_ADDR;
    //write
    transaction.writeBuf = set_time;
    transaction.writeCount = sizeof(set_time);
    transaction.readCount = 0;
    I2C_transfer(i2cHandle, &transaction);
    //read

    transaction.slaveAddress = OPT_ADDR;
    transaction.readBuf = reading;
    transaction.readCount = sizeof(reading);
    transaction.writeBuf = readFrom;
    transaction.writeCount =1;
    while(1){

        I2C_transfer(i2cHandle, &transaction);

        reading[0] &= 0x7F;
        reading[2] &= 0x1F;

        sprintf(SS, "%02d", bcdtodec(reading[0]));
        sprintf(MM, "%02d:", bcdtodec(reading[1]));
        sprintf(HH, "%02d:", bcdtodec(reading[2]));
        DAY = bcdtodec(reading[3]);
        sprintf(DD, "%02d ", bcdtodec(reading[4]));
        MONTH = bcdtodec(reading[5]);
        sprintf(YY, "%d", bcdtodec(reading[6]));

        // HH MM SS
        //DAY MONTH DD YY

        lcd_string(HH);
        lcd_string(MM);
        lcd_string(SS);
        lcd_command(lcd_SetCursor|lcd_LineTwo);
        num_to_Day(DAY);
        num_to_Mon(MONTH);
        lcd_string(DD);
        lcd_string(YY);
        lcd_command(lcd_SetCursor|lcd_LineOne);



    }


}





