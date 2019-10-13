/* Example/Board Header files */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Capture.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/PWM.h>
#include "Board.h"
#include "lcd.h"

void LowPowerMode(void);
void PWMGenerator(void);
void RGBGenerator(void);
void complimentary_task(void);


void keypad(void);
void key_write(uint8_t);
void decode_stroke(uint_least8_t index);
void PWMod_init(void);
void update_duty(uint8_t new_duty);


uint8_t count = 0;
uint8_t flag = 1;
uint8_t text[] = "base string";
uint8_t cursor = 0;
uint8_t ppast_duty = 1;
uint8_t past_duty = 1;
uint8_t pres_duty = 0;
uint8_t lookup_index =0;
PWM_Handle pwm1 = NULL;

void keypadint(uint_least8_t index) {
    GPIO_write(Board_GPIO_ROW1,GPIO_OFF);
    GPIO_write(Board_GPIO_ROW2,GPIO_OFF);
    GPIO_write(Board_GPIO_ROW3,GPIO_OFF);
    GPIO_write(Board_GPIO_ROW4,GPIO_OFF);
    GPIO_disableInt(Board_GPIO_COL1);
    GPIO_disableInt(Board_GPIO_COL2);
    GPIO_disableInt(Board_GPIO_COL3);

    decode_stroke(index);



    GPIO_enableInt(Board_GPIO_COL1);
    GPIO_enableInt(Board_GPIO_COL2);
    GPIO_enableInt(Board_GPIO_COL3);
    GPIO_write(Board_GPIO_ROW1,GPIO_ON);
    GPIO_write(Board_GPIO_ROW2,GPIO_ON);
    GPIO_write(Board_GPIO_ROW3,GPIO_ON);
    GPIO_write(Board_GPIO_ROW4,GPIO_ON);
}

void *mainThread(void *arg0){
    //    PWMGenerator();
    complimentary_task();
}

void LowPowerMode(){

}

//PWM Generator
void PWMGenerator(){
    uint16_t   pwmPeriod = 125;
    uint16_t   duty = 63;

    uint32_t   time = 50000;
    PWM_Handle pwm1 = NULL;
    PWM_Params params;

    PWM_init();

    PWM_Params_init(&params);
    params.dutyUnits = PWM_DUTY_US;
    params.dutyValue = 0;
    params.periodUnits = PWM_PERIOD_US;
    params.periodValue = pwmPeriod;
    pwm1 = PWM_open(Board_PWM0, &params);
    if (pwm1 == NULL) {
        while (1);
    }

    PWM_start(pwm1);

    while (1) {
        PWM_setDuty(pwm1, duty);
        usleep(time);
    }
}

void RGBGenerator(){

}

void complimentary_task(){

    keypad();
    PWM_init();
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    PWMod_init();
    key_write(0);
    GPIO_write(Board_GPIO_ROW1,GPIO_ON);
    GPIO_write(Board_GPIO_ROW2,GPIO_ON);
    GPIO_write(Board_GPIO_ROW3,GPIO_ON);
    GPIO_write(Board_GPIO_ROW4,GPIO_ON);
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

    GPIO_setCallback(Board_GPIO_COL1, keypadint);
    GPIO_setCallback(Board_GPIO_COL2, keypadint);
    GPIO_setCallback(Board_GPIO_COL3, keypadint);
    GPIO_enableInt(Board_GPIO_COL1);
    GPIO_enableInt(Board_GPIO_COL2);
    GPIO_enableInt(Board_GPIO_COL3);
}


void key_write(uint8_t number){


    //    lcd_command(Clear);
    lcd_command(lcd_SetCursor|lcd_LineOne);
    //    lcd_string("hello");
    if(number == 0) lcd_string("0 -> 0%");
    else if(number == 10) lcd_string("1 -> 10%");
    else if(number == 20) lcd_string("2 -> 20%");
    else if(number == 30) lcd_string("3 -> 30%");
    else if(number == 40) lcd_string("4 -> 40%");
    else if(number == 50) lcd_string("5 -> 50%");
    else if(number == 60) lcd_string("6 -> 60%");
    else if(number == 70) lcd_string("7 -> 70%");
    else if(number == 80) lcd_string("8 -> 80%");
    else if(number == 90) lcd_string("9 -> 90%");
    else  lcd_string("# -> 100%");
    usleep(500);

}

void PWMod_init(){
    uint16_t   pwmPeriod = 1000;
    uint32_t   time = 50000;
    PWM_Params params;
    PWM_Params_init(&params);
    params.dutyUnits = PWM_DUTY_US;
    params.dutyValue = 0;
    params.periodUnits = PWM_PERIOD_US;
    params.periodValue = pwmPeriod;
    pwm1 = PWM_open(Board_PWM0, &params);

    PWM_start(pwm1);


}
void update_duty(uint8_t new_duty){

    if (ppast_duty != past_duty && pres_duty!= new_duty){
        ppast_duty = pres_duty;
        pres_duty = new_duty;

    }
    if (past_duty != pres_duty){
        PWM_stop(pwm1);
        key_write(pres_duty);
        PWM_start(pwm1);
        PWM_setDuty(pwm1, (pres_duty * 1000)/100);
        past_duty=pres_duty;

    }

}

void decode_stroke(uint_least8_t index){
    usleep(40);
    {
        GPIO_write(Board_GPIO_ROW1,GPIO_ON);
        GPIO_write(Board_GPIO_ROW2,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW3,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW4,GPIO_OFF);
        if(GPIO_read(Board_GPIO_COL1)) {update_duty(10); return;}
        if(GPIO_read(Board_GPIO_COL2)) {update_duty(20); return;}
        if(GPIO_read(Board_GPIO_COL3)) {update_duty(30); return;}
    }
    {
        GPIO_write(Board_GPIO_ROW1,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW2,GPIO_ON);
        GPIO_write(Board_GPIO_ROW3,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW4,GPIO_OFF);
        if(GPIO_read(Board_GPIO_COL1)) {update_duty(40); return;}
        if(GPIO_read(Board_GPIO_COL2)) {update_duty(50); return;}
        if(GPIO_read(Board_GPIO_COL3)) {update_duty(60); return;}
    }
    {
        GPIO_write(Board_GPIO_ROW1,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW2,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW3,GPIO_ON);
        GPIO_write(Board_GPIO_ROW4,GPIO_OFF);
        if(GPIO_read(Board_GPIO_COL1)) {update_duty(70); return;}
        if(GPIO_read(Board_GPIO_COL2)) {update_duty(80); return;}
        if(GPIO_read(Board_GPIO_COL3)) {update_duty(90); return;}
    }
    {
        GPIO_write(Board_GPIO_ROW1,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW2,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW3,GPIO_OFF);
        GPIO_write(Board_GPIO_ROW4,GPIO_ON);
        if(GPIO_read(Board_GPIO_COL2)) {update_duty(0); return;}
        if(GPIO_read(Board_GPIO_COL3)) {update_duty(100); return;}
    }
}
