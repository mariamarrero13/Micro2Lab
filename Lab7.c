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
#include <math.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/ADC.h>
#include "Board.h"
#include "lcd.h"

void generating_volts();
void reading_volts();
void dimmer();
void complimentary_task();
uint8_t o_index = 13;
uint8_t index = 0;
uint8_t direction =  1 ;
uint_fast16_t adcValue;
uint8_t status = 0;



Timer_Handle timer;
PWM_Handle pwm1 = NULL;

/*
 * HELPER METHODS
 */
void init_timer(int period, Timer_Mode mode, Timer_PeriodUnits units, Timer_CallBackFxn callback);
void write_to_port(uint8_t byte);
int bcdtodec(uint8_t bcd);
void PWMod_init();

/*
 * ISRs
 */

void change_dac_value(Timer_Handle myHandle) //
{
    //comment for sin wave
    if(++index ==12) index =0;

    //uncomment for sin wave
    //        if(++index ==50) index =0;
}
void change_units(uint_least8_t index){
    if(status == 1) status =0;
    else status = 1;
}


void *mainThread(void *arg0)
{

    //    generating_volts();
    //    reading_volts();
    //    dimmer();
    complimentary_task();


    return (NULL);
}

void generating_volts()
{
    //this uses PINS 18, 8, 7, 6, 21, 15, 61 and 62
    GPIO_init();
    Timer_init();
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

    /*sin wave
     *to 500Hz, period == 2000us
    remember to uncomment code in ISR*/

    //    uint8_t dac_values[] = {0x80,0x8f,0x9f,0xae,0xbd,0xca,0xd7,0xe2,
    //                            0xeb,0xf3,0xf9,0xfd,0xff,0xff,0xfd,0xf9,
    //                            0xf3,0xeb,0xe2,0xd7,0xca,0xbd,0xae,0x9f,
    //                            0x8f,0x80,0x70,0x60,0x51,0x42,0x35,0x28,
    //                            0x1d,0x14,0xc,0x6,0x2,0x0,0x0,0x2,
    //                            0x6,0xc,0x14,0x1d,0x28,0x35,0x42,0x51,
    //                            0x60,0x70,0x80};
    //    init_timer(40,Timer_CONTINUOUS_CALLBACK,Timer_PERIOD_US, change_dac_value);
    //    while(1){
    //        if(o_index!=index){
    //            write_to_port(dac_values[index]);
    //            o_index = index;
    //        }
    //    }
}

void reading_volts()
{
    uint8_t result;
    uint32_t adcValueUv;
    uint8_t first[1];
    uint8_t sec[1];
    uint8_t third[1];
    uint8_t fourth[1];
    uint8_t hex[2];

    ADC_init();
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);

    ADC_Params params;
    ADC_Params_init(&params);
    ADC_Handle adcHandle = ADC_open(CC3220S_LAUNCHXL_ADC0, &params);  //pin 59 used
    while(1){
        result =  ADC_convert(adcHandle, &adcValue);

        if (result == ADC_STATUS_SUCCESS)
        {
            adcValueUv = ADC_convertToMicroVolts(adcHandle,adcValue)/421,530;
            lcd_command(lcd_SetCursor|lcd_LineOne);
            sprintf(hex, "%02x", adcValue);
            lcd_string(hex);

            lcd_command(lcd_SetCursor|lcd_LineTwo);
            ltoa( adcValueUv%10,fourth);
            adcValueUv/=10;
            ltoa( adcValueUv%10,third);
            adcValueUv/=10;
            ltoa( adcValueUv%10,sec);
            adcValueUv/=10;
            ltoa( adcValueUv%10,first);
            //add hex value to lcd
            //this should be line two
            lcd_string(first);
            lcd_string(".");
            lcd_string(sec);
            lcd_string(third);
            lcd_string(fourth);

            sleep(1);

        }
    }
}
void dimmer()
{
    uint8_t result;
    uint32_t adcValueUv;
    uint16_t duty_val;
    uint8_t lcd_duty[1];

    ADC_init();
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    PWMod_init();

    ADC_Params params;
    ADC_Params_init(&params);
    ADC_Handle adcHandle = ADC_open(CC3220S_LAUNCHXL_ADC0, &params);  //pin 59 used
    while(1){
        result =  ADC_convert(adcHandle, &adcValue);

        if (result == ADC_STATUS_SUCCESS)
        {
            adcValueUv = ADC_convertToMicroVolts(adcHandle,adcValue)/421,530;
            duty_val = (adcValueUv*1000)/1500;
            PWM_setDuty(pwm1,duty_val); //pin 1 used
            sprintf(lcd_duty, "%03d", duty_val/10);
            lcd_command(lcd_SetCursor|lcd_LineOne);
            lcd_string(lcd_duty);

            if(adcValueUv<100){
                lcd_command(lcd_SetCursor|lcd_LineTwo);
                lcd_string("MIN");
            }
            else if(adcValueUv>1490){
                lcd_command(lcd_SetCursor|lcd_LineTwo);
                lcd_string("MAX");
            }
            else{
                lcd_command(lcd_SetCursor|lcd_LineTwo);
                lcd_string("                "); //clear line
            }
        }
        usleep(1000);
    }
}
void complimentary_task()
{
    ADC_init();
    GPIO_init();

    GPIO_setConfig(Board_GPIO_BUTTON1, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);  // pin 2 used, pb_out1 in lcd
    GPIO_setCallback(Board_GPIO_BUTTON1, change_units);
    GPIO_enableInt(Board_GPIO_BUTTON1);
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    uint8_t result;
    uint32_t adcValueUv;
    uint32_t tempF;
    ADC_Params params;
    uint8_t lcd_temp[1];
    uint8_t decimal[1];
    ADC_Params_init(&params);
    ADC_Handle adcHandle = ADC_open(CC3220S_LAUNCHXL_ADC0, &params);  //pin 59 used
    while(1)
    {
        result =  ADC_convert(adcHandle, &adcValue);
        if (result == ADC_STATUS_SUCCESS)
        {
            adcValueUv = ADC_convertToMicroVolts(adcHandle,adcValue)/421,530;
            lcd_command(lcd_SetCursor|lcd_LineOne);
            //write value to lcd
            if(status == 0){
                sprintf(lcd_temp, "%02d", adcValueUv/10);
                lcd_string("Temp in C: ");
                lcd_string(lcd_temp);
                ltoa(adcValueUv%10,decimal);
                lcd_string(".");
                lcd_string(decimal);
            }
            else{
                tempF = ((adcValueUv * 9)/5) + 320 ;
                sprintf(lcd_temp, "%02d", tempF/10);
                lcd_string("Temp in F: ");
                lcd_string(lcd_temp);
                ltoa(adcValueUv%10,decimal);
                lcd_string(".");
                lcd_string(decimal);
            }


        }
    }


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
void PWMod_init(){
    uint16_t   pwmPeriod = 1000;
    PWM_Params params;
    PWM_init();
    PWM_Params_init(&params);
    params.dutyUnits = PWM_DUTY_US;
    params.dutyValue = 0;
    params.periodUnits = PWM_PERIOD_US;
    params.periodValue = pwmPeriod;
    pwm1 = PWM_open(Board_PWM0, &params);

    PWM_start(pwm1);


}
int bcdtodec(uint8_t bcd)
{
    int dec = (( (( bcd & 0xF0 )>>4) * 10 ) + ( bcd & 0x0F ));
    return dec;
}
