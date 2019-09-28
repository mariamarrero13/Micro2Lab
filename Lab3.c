#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Capture.h>
#include <ti/drivers/Timer.h>
#include "Board.h"
#include "lcd.h"

void timer_by_polling(void);
void timer_by_interrupts(void);
void seven_segment(void);
void complimentary_task(void);
void init_timer(int period, Timer_Mode mode, Timer_PeriodUnits units, Timer_CallBackFxn callback);
void led_byte(uint8_t byte);
void singleLED(void);
void multipleLED(void);
uint8_t down_pressed(uint8_t, uint8_t);
uint8_t up_pressed(uint8_t, uint8_t);


/*
 * Global Vars
 */
int i0 = 0;
int i = 0;
int flag =0;
int period = 500;
Timer_Handle timer;
Capture_Handle capture;
Capture_Params captureParams;
uint8_t numbers[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x98, 0x88, 0x80, 0xC6, 0xC0, 0x86, 0x8E};

uint8_t count = 0;
uint8_t text[] = "base string";
uint8_t cursor = 0;
uint8_t olda = 0;
uint8_t oldb = 0;
uint8_t newa = 0;
uint8_t newb = 0;
uint8_t lookup_index =0;
uint32_t interval_v = 0;
/*
 * Interrupts
 */
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
void opto_int(uint_least8_t index)
{
    olda= newa;
    oldb= newb;
    newa =GPIO_read(Board_GPIO_COL2);
    newb =GPIO_read(Board_GPIO_COL3);

    flag = olda<<3|oldb<<2|newa<<1|newb<<0;
    lookup_index = flag;
}

void captureCallback(Capture_Handle handle, uint32_t interval){
    interval_v = interval;

}


/*
 * MAIN
 */
void *mainThread(void *arg0)
{
    GPIO_init();
    Timer_init();
    Capture_init();

    //    timer_by_polling();
    //    timer_by_interrupts();
    //    seven_segment();
    complimentary_task();
}

void timer_by_polling(void)
{
    uint32_t poll = 0;
    uint32_t oldpoll=0;
    GPIO_setConfig(Board_GPIO_24, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    init_timer(250000,Timer_FREE_RUNNING,Timer_PERIOD_US, NULL);

    while (1){
        oldpoll = poll;
        poll =Timer_getCount(timer);
        if (Timer_getCount(timer) == 0){
            printf("\n poll %d", oldpoll);
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
        usleep(6000);                           //delay
        GPIO_write(Board_GPIO_7S2,GPIO_ON);   //turning first 7 seg off

        led_byte(numbers[i]);                //data for second 7seg
        GPIO_write(Board_GPIO_7S1,GPIO_OFF);  //turning second 7 seg on
        usleep(6000);                           //delay
        GPIO_write(Board_GPIO_7S1,GPIO_ON);   //turning second 7 seg off
    }
}

void complimentary_task(void)
{
    GPIO_init();
    uint8_t lookup_table[] = {0,1,2,0,2,0,0,1,1,0,0,2,0,2,1,0};
    uint8_t rpm_s[4];
    Capture_Params_init(&captureParams);
    captureParams.mode = Capture_ANY_EDGE;
    captureParams.periodUnit = Capture_PERIOD_US;
    captureParams.callbackFxn = captureCallback;
    capture = Capture_open(Board_CAPTURE1, &captureParams);
    Capture_start(capture);

    GPIO_setConfig(Board_GPIO_COL2, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_BOTH_EDGES);
    GPIO_setConfig(Board_GPIO_COL3, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_BOTH_EDGES);
    GPIO_setCallback(Board_GPIO_COL2, opto_int);
    GPIO_setCallback(Board_GPIO_COL3, opto_int);
    GPIO_enableInt(Board_GPIO_COL2);
    GPIO_enableInt(Board_GPIO_COL3);

    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    usleep(50);
    lcd_string("Start Spinning");


    while(1){
        uint32_t rpm = 3750000/interval_v;
        if (!(rpm > 9999))
            ltoa(rpm,rpm_s);

        if(lookup_table[lookup_index] == 1){          //moved forward
            lcd_command(Clear);
            usleep(50);
            lcd_string("Speed= ");
            lcd_string(rpm_s);
            lcd_string(" RPM");
            lcd_command(lcd_SetCursor|lcd_LineTwo);
            usleep(50);
            lcd_string("Clockwise");
            usleep(50);
            lookup_index =0;

        }
        if(lookup_table[lookup_index] == 2){          //moved backward
            lcd_command(Clear);
            usleep(50);
            lcd_string("Speed= ");
            lcd_string(rpm_s);
            lcd_string(" RPM");
            lcd_command(lcd_SetCursor|lcd_LineTwo);
            usleep(50);
            lcd_string("Co Clockwise");
            usleep(50);
            lookup_index =0;
        }

    }
}


