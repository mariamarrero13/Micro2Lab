/*
 * Lab7.c
 *
 *  Created on: Oct 30, 2019
 *      Author: maria
 */

#include <unistd.h>
#include <ctype.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>
#include "Board.h"
#include "lcd.h"

void generating_volts();
void reading_volts();
void dimmer();
void complimentary_task();
uint8_t o_index = 13;
uint8_t index = 0;
uint8_t direction =  1 ;


Timer_Handle timer;

/*
 * HELPER METHODS
 */
void init_timer(int period, Timer_Mode mode, Timer_PeriodUnits units, Timer_CallBackFxn callback);
void write_to_port(uint8_t byte);

/*
 * ISRs
 */

void change_dac_value(Timer_Handle myHandle)
{
    o_index = index;
    if(++index >12) index =0;

    //sin wave
    //    if(direction == 1){
    //        if(++index > 0xFF){
    //            index = 0xFE;
    //            direction = 2; //decreasing
    //        }
    //    }
    //    else{
    //        if(--index < 0x00){
    //            index++;
    //            direction = 1; //increasing
    //        }
    //    }
}


void *mainThread(void *arg0)
{

    generating_volts();
    //reading_volts();
    //dimmer();
    //complimentary_task();


    return (NULL);
}

void generating_volts()
{
    //this uses PINS 18, 8, 7, 6, 21, 15, 61 and 62
    GPIO_init();
    GPIO_setConfig(Board_GPIO_28, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //    p18
    GPIO_setConfig(Board_GPIO_17, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //    p08
    GPIO_setConfig(Board_GPIO_16, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //    p07
    GPIO_setConfig(Board_GPIO_15, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //    p06
    GPIO_setConfig(Board_GPIO_25, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //    p21
    GPIO_setConfig(Board_GPIO_22, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //    p15
    GPIO_setConfig(Board_GPIO_COL2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //    p61
    GPIO_setConfig(Board_GPIO_COL3, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW); //    p62


    //comment this for sin wave
    uint8_t dac_values[] = {0x00, 0x17, 0x2E, 0x45, 0x5C, 0x73, 0x8A, 0xA1, 0xB8, 0xCF, 0xE6, 0xFF};

    init_timer(1000000,Timer_CONTINUOUS_CALLBACK,Timer_PERIOD_US, change_dac_value);
    while(1){
        if(o_index!=index){
            write_to_port(dac_values[index]);
            o_index = index;
        }

    }
    //comment this for sin wave

    //sin wave
    //to 500Hz, period == 2000us
    //remember to uncomment code in ISR
    //        init_timer(2000,Timer_CONTINUOUS_CALLBACK,Timer_PERIOD_US, change_dac_value);
    //    while(1){
    //        if(o_index!=index){
    //            //write_to_port(index);
    //            o_index = index;
    //        }
    //    }
}





/*
 * HELPER METHODS
 */

void init_timer(int period, Timer_Mode mode, Timer_PeriodUnits units, Timer_CallBackFxn callback)
{
    Timer_Params params;
    Timer_Params_init(&params);
    params.period = period;
    params.periodUnits = units;
    params.timerMode = mode;
    params.timerCallback = callback;
    timer = Timer_open(Board_TIMER0, &params);
    Timer_start(timer);
}

void write_to_port(uint8_t byte)
{
    GPIO_write(Board_GPIO_28,(unsigned int)(1 & (byte >> 7 )));
    GPIO_write(Board_GPIO_17,(unsigned int)(1 & (byte >> 6 )));
    GPIO_write(Board_GPIO_16,(unsigned int)(1 & (byte >> 5 )));
    GPIO_write(Board_GPIO_15,(unsigned int)(1 & (byte >> 4 )));
    GPIO_write(Board_GPIO_25,(unsigned int)(1 & (byte >> 3 )));
    GPIO_write(Board_GPIO_22,(unsigned int)(1 & (byte >> 2 )));
    GPIO_write(Board_GPIO_COL2,(unsigned int)(1 & (byte >> 1 )));
    GPIO_write(Board_GPIO_COL3,(unsigned int)(1 & (byte >> 0 )));

}
