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
#include "Board.h"
#include "lcd.h"

PWM_Handle pwm1 = NULL;
PWM_Handle pwm2 = NULL;
uint8_t text[] = "base string";

void dc_motor_trans();
void dc_motor_pwm();
void servo_motor();
void clear_line(int line);
void set_duty(uint8_t motor_on,uint32_t duty);
void duty_onscreen(uint8_t duty);

void *mainThread(void *arg0){
    //    dc_motor_trans();
    //    dc_motor_pwm();
    servo_motor();
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

    while (1) {
        GPIO_write(Board_GPIO_ORANGE, lookup_table_orange[index]);
        GPIO_write(Board_GPIO_YELLOW, lookup_table_yellow[index]);
        GPIO_write(Board_GPIO_PINK, lookup_table_pink[index]);
        GPIO_write(Board_GPIO_BLUE, lookup_table_blue[index]);
        if(index < 4){
            index++;
        }
        else{
            index = 0;
        }
        usleep(time);
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
