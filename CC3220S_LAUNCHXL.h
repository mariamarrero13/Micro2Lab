/*
 * Copyright (c) 2016-2018, Texas Instruments Incorporated
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
/** ============================================================================
 *  @file       C3220S_LAUNCHXL.h
 *
 *  @brief      CC3220 Board Specific APIs
 *
 *  The CC3220S_LAUNCHXL header file should be included in an application as
 *  follows:
 *  @code
 *  #include <CC3220S_LAUNCHXL.h>
 *  @endcode
 *
 *  ============================================================================
 */
#ifndef __CC3220S_LAUNCHXL_H
#define __CC3220S_LAUNCHXL_H

#ifdef __cplusplus
extern "C" {
#endif

#define CC3220S_LAUNCHXL_GPIO_LED_OFF (0)
#define CC3220S_LAUNCHXL_GPIO_LED_ON  (1)
#define GPIO_ON (1)
#define GPIO_OFF (0)

/*!
 *  @def    CC3220S_LAUNCHXL_ADCName
 *  @brief  Enum of ADC names on the CC3220S_LAUNCHXL dev board
 */
typedef enum CC3220S_LAUNCHXL_ADCName {
    CC3220S_LAUNCHXL_ADC0 = 0,
    CC3220S_LAUNCHXL_ADC1,

    CC3220S_LAUNCHXL_ADCCOUNT
} CC3220S_LAUNCHXL_ADCName;

/*!
 *  @def    CC3220S_LAUNCHXL_CaptureName
 *  @brief  Enum of Capture names on the CC3220S_LAUNCHXL dev board
 */
typedef enum CC3220S_LAUNCHXL_CaptureName {
    CC3220S_LAUNCHXL_CAPTURE0 = 0,
    CC3220S_LAUNCHXL_CAPTURE1,

    CC3220S_LAUNCHXL_CAPTURECOUNT
} CC3220S_LAUNCHXL_CaptureName;

/*!
 *  @def    CC3220S_LAUNCHXL_CryptoName
 *  @brief  Enum of Crypto names on the CC3220S_LAUNCHXL dev board
 */
typedef enum CC3220S_LAUNCHXL_CryptoName {
    CC3220S_LAUNCHXL_CRYPTO0 = 0,

    CC3220S_LAUNCHXL_CRYPTOCOUNT
} CC3220S_LAUNCHXL_CryptoName;

/*!
 *  @def    CC3220S_LAUNCHXL_GPIOName
 *  @brief  Enum of GPIO names on the CC3220S_LAUNCHXL dev board
 */
typedef enum CC3220S_LAUNCHXL_GPIOName {
//    CC3220S_LAUNCHXL_GPIO_Bot1=0,
//    CC3220S_LAUNCHXL_GPIO_Bot2,
//    CC3220S_LAUNCHXL_GPIO_Bot3,
    CC3220S_LAUNCHXL_GPIO_SW2=0,
    CC3220S_LAUNCHXL_GPIO_CLK,
    CC3220S_LAUNCHXL_GPIO_DT,
    CC3220S_LAUNCHXL_GPIO_PB1,
    CC3220S_LAUNCHXL_GPIO_COL1,
    CC3220S_LAUNCHXL_GPIO_COL2,
    CC3220S_LAUNCHXL_GPIO_COL3,
    CC3220S_LAUNCHXL_SPI_MASTER_READY,
    CC3220S_LAUNCHXL_SPI_SLAVE_READY,
    CC3220S_LAUNCHXL_GPIO_PB2,
    CC3220S_LAUNCHXL_GPIO_D7,
    CC3220S_LAUNCHXL_GPIO_D6,
    CC3220S_LAUNCHXL_GPIO_D5,
    CC3220S_LAUNCHXL_GPIO_D4,
    CC3220S_LAUNCHXL_GPIO_EN,
    CC3220S_LAUNCHXL_GPIO_RS,
    CC3220S_LAUNCHXL_GPIO_ROW1,
    CC3220S_LAUNCHXL_GPIO_ROW2,
    CC3220S_LAUNCHXL_GPIO_ROW3,
    CC3220S_LAUNCHXL_GPIO_ROW4,

    CC3220S_LAUNCHXL_GPIO_Mot1,
    CC3220S_LAUNCHXL_GPIO_Mot2,
    CC3220S_LAUNCHXL_GPIO_ORANGE,
    CC3220S_LAUNCHXL_GPIO_YELLOW,
    CC3220S_LAUNCHXL_GPIO_PINK,
    CC3220S_LAUNCHXL_GPIO_BLUE,
//    CC3220S_LAUNCHXL_GPIO_A,//A
//    CC3220S_LAUNCHXL_GPIO_B,//B
//    CC3220S_LAUNCHXL_GPIO_C,//C
//    CC3220S_LAUNCHXL_GPIO_D,//D
//    CC3220S_LAUNCHXL_GPIO_E,//E
//    CC3220S_LAUNCHXL_GPIO_F,//F
//    CC3220S_LAUNCHXL_GPIO_G,//G
//    CC3220S_LAUNCHXL_GPIO_7S1,//7S1
//    CC3220S_LAUNCHXL_GPIO_7S2,//&S2

    /*
     *  CC3220S_LAUNCHXL_GPIO_LED_D8 and CC3220S_LAUNCHXL_GPIO_LED_D9 are shared with the
     *  I2C and PWM peripherals. In order for those examples to work, these
     *  LEDs are  taken out of gpioPinCOnfig[]
     */
    /* CC3220S_LAUNCHXL_GPIO_LED_D9, */
    /* CC3220S_LAUNCHXL_GPIO_LED_D8, */

//    CC3220S_LAUNCHXL_GPIO_TMP116_EN,

    CC3220S_LAUNCHXL_GPIOCOUNT
} CC3220S_LAUNCHXL_GPIOName;

/*!
 *  @def    CC3220S_LAUNCHXL_I2CName
 *  @brief  Enum of I2C names on the CC3220S_LAUNCHXL dev board
 */
typedef enum CC3220S_LAUNCHXL_I2CName {
    CC3220S_LAUNCHXL_I2C0 = 0,

    CC3220S_LAUNCHXL_I2CCOUNT
} CC3220S_LAUNCHXL_I2CName;

/*!
 *  @def    CC3220S_LAUNCHXL_I2SName
 *  @brief  Enum of I2S names on the CC3220S_LAUNCHXL dev board
 */
typedef enum CC3220S_LAUNCHXL_I2SName {
    CC3220S_LAUNCHXL_I2S0 = 0,

    CC3220S_LAUNCHXL_I2SCOUNT
} CC3220S_LAUNCHXL_I2SName;

/*!
 *  @def    CC3220S_LAUNCHXL_PWMName
 *  @brief  Enum of PWM names on the CC3220S_LAUNCHXL dev board
 */
typedef enum CC3220S_LAUNCHXL_PWMName {
    CC3220S_LAUNCHXL_PWM6 = 0,
    CC3220S_LAUNCHXL_PWM7,
    CC3220S_LAUNCHXL_PWM8,

    CC3220S_LAUNCHXL_PWMCOUNT
} CC3220S_LAUNCHXL_PWMName;

/*!
 *  @def    CC3220S_LAUNCHXL_SDFatFSName
 *  @brief  Enum of SDFatFS names on the CC3220S_LAUNCHXL dev board
 */
typedef enum CC3220S_LAUNCHXL_SDFatFSName {
    CC3220S_LAUNCHXL_SDFatFS0 = 0,

    CC3220S_LAUNCHXL_SDFatFSCOUNT
} CC3220S_LAUNCHXL_SDFatFSName;

/*!
 *  @def    CC3220S_LAUNCHXL_SDName
 *  @brief  Enum of SD names on the CC3220S_LAUNCHXL dev board
 */
typedef enum CC3220S_LAUNCHXL_SDName {
    CC3220S_LAUNCHXL_SD0 = 0,

    CC3220S_LAUNCHXL_SDCOUNT
} CC3220S_LAUNCHXL_SDName;

/*!
 *  @def    CC3220S_LAUNCHXL_SPIName
 *  @brief  Enum of SPI names on the CC3220S_LAUNCHXL dev board
 */
typedef enum CC3220S_LAUNCHXL_SPIName {
    CC3220S_LAUNCHXL_SPI0 = 0,
    CC3220S_LAUNCHXL_SPI1,

    CC3220S_LAUNCHXL_SPICOUNT
} CC3220S_LAUNCHXL_SPIName;

/*!
 *  @def    CC3220S_LAUNCHXL_TimerName
 *  @brief  Enum of Timer names on the CC3220S_LAUNCHXL dev board
 */
typedef enum CC3220S_LAUNCHXL_TimerName {
    CC3220S_LAUNCHXL_TIMER0 = 0,
    CC3220S_LAUNCHXL_TIMER1,
    CC3220S_LAUNCHXL_TIMER2,

    CC3220S_LAUNCHXL_TIMERCOUNT
} CC3220S_LAUNCHXL_TimerName;

/*!
 *  @def    CC3220S_LAUNCHXL_UARTName
 *  @brief  Enum of UARTs on the CC3220S_LAUNCHXL dev board
 */
typedef enum CC3220S_LAUNCHXL_UARTName {
    CC3220S_LAUNCHXL_UART0 = 0,
    CC3220S_LAUNCHXL_UART1,

    CC3220S_LAUNCHXL_UARTCOUNT
} CC3220S_LAUNCHXL_UARTName;

/*!
 *  @def    CC3220S_LAUNCHXL_WatchdogName
 *  @brief  Enum of Watchdogs on the CC3220S_LAUNCHXL dev board
 */
typedef enum CC3220S_LAUNCHXL_WatchdogName {
    CC3220S_LAUNCHXL_WATCHDOG0 = 0,

    CC3220S_LAUNCHXL_WATCHDOGCOUNT
} CC3220S_LAUNCHXL_WatchdogName;

/*!
 *  @brief  Initialize the general board specific settings
 *
 *  This function initializes the general board specific settings.
 */
extern void CC3220S_LAUNCHXL_initGeneral(void);

#ifdef __cplusplus
}
#endif

#endif /* __CC3220S_LAUNCHXL_H */
