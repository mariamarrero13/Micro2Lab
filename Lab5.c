/*
 * Lab5.c
 *
 *  Created on: Oct 18, 2019
 *      Author: maria
 */




/* Example/Board Header files */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/Timer.h>
#include "Board.h"
#include "lcd.h"

PWM_Handle pwm1 = NULL;
PWM_Handle pwm2 = NULL;
uint8_t text[] = "base string";
uint8_t motor_on = 0;
int index = 0;
uint32_t steps = 0;
Timer_Handle timer;
uint32_t sleep_time[] = {10000,5000,2000, 1000, 500, 200, 100, 50, 20, 10};
uint8_t tme = 0;

uint8_t lookup_table_orange[] = {1,1,0,0,0,0,0,1};
uint8_t lookup_table_yellow[] = {0,1,1,1,0,0,0,0};
uint8_t lookup_table_pink[] = {0,0,0,1,1,1,0,0};
uint8_t lookup_table_blue[] = {0,0,0,0,0,1,1,1};
//    uint8_t lookup_table_orange[] = {1,0,0,0};
//    uint8_t lookup_table_yellow[] = {0,1,0,0};
//    uint8_t lookup_table_pink[] = {0,0,1,0};
//    uint8_t lookup_table_blue[] = {0,0,0,1};

void dc_motor_trans();
void dc_motor_pwm();
void servo_motor();
void stepper_motor();
void complimentary_task();
void clear_line(int line);
void set_duty(uint8_t motor_on,uint32_t duty);
void duty_onscreen(uint8_t duty);
void init_timer(int period, Timer_Mode mode, Timer_PeriodUnits units, Timer_CallBackFxn callback);

void timer_int(Timer_Handle myHandle){
    if(motor_on==1){
        if(steps<=2300){
            GPIO_write(Board_GPIO_ORANGE, lookup_table_orange[index]);
            GPIO_write(Board_GPIO_YELLOW, lookup_table_yellow[index]);
            GPIO_write(Board_GPIO_PINK, lookup_table_pink[index]);
            GPIO_write(Board_GPIO_BLUE, lookup_table_blue[index]);
            steps++;
            if(index < 8){
                index++;
            }
            else{
                index = 0;
            }
        }
    }
}
void button_one(uint_least8_t index)
{
    usleep(5000);
    puts("entered one");
    if(tme != 10) {
        tme ++;
        //                puts("entered plus");
        steps = 0;
        Timer_close(timer);
        init_timer(sleep_time[tme],Timer_CONTINUOUS_CALLBACK,Timer_PERIOD_US, timer_int);
    }
}
void button_two(uint_least8_t index){
    usleep(5000);
    puts("entered two");
    if(tme != 0) {
        tme --;
        //                puts("entered minus");
        steps =0;

        Timer_close(timer);
        init_timer(sleep_time[tme],Timer_CONTINUOUS_CALLBACK,Timer_PERIOD_US, timer_int);
    }
}
void button_three(uint_least8_t index){
    usleep(5000);
    puts("entered three");
    if(motor_on == 1) {
        motor_on = 0;
        Timer_close(timer);
        //                puts("off");
        GPIO_write(Board_GPIO_ORANGE, 0);
        GPIO_write(Board_GPIO_YELLOW, 0);
        GPIO_write(Board_GPIO_PINK, 0);
        GPIO_write(Board_GPIO_BLUE, 0);
    }
    else {
        //                puts("on");
        motor_on = 1;
        steps = 0;
        init_timer(sleep_time[tme],Timer_CONTINUOUS_CALLBACK,Timer_PERIOD_US, timer_int);
    }
}

void *mainThread(void *arg0){
    //        dc_motor_trans();
    //        dc_motor_pwm();
    servo_motor();
    //         stepper_motor();
    //    complimentary_task();
    return (NULL);
}

