/* Example/Board Header files */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Capture.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/PWM.h>
#include "Board.h"
#include "lcd.h"


void LowPowerMode(void);
void PWMGenerator(void);
void RGBGenerator(void);
void complimentary_task(void);

void keypad(void);
void key_write(uint8_t);
void decode_stroke();
void PWMod_init(void);
void update_duty(uint8_t new_duty);
uint8_t down_pressed(uint8_t index,uint8_t size);
uint8_t up_pressed(uint8_t index,uint8_t size);
void test();
const int STRINGS = 16;
uint8_t text[STRINGS][16] = {"1. Maria","2. Alejandra","3. Marrero", "4. Ortiz",
                             "5. Yamil", "6. Jose", "7. Gonzalez", "8. Zuaznabar",
                             "9. Diego", "10. Jose","11. Amador", "12. Bonilla",
                             "13. Christian", "14. Antonio", "15. Santiago", "16. Berio"};
uint8_t lookup_table_red[] = {0,0,1000,1000,118,941,1000,1000};
uint8_t lookup_table_green[] = {0,1000,0,118,871,784,482,1000};
uint8_t lookup_table_blue[] = {1000,0,0,851,988,156,129,1000};

int color = 0;
uint8_t flag = 1;
uint8_t olda = 0;
uint8_t oldb = 0;
uint8_t newa = 0;
uint8_t newb = 0;
uint8_t cursor = 0;
uint8_t ppast_duty = 1;
uint8_t past_duty = 1;
uint8_t pres_duty = 0;
uint8_t lookup_index =0;
PWM_Handle pwm1 = NULL;
PWM_Handle pwm2 = NULL;
PWM_Handle pwm3 = NULL;

void gpioButtonFxn1(uint_least8_t index){
    color++;
    if(color > sizeof(lookup_table_red)){
        color = 0;
    }
    PWM_setDuty(pwm1, lookup_table_red[color]);
    PWM_setDuty(pwm2, lookup_table_blue[color]);
    PWM_setDuty(pwm3, lookup_table_green[color]);
}
void opto_int(uint_least8_t index)
{
    olda= newa;
    oldb= newb;
    newa =GPIO_read(Board_GPIO_BUTTON0);
    newb =GPIO_read(Board_GPIO_BUTTON1);

    flag = olda<<3|oldb<<2|newa<<1|newb<<0;
    lookup_index = flag;
}
void rotary_int(uint_least8_t index)
{
    olda= newa;
    oldb= newb;
    newa =GPIO_read(rotary_DT);
    newb =GPIO_read(rotary_CLK);

    flag = olda<<3|oldb<<2|newa<<1|newb<<0;
    lookup_index = flag;
}

void *mainThread(void *arg0){
    //    LowPowerMode();
    //    PWMGenerator();
    //    RGBGenerator();
    //    complimentary_task();
    test();
    return (NULL);
}

void LowPowerMode()
{
    GPIO_init();
    uint8_t positions[] = {0,1};
    uint8_t lookup_table[] = {0,1,2,0,2,0,0,1,1,0,0,2,0,2,1,0};
    GPIO_setConfig(Board_GPIO_BUTTON0, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_BOTH_EDGES);
    GPIO_setConfig(Board_GPIO_BUTTON1, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_BOTH_EDGES);
    GPIO_setCallback(Board_GPIO_BUTTON0, opto_int);
    GPIO_setCallback(Board_GPIO_BUTTON1, opto_int);
    GPIO_enableInt(Board_GPIO_BUTTON0);
    GPIO_enableInt(Board_GPIO_BUTTON1);

    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
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
        Power_enablePolicy();
    }
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
    /**GPIO definitions**/
    GPIO_init();

    GPIO_setConfig(Board_GPIO_BUTTON0, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_RISING);
    GPIO_setCallback(Board_GPIO_BUTTON0, gpioButtonFxn1);
    GPIO_enableInt(Board_GPIO_BUTTON0);

    /* Period at Hz and duty in microseconds */
    uint16_t   pwmPeriod = 1000;

    /* Sleep time in microseconds */
    uint32_t   time = 50000;
    PWM_Params params;

    PWM_init();

    PWM_Params_init(&params);
    params.dutyUnits = PWM_DUTY_US;
    params.dutyValue = 0;
    params.periodUnits = PWM_PERIOD_HZ;
    params.periodValue = pwmPeriod;


    pwm1 = PWM_open(Board_PWM0, &params);
    if (pwm1 == NULL) {
        while (1);
    }

    PWM_start(pwm1);

    pwm2 = PWM_open(Board_PWM1, &params);
    if (pwm2 == NULL) {
        while (1);
    }

    PWM_start(pwm2);

    pwm3 = PWM_open(Board_PWM2, &params);
    if (pwm3 == NULL) {
        while (1);
    }

    PWM_start(pwm3);

    PWM_setDuty(pwm1, lookup_table_red[color]);
    PWM_setDuty(pwm2, lookup_table_blue[color]);
    PWM_setDuty(pwm3, lookup_table_green[color]);
    while(1){

        sleep(30);
    }
    //usleep(time);
}

