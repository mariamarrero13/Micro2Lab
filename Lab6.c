/*
 * Lab6.c
 *
 *  Created on: Oct 27, 2019
 *      Author: maria
 */

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

void uart();

void *mainThread(void *arg0){
    uart();
    return (NULL);
}

void uart(){

}


