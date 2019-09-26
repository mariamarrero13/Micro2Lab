#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>

/* Board Header file */
#include "Board.h"
#include "lcd.h"
void buzzer_toggle(Timer_Handle myHandle);
void board_button(uint_least8_t index);
void timer_by_polling(void);
void timer_by_interrupts(void);
void init_timer(int period, Timer_Mode mode, Timer_CallBackFxn callback);
void led_byte(uint8_t byte);
void singleLED(void);
void refreshLED(void);

int i = 0;
int flag =0;
int period = 2000;
Timer_Handle timer;

void *mainThread(void *arg0)
{
    GPIO_init();
    Timer_init();

    // timer_by_polling();
    timer_by_interrupts();
    //    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    //    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
}

void timer_by_polling(void)
{
    uint32_t poll;
    uint32_t oldpoll;
    GPIO_setConfig(Board_GPIO_24, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    init_timer(1000,Timer_FREE_RUNNING, NULL);

    while (1){
        oldpoll=poll;
        poll = Timer_getCount(timer);
        if (Timer_getCount(timer)  == 0){
            GPIO_toggle(Board_GPIO_24);
            printf("Poll = %d", oldpoll);
        }
    }
}

void timer_by_interrupts(void)
{
    GPIO_setConfig(Board_GPIO_BUTTON0, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setCallback(Board_GPIO_BUTTON0, board_button);
    GPIO_enableInt(Board_GPIO_BUTTON0);
    GPIO_setConfig(Board_GPIO_24, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    init_timer(period,Timer_CONTINUOUS_CALLBACK, buzzer_toggle);

    while(1)
    {
        sleep(1);
        flag=0;
    }
}

void init_timer(int period, Timer_Mode mode, Timer_CallBackFxn callback){
    Timer_Params params;
    Timer_Params_init(&params);
    params.period = period;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = mode;
    params.timerCallback = callback;
    timer = Timer_open(Board_TIMER0, &params);
    Timer_start(timer);
}

void buzzer_toggle(Timer_Handle myHandle)
{
    GPIO_toggle(Board_GPIO_24);
}

void board_button(uint_least8_t index)
{
    if(period==2000 && flag == 0 ){
        period=1000;
        flag=1;
        Timer_close(timer);
        init_timer(period,Timer_CONTINUOUS_CALLBACK,buzzer_toggle);
    }
    if(period==1000 && flag ==0 ){
        period=666;
        flag=1;
        Timer_close(timer);
        init_timer(period,Timer_CONTINUOUS_CALLBACK,buzzer_toggle);
    }
    if(period==666 && flag ==0 ){
        period=500;
        flag=1;
        Timer_close(timer);
        init_timer(period,Timer_CONTINUOUS_CALLBACK,buzzer_toggle);
    }
    if(period==500 && flag ==0 ) {
        period=333;
        flag=1;
        Timer_close(timer);
        init_timer(period,Timer_CONTINUOUS_CALLBACK,buzzer_toggle);
    }
    if(period==333 && flag ==0 ) {
        period=2000;
        flag=1;
        Timer_close(timer);
        init_timer(period,Timer_CONTINUOUS_CALLBACK,buzzer_toggle);
    }
}


void seven_segment(void){
    i=0;
    //uint8_t lookup_table[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99h, 0x92, 0x82, 0xF8, 0x80, 0x98, 0x88, 0x80, 0xC6, 0xC0, 0x86, 0x8E};
    uint8_t lookup_table[] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xE7, 0xF7, 0xFF, 0xB9, 0xBF, 0xF9, 0xF1};

    GPIO_init();

    /*Seven segment*/
    GPIO_setConfig(Board_GPIO_A, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_B, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_C, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_D, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_E, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_F, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_G, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_DP, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /*Control signal*/
    GPIO_setConfig(Board_GPIO_7S1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_7S2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    while (1) {
        singleLED();
    }
}

void led_byte(uint8_t byte){
    GPIO_write(Board_GPIO_DP,(unsigned int)(1 & (byte >> 7 )));
    GPIO_write(Board_GPIO_G,(unsigned int)(1 & (byte >> 6 )));
    GPIO_write(Board_GPIO_F,(unsigned int)(1 & (byte >> 5 )));
    GPIO_write(Board_GPIO_E,(unsigned int)(1 & (byte >> 4 )));
    GPIO_write(Board_GPIO_D,(unsigned int)(1 & (byte >> 3 )));
    GPIO_write(Board_GPIO_C,(unsigned int)(1 & (byte >> 2 )));
    GPIO_write(Board_GPIO_B,(unsigned int)(1 & (byte >> 1 )));
    GPIO_write(Board_GPIO_A,(unsigned int)(1 & (byte >> 0 )));
}

void singleLED(void){
    sleep(1);
    lcd_byte(lookup_table[i]);
    i = i+1;
    if(i == sizeof(lookup_table)){
        i = 0;
    }
}

void refreshLED(void){
    GPIO_write(Board_GPIO_7S1,GPIO_OFF);
    GPIO_write(Board_GPIO_7S2,GPIO_ON);
    sleep(1);
    GPIO_write(Board_GPIO_7S1,GPIO_ON);
    GPIO_write(Board_GPIO_7S2,GPIO_OF);
    sleep(1);
}