void dc_motor_trans(){
    GPIO_init();
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    GPIO_setConfig(mot_bot1, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_RISING);
    GPIO_setConfig(mot_bot2, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_RISING);
    GPIO_setConfig(mot_bot3, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_RISING);
    GPIO_setConfig(mot1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(mot2, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    lcd_string("Button 1");
    lcd_command(lcd_SetCursor|lcd_LineTwo);
    lcd_string("Stop Free");
    while(1){
        if(GPIO_read(mot_bot1)==0){
            GPIO_write(mot1, 0);
            GPIO_write(mot2, 0);
            lcd_command(Clear);
            lcd_string("Button 1");
            lcd_command(lcd_SetCursor|lcd_LineTwo);
            lcd_string("Stop Free");
        }
        if(GPIO_read(mot_bot2)){
            GPIO_write(mot1, 0);
            GPIO_write(mot2, 1);
            lcd_command(Clear);
            lcd_string("Button 2");
            lcd_command(lcd_SetCursor|lcd_LineTwo);
            lcd_string("Rot. Left");
        }
        if(GPIO_read(mot_bot3)){
            GPIO_write(mot1, 1);
            GPIO_write(mot2, 0);
            lcd_command(Clear);
            lcd_string("Button 3");
            lcd_command(lcd_SetCursor|lcd_LineTwo);
            lcd_string("Rot. Right");
        }
    }
}

void dc_motor_pwm(){
    uint32_t duty_lookup[] = {0,200, 400, 600, 800, 1000};
    uint8_t duty = 1;
    uint8_t motor_on = 0;
    uint8_t word[] = "Duty: 200";
    GPIO_init();
    PWM_init();
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    GPIO_setConfig(mot_bot1, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_RISING);
    GPIO_setConfig(mot_bot2, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_RISING);
    GPIO_setConfig(mot_bot3, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_RISING);


    uint16_t   pwmPeriod = 1000; // period in Hz

    PWM_Params params;
    PWM_Params_init(&params);
    params.dutyUnits = PWM_DUTY_US;
    params.dutyValue = 0;
    params.periodUnits = PWM_PERIOD_HZ;
    params.periodValue = pwmPeriod;


    pwm1 = PWM_open(Board_PWM0, &params);
    pwm2 = PWM_open(Board_PWM1, &params);
    PWM_start(pwm1);
    PWM_start(pwm2);

    PWM_setDuty(pwm1, 0);
    PWM_setDuty(pwm2, duty);

    lcd_string(word);
    lcd_command(lcd_SetCursor|lcd_LineTwo);
    lcd_string("Rot. Left");


    while(1){
        if(GPIO_read(mot_bot1)==0){
            if(duty != 5) duty ++;
            set_duty(motor_on, duty_lookup[duty]);
            duty_onscreen(duty);
        }
        if(GPIO_read(mot_bot2)){
            if(duty != 0) duty --;
            set_duty(motor_on, duty_lookup[duty]);
            duty_onscreen(duty);
        }
        if(GPIO_read(mot_bot3)){
            if(motor_on == 1) {
                motor_on = 0;
                clear_line(2);
                lcd_string("Rot. Right");
            }
            else {
                motor_on = 1;
                clear_line(2);
                lcd_string("Rot. Left");
            }
            set_duty(motor_on, duty_lookup[duty]);
        }
    }
}
void servo_motor()
{
    //    uint32_t duty_lookup[] = {300, 525,750,975,1200,1425, 1650, 1875, 2100, 1875, 1650, 1425,1200, 975,750, 525};
    uint32_t duty_lookup[] = {250, 525, 1425, 1200,1650, 750, 2100, 1875, 975};
    uint8_t duty = 0;
    PWM_init();
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);

    uint16_t   pwmPeriod = 50; // period in Hz

    PWM_Params params;
    PWM_Params_init(&params);
    params.dutyUnits = PWM_DUTY_US;
    params.dutyValue = 1000;
    params.periodUnits = PWM_PERIOD_HZ;
    params.periodValue = pwmPeriod;


    pwm1 = PWM_open(Board_PWM0, &params);
    PWM_start(pwm1);

    PWM_setDuty(pwm1, duty_lookup[duty]);

    while(1)
    {
        sleep(4);
        if(++duty == 9) duty = 0;
        PWM_setDuty(pwm1, duty_lookup[duty]);
    }
}
void stepper_motor()
{
    uint8_t lookup_table_orange[] = {1,1,0,0,0,0,0,1};
    uint8_t lookup_table_yellow[] = {0,1,1,1,0,0,0,0};
    uint8_t lookup_table_pink[] = {0,0,0,1,1,1,0,0};
    uint8_t lookup_table_blue[] = {0,0,0,0,0,1,1,1};

    uint8_t lookup_table_orange_inv[] =  {0,0,0,0,0,1,1,1};
    uint8_t lookup_table_yellow_inv[] = {0,0,0,1,1,1,0,0};
    uint8_t lookup_table_pink_inv[] = {0,1,1,1,0,0,0,0};
    uint8_t lookup_table_blue_inv[] = {1,1,0,0,0,0,0,1};
    int index = 0;
    /* 1 second delay */
    uint32_t time = 10000;

    /* Call driver init functions */
    GPIO_init();

    GPIO_setConfig(Board_GPIO_ORANGE, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_YELLOW, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_PINK, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_BLUE, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Turn on user LED */
    //GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);
    steps =0;
    while (1) {
        while(steps<864){
            GPIO_write(Board_GPIO_ORANGE, lookup_table_orange[index]);
            GPIO_write(Board_GPIO_YELLOW, lookup_table_yellow[index]);
            GPIO_write(Board_GPIO_PINK, lookup_table_pink[index]);
            GPIO_write(Board_GPIO_BLUE, lookup_table_blue[index]);
            if(index < 8){
                index++;
            }
            else{
                index = 0;
            }
            usleep(time);
            steps++;
        }
        steps =0;
        sleep(1);
        while (steps<576){
            GPIO_write(Board_GPIO_ORANGE, lookup_table_orange[index]);
            GPIO_write(Board_GPIO_YELLOW, lookup_table_yellow[index]);
            GPIO_write(Board_GPIO_PINK, lookup_table_pink[index]);
            GPIO_write(Board_GPIO_BLUE, lookup_table_blue[index]);
            if(index < 8){
                index++;
            }
            else{
                index = 0;
            }
            usleep(time);
            steps++;
        }
        steps =0;
        sleep(1);
        while(steps<288){
            GPIO_write(Board_GPIO_ORANGE, lookup_table_orange_inv[index]);
            GPIO_write(Board_GPIO_YELLOW, lookup_table_yellow_inv[index]);
            GPIO_write(Board_GPIO_PINK, lookup_table_pink_inv[index]);
            GPIO_write(Board_GPIO_BLUE, lookup_table_blue_inv[index]);
            if(index < 8){
                index++;
            }
            else{
                index = 0;
            }
            usleep(time);
            steps++;
        }
        sleep(2);
    }
}
void complimentary_task()
{
    //    uint8_t lookup_table_orange[] = {1,1,0,0,0,0,0,1};
    //    uint8_t lookup_table_yellow[] = {0,1,1,1,0,0,0,0};
    //    uint8_t lookup_table_pink[] = {0,0,0,1,1,1,0,0};
    //    uint8_t lookup_table_blue[] = {0,0,0,0,0,1,1,1};
    //    uint8_t lookup_table_orange[] = {1,0,0,0};
    //    uint8_t lookup_table_yellow[] = {0,1,0,0};
    //    uint8_t lookup_table_pink[] = {0,0,1,0};
    //    uint8_t lookup_table_blue[] = {0,0,0,1};
    GPIO_init();
    Timer_init();
    GPIO_setConfig(Board_GPIO_ORANGE, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_YELLOW, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_PINK, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO_BLUE, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(mot_bot1, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_LOW);
    GPIO_setConfig(mot_bot2, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_HIGH);
    GPIO_setConfig(mot_bot3, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_HIGH);
    GPIO_setCallback(mot_bot1, button_one);
    GPIO_setCallback(mot_bot2, button_two);
    GPIO_setCallback(mot_bot3, button_three);
    GPIO_enableInt(mot_bot1);
    GPIO_enableInt(mot_bot2);
    GPIO_enableInt(mot_bot3);
    //    init_timer(sleep_time[tme],Timer_CONTINUOUS_CALLBACK,Timer_PERIOD_US, timer_int);
    while (1) {
    }
}

/**
 * Helper Methods
 * */
void set_duty(uint8_t motor_on,uint32_t duty){
    if(motor_on == 0){
        PWM_setDuty(pwm1, duty);
        PWM_setDuty(pwm2, 0);
    }
    else{
        PWM_setDuty(pwm1, 0);
        PWM_setDuty(pwm2, duty);
    }
}
void clear_line(int line){
    if(line == 1){
        lcd_command(lcd_SetCursor|lcd_LineOne);
        lcd_string("                ");
        lcd_command(lcd_SetCursor|lcd_LineOne);
    }
    else{
        lcd_command(lcd_SetCursor|lcd_LineTwo);
        lcd_string("                ");
        lcd_command(lcd_SetCursor|lcd_LineTwo);
    }
}
void duty_onscreen(uint8_t duty){
    clear_line(1);
    if(duty== 0)  lcd_string("Duty: 0");
    else if(duty== 1)  lcd_string("Duty: 200");
    else if(duty== 2)  lcd_string("Duty: 400");
    else if(duty== 3)  lcd_string("Duty: 600");
    else if(duty== 4)  lcd_string("Duty: 800");
    else if(duty== 5)  lcd_string("Duty: 1000");
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
