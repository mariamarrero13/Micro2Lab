/*
 * Copyright (c) 2016-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __BOARD_H
#define __BOARD_H

#define Board_CC3220S_LAUNCHXL

#ifdef __cplusplus
extern "C" {
#endif

#include <ti/drivers/Board.h>

#define Board_initGeneral()     Board_init()  /* deprecated */

#include "CC3220S_LAUNCHXL.h"

/* These #defines allow us to reuse TI-RTOS across other device families */

#define Board_ADC0                   CC3220S_LAUNCHXL_ADC0
#define Board_ADC1                   CC3220S_LAUNCHXL_ADC1

#define Board_CAPTURE0               CC3220S_LAUNCHXL_CAPTURE0
#define Board_CAPTURE1               CC3220S_LAUNCHXL_CAPTURE1

#define Board_CRYPTO0                CC3220S_LAUNCHXL_CRYPTO0

#define Board_GPIO_BUTTON0           CC3220S_LAUNCHXL_GPIO_SW2
#define Board_GPIO_BUTTON1           CC3220S_LAUNCHXL_GPIO_PB1

#define rotary_DT                    CC3220S_LAUNCHXL_GPIO_DT
#define rotary_CLK                   CC3220S_LAUNCHXL_GPIO_CLK

#define Board_GPIO_LED_ON            CC3220S_LAUNCHXL_GPIO_LED_ON
#define Board_GPIO_LED_OFF           CC3220S_LAUNCHXL_GPIO_LED_OFF
//#define Board_GPIO_24                CC3220S_LAUNCHXL_GPIO_PB1
#define Board_GPIO_COL1              CC3220S_LAUNCHXL_GPIO_COL1
#define Board_GPIO_COL2              CC3220S_LAUNCHXL_GPIO_COL2
#define Board_GPIO_COL3              CC3220S_LAUNCHXL_GPIO_COL3
#define Board_GPIO_06                CC3220S_LAUNCHXL_GPIO_PB2

/* LCD GPIO definition */
#define Board_GPIO_28                CC3220S_LAUNCHXL_GPIO_D7
#define Board_GPIO_17                CC3220S_LAUNCHXL_GPIO_D6
#define Board_GPIO_16                CC3220S_LAUNCHXL_GPIO_D5
#define Board_GPIO_15                CC3220S_LAUNCHXL_GPIO_D4
#define Board_GPIO_22                CC3220S_LAUNCHXL_GPIO_RS
#define Board_GPIO_25                CC3220S_LAUNCHXL_GPIO_EN
#define Board_GPIO_ROW1              CC3220S_LAUNCHXL_GPIO_ROW1
#define Board_GPIO_ROW2              CC3220S_LAUNCHXL_GPIO_ROW2
#define Board_GPIO_ROW3              CC3220S_LAUNCHXL_GPIO_ROW3
#define Board_GPIO_ROW4              CC3220S_LAUNCHXL_GPIO_ROW4

/* Lab 5 */

//#define mot_bot1                     CC3220S_LAUNCHXL_GPIO_Bot1
//#define mot_bot2                     CC3220S_LAUNCHXL_GPIO_Bot2
//#define mot_bot3                     CC3220S_LAUNCHXL_GPIO_Bot3
#define mot1                         CC3220S_LAUNCHXL_GPIO_Mot1
#define mot2                         CC3220S_LAUNCHXL_GPIO_Mot2

#define Board_GPIO_ORANGE            CC3220S_LAUNCHXL_GPIO_ORANGE
#define Board_GPIO_YELLOW            CC3220S_LAUNCHXL_GPIO_YELLOW
#define Board_GPIO_PINK              CC3220S_LAUNCHXL_GPIO_PINK
#define Board_GPIO_BLUE              CC3220S_LAUNCHXL_GPIO_BLUE

///*SEVEN SEGMENT*/
//#define Board_GPIO_A              CC3220S_LAUNCHXL_GPIO_A
//#define Board_GPIO_B              CC3220S_LAUNCHXL_GPIO_B
//#define Board_GPIO_C              CC3220S_LAUNCHXL_GPIO_C
//#define Board_GPIO_D              CC3220S_LAUNCHXL_GPIO_D
//#define Board_GPIO_E              CC3220S_LAUNCHXL_GPIO_E
//#define Board_GPIO_F              CC3220S_LAUNCHXL_GPIO_F
//#define Board_GPIO_G              CC3220S_LAUNCHXL_GPIO_G

///*Transistors*/
//#define Board_GPIO_7S1            CC3220S_LAUNCHXL_GPIO_7S1
//#define Board_GPIO_7S2            CC3220S_LAUNCHXL_GPIO_7S2

/*
 *  CC3220S_LAUNCHXL_GPIO_LED_D8 and CC3220S_LAUNCHXL_GPIO_LED_D9 are shared with the I2C
 *  and PWM peripherals. In order for those examples to work, these LEDs are
 *  taken out of gpioPinCOnfig[]
 */
//#define Board_GPIO_LED1              CC3220S_LAUNCHXL_GPIO_LED_D10
//#define Board_GPIO_LED2              CC3220S_LAUNCHXL_GPIO_LED_D10
//
//#define Board_GPIO_TMP116_EN         CC3220S_LAUNCHXL_GPIO_TMP116_EN

#define Board_I2C0                   CC3220S_LAUNCHXL_I2C0
#define Board_I2C_TMP                CC3220S_LAUNCHXL_I2C0

#define Board_I2S0                   CC3220S_LAUNCHXL_I2S0

#define Board_PWM0                   CC3220S_LAUNCHXL_PWM6
#define Board_PWM1                   CC3220S_LAUNCHXL_PWM7
#define Board_PWM2                   CC3220S_LAUNCHXL_PWM8

#define Board_SD0                    CC3220S_LAUNCHXL_SD0

#define Board_SDFatFS0               CC3220S_LAUNCHXL_SD0

/* CC3220S_LAUNCHXL_SPI0 is reserved for the NWP */
#define Board_SPI0                   CC3220S_LAUNCHXL_SPI1

#define Board_SPI_MASTER             CC3220S_LAUNCHXL_SPI1
#define Board_SPI_SLAVE              CC3220S_LAUNCHXL_SPI1
#define Board_SPI_MASTER_READY       CC3220S_LAUNCHXL_SPI_MASTER_READY
#define Board_SPI_SLAVE_READY        CC3220S_LAUNCHXL_SPI_SLAVE_READY

#define Board_TIMER0                 CC3220S_LAUNCHXL_TIMER0
#define Board_TIMER1                 CC3220S_LAUNCHXL_TIMER1
#define Board_TIMER2                 CC3220S_LAUNCHXL_TIMER2

#define Board_UART0                  CC3220S_LAUNCHXL_UART0
#define Board_UART1                  CC3220S_LAUNCHXL_UART1

#define Board_WATCHDOG0              CC3220S_LAUNCHXL_WATCHDOG0

/* Board specific I2C address */
#define Board_TMP006_ADDR            (0x41)
#define Board_I2C_TMP006_ADDR        Board_TMP006_ADDR


#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H */