void complimentary_task(){

    keypad();
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    PWMod_init();
    key_write(0);
    while(1){
        if(GPIO_read(Board_GPIO_COL1)||GPIO_read(Board_GPIO_COL2)||GPIO_read(Board_GPIO_COL3))
        {
            decode_stroke();
            usleep(5);
            GPIO_write(Board_GPIO_ROW1,GPIO_ON);
            GPIO_write(Board_GPIO_ROW2,GPIO_ON);
            GPIO_write(Board_GPIO_ROW3,GPIO_ON);
            GPIO_write(Board_GPIO_ROW4,GPIO_ON);
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
}


void key_write(uint8_t number){
    lcd_command(lcd_SetCursor|lcd_LineOne);
    if(number == 0) lcd_string("0 -> 0%  ");
    else if(number == 10) lcd_string("1 -> 10% ");
    else if(number == 20) lcd_string("2 -> 20% ");
    else if(number == 30) lcd_string("3 -> 30% ");
    else if(number == 40) lcd_string("4 -> 40% ");
    else if(number == 50) lcd_string("5 -> 50% ");
    else if(number == 60) lcd_string("6 -> 60% ");
    else if(number == 70) lcd_string("7 -> 70% ");
    else if(number == 80) lcd_string("8 -> 80% ");
    else if(number == 90) lcd_string("9 -> 90% ");
    else  lcd_string("# -> 100%");
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
void update_duty(uint8_t new_duty){

    if (ppast_duty != past_duty && pres_duty!= new_duty){
        ppast_duty = pres_duty;
        pres_duty = new_duty;

    }
    if (past_duty != pres_duty){
        key_write(pres_duty);
        PWM_setDuty(pwm1, (pres_duty * 1000)/100);
        past_duty=pres_duty;

    }

}

void decode_stroke(){
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

void test()
{
    uint8_t encoder_count;
    uint8_t lookup_table[] = {0,1,2,0,2,0,0,1,1,0,0,2,0,2,1,0};
    GPIO_init();
    GPIO_setConfig(rotary_DT, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_BOTH_EDGES);
    GPIO_setConfig(rotary_CLK, GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_BOTH_EDGES);
    GPIO_setCallback(rotary_DT, rotary_int);
    GPIO_setCallback(rotary_CLK, rotary_int);
    GPIO_enableInt(rotary_DT);
    GPIO_enableInt(rotary_CLK);

    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    lcd_string("Start rotating");


    while(1){
        if(lookup_table[lookup_index] == 1){          //moved forward
            if(encoder_count != 0)
                encoder_count--;
            printf("count: %d\n", encoder_count);
            lcd_command(Clear);
            lcd_string("CCW");
            lookup_index=0;

        }
        if(lookup_table[lookup_index] == 2){          //moved backward
            encoder_count++;
            printf("count: %d\n", encoder_count);
            lcd_command(Clear);
            lcd_string("CW");
            lookup_index=0;
        }

    }
}



