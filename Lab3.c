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

void timer_by_polling(void);

void *mainThread(void *arg0)
{
    GPIO_init();
    Timer_init();

    timer_by_polling();
    //    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
    //    lcd_init_4bit(Board_GPIO_28,Board_GPIO_17,Board_GPIO_16,Board_GPIO_15,Board_GPIO_22,Board_GPIO_25);
}

void timer_by_polling(void){
    Timer_Handle timer;
    Timer_Params params;
    uint32_t poll;
    GPIO_setConfig(Board_GPIO_24, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW );
    Timer_Params_init(&params);
    params.period = 1000;
    params.periodUnits = Timer_PERIOD_HZ;
    params.timerMode = Timer_FREE_RUNNING;
    params.timerCallback = NULL;
    timer = Timer_open(Board_TIMER0, &params);
    Timer_start(timer);

    while (1){
        poll = Timer_getCount(timer);

        if (poll ==0)
            GPIO_write(Board_GPIO_24, GPIO_ON);
    }
}
