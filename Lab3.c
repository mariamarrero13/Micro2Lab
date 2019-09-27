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
void seven_segment(void);
void init_timer(int period, Timer_Mode mode, Timer_PeriodUnits units, Timer_CallBackFxn callback);
void led_byte(uint8_t byte);
void singleLED(void);
void multipleLED(void);
void refreshLED(Timer_Handle myHandle);

int i0 = 0;
int i = 0;
uint8_t numbers[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x98, 0x88, 0x80, 0xC6, 0xC0, 0x86, 0x8E};
//uint8_t numbers[] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xE7, 0xF7, 0xFF, 0xB9, 0xBF, 0xF9, 0xF1};
int flag =0;
int period = 500;
Timer_Handle timer;
void *mainThread(void *arg0)
{
    GPIO_init();
    Timer_init();

    //   timer_by_polling();
    //    timer_by_interrupts();
    seven_segment();
    //    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    //    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
}

void timer_by_polling(void)
{
    GPIO_setConfig(Board_GPIO_24, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    init_timer(2000,Timer_FREE_RUNNING,Timer_PERIOD_COUNTS, NULL);

    while (1){
        if (Timer_getCount(timer) == 0){
            GPIO_toggle(Board_GPIO_24);
        }
    }
}

void timer_by_interrupts(void)
{
    GPIO_setConfig(Board_GPIO_BUTTON0, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setCallback(Board_GPIO_BUTTON0, board_button);
    GPIO_enableInt(Board_GPIO_BUTTON0);
    GPIO_setConfig(Board_GPIO_24, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    init_timer(period,Timer_CONTINUOUS_CALLBACK,Timer_PERIOD_US, buzzer_toggle);

    while(1)
    {
        sleep(1);
        flag=0;
    }
}

void init_timer(int period, Timer_Mode mode, Timer_PeriodUnits units, Timer_CallBackFxn callback){
    Timer_Params params;
    Timer_Params_init(&params);
    params.period = period;
    params.periodUnits = units;
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
    if(period==500 && flag == 0 ){
        period=1000;
        flag=1;
        Timer_close(timer);
        init_timer(period,Timer_CONTINUOUS_CALLBACK,Timer_PERIOD_HZ,buzzer_toggle);
    }
    if(period==1000 && flag ==0 ){
        period=1500;
        flag=1;
        Timer_close(timer);
        init_timer(period,Timer_CONTINUOUS_CALLBACK,Timer_PERIOD_HZ,buzzer_toggle);
    }
    if(period==1500 && flag ==0 ){
        period=2000;
        flag=1;
        Timer_close(timer);
        init_timer(period,Timer_CONTINUOUS_CALLBACK,Timer_PERIOD_HZ,buzzer_toggle);
    }
    if(period==2000 && flag ==0 ) {
        period=3000;
        flag=1;
        Timer_close(timer);
        init_timer(period,Timer_CONTINUOUS_CALLBACK,Timer_PERIOD_HZ,buzzer_toggle);
    }
    if(period==3000 && flag ==0 ) {
        period=500;
        flag=1;
        Timer_close(timer);
        init_timer(period,Timer_CONTINUOUS_CALLBACK,Timer_PERIOD_HZ,buzzer_toggle);
    }
}


void seven_segment(void){
    i=0;

    GPIO_init();

    /*Seven segment*/
    GPIO_setConfig(Board_GPIO_A, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_B, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_C, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_D, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_E, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_F, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_G, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /*Control signal*/
    GPIO_setConfig(Board_GPIO_7S1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_7S2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    //    singleLED();
    multipleLED();

}

void led_byte(uint8_t byte){
    GPIO_write(Board_GPIO_G,(unsigned int)(1 & (byte >> 6 )));
    GPIO_write(Board_GPIO_F,(unsigned int)(1 & (byte >> 5 )));
    GPIO_write(Board_GPIO_E,(unsigned int)(1 & (byte >> 4 )));
    GPIO_write(Board_GPIO_D,(unsigned int)(1 & (byte >> 3 )));
    GPIO_write(Board_GPIO_C,(unsigned int)(1 & (byte >> 2 )));
    GPIO_write(Board_GPIO_B,(unsigned int)(1 & (byte >> 1 )));
    GPIO_write(Board_GPIO_A,(unsigned int)(1 & (byte >> 0 )));
}

void singleLED(void){
    while (1){
        GPIO_write(Board_GPIO_7S2, GPIO_ON);
        sleep(1);
        led_byte(numbers[i]);
        i++;
        if(i == sizeof(numbers)){
            i = 0;
        }
    }
}
void multipleLED(void){
    init_timer(60,Timer_CONTINUOUS_CALLBACK,Timer_PERIOD_HZ,refreshLED);
    while(1){
        GPIO_write(Board_GPIO_7S1,GPIO_ON);   //turning control signal off
        GPIO_write(Board_GPIO_7S2,GPIO_ON);   //turning control signal off

        led_byte(numbers[i0]);                //data for first 7seg
        GPIO_write(Board_GPIO_7S2,GPIO_OFF);  //turning first 7 seg on
        usleep(5000);                           //delay
        GPIO_write(Board_GPIO_7S2,GPIO_ON);   //turning first 7 seg off

        led_byte(numbers[i]);                //data for second 7seg
        GPIO_write(Board_GPIO_7S1,GPIO_OFF);  //turning second 7 seg on
        usleep(5000);                           //delay
        GPIO_write(Board_GPIO_7S1,GPIO_ON);   //turning second 7 seg off
    }
}

void refreshLED(Timer_Handle myHandle){
    flag++;
    if(flag == 100){
        i0++;
        if(i0 == sizeof(numbers)){
            i0 = 0;
            i++;
            if(i == sizeof(numbers))   i = 0;
        }
        flag =0;
    }
}


