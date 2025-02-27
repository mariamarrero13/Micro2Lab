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

/*
 *  ======== CC3220S_LAUNCHXL.c ========
 *  This file is responsible for setting up the board specific items for the
 *  CC3220S_LAUNCHXL board.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <ti/devices/cc32xx/inc/hw_ints.h>
#include <ti/devices/cc32xx/inc/hw_memmap.h>
#include <ti/devices/cc32xx/inc/hw_types.h>

#include <ti/devices/cc32xx/driverlib/rom.h>
#include <ti/devices/cc32xx/driverlib/rom_map.h>
#include <ti/devices/cc32xx/driverlib/adc.h>
#include <ti/devices/cc32xx/driverlib/gpio.h>
#include <ti/devices/cc32xx/driverlib/pin.h>
#include <ti/devices/cc32xx/driverlib/prcm.h>
#include <ti/devices/cc32xx/driverlib/spi.h>
#include <ti/devices/cc32xx/driverlib/sdhost.h>
#include <ti/devices/cc32xx/driverlib/timer.h>
#include <ti/devices/cc32xx/driverlib/uart.h>
#include <ti/devices/cc32xx/driverlib/udma.h>
#include <ti/devices/cc32xx/driverlib/wdt.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC32XX.h>

#include "CC3220S_LAUNCHXL.h"

/*
 *  This define determines whether to use the UARTCC32XXDMA driver
 *  or the UARTCC32XX (no DMA) driver.  Set to 1 to use the UARTCC32XXDMA
 *  driver.
 */
#ifndef TI_DRIVERS_UART_DMA
#define TI_DRIVERS_UART_DMA 0
#endif

/*
 *  =============================== ADC ===============================
 */
#include <ti/drivers/ADC.h>
#include <ti/drivers/adc/ADCCC32XX.h>

ADCCC32XX_Object adcCC3220SObjects[CC3220S_LAUNCHXL_ADCCOUNT];

const ADCCC32XX_HWAttrsV1 adcCC3220SHWAttrs[CC3220S_LAUNCHXL_ADCCOUNT] = {
    {
        .adcPin = ADCCC32XX_PIN_59_CH_2
    },
    {
        .adcPin = ADCCC32XX_PIN_60_CH_3
    }
};

const ADC_Config ADC_config[CC3220S_LAUNCHXL_ADCCOUNT] = {
    {
        .fxnTablePtr = &ADCCC32XX_fxnTable,
        .object = &adcCC3220SObjects[CC3220S_LAUNCHXL_ADC0],
        .hwAttrs = &adcCC3220SHWAttrs[CC3220S_LAUNCHXL_ADC0]
    },
    {
        .fxnTablePtr = &ADCCC32XX_fxnTable,
        .object = &adcCC3220SObjects[CC3220S_LAUNCHXL_ADC1],
        .hwAttrs = &adcCC3220SHWAttrs[CC3220S_LAUNCHXL_ADC1]
    }
};

const uint_least8_t ADC_count = CC3220S_LAUNCHXL_ADCCOUNT;

/*
 *  =============================== Capture ===============================
 */
#include <ti/drivers/Capture.h>
#include <ti/drivers/capture/CaptureCC32XX.h>

CaptureCC32XX_Object captureCC3220SObjects[CC3220S_LAUNCHXL_CAPTURECOUNT];

const CaptureCC32XX_HWAttrs captureCC3220SHWAttrs[CC3220S_LAUNCHXL_CAPTURECOUNT] =
{
      {
         .capturePin = CaptureCC32XX_PIN_61,
         .intPriority = ~0
      },
      {
          .capturePin = CaptureCC32XX_PIN_05,
          .intPriority = ~0
      },
};

const Capture_Config Capture_config[CC3220S_LAUNCHXL_CAPTURECOUNT] = {
    {
        .fxnTablePtr = &CaptureCC32XX_fxnTable,
        .object = &captureCC3220SObjects[CC3220S_LAUNCHXL_CAPTURE0],
        .hwAttrs = &captureCC3220SHWAttrs[CC3220S_LAUNCHXL_CAPTURE0]
    },
    {
        .fxnTablePtr = &CaptureCC32XX_fxnTable,
        .object = &captureCC3220SObjects[CC3220S_LAUNCHXL_CAPTURE1],
        .hwAttrs = &captureCC3220SHWAttrs[CC3220S_LAUNCHXL_CAPTURE1]
    }
};

const uint_least8_t Capture_count = CC3220S_LAUNCHXL_CAPTURECOUNT;

/*
 *  =============================== Crypto ===============================
 */
#include <ti/drivers/crypto/CryptoCC32XX.h>

CryptoCC32XX_Object cryptoCC3220SObjects[CC3220S_LAUNCHXL_CRYPTOCOUNT];

const CryptoCC32XX_Config CryptoCC32XX_config[CC3220S_LAUNCHXL_CRYPTOCOUNT] = {
    {
        .object = &cryptoCC3220SObjects[CC3220S_LAUNCHXL_CRYPTO0]
    }
};

const uint_least8_t CryptoCC32XX_count = CC3220S_LAUNCHXL_CRYPTOCOUNT;

/*
 *  =============================== DMA ===============================
 */
#include <ti/drivers/dma/UDMACC32XX.h>

static tDMAControlTable dmaControlTable[64] __attribute__ ((aligned (1024)));

/*
 *  ======== dmaErrorFxn ========
 *  This is the handler for the uDMA error interrupt.
 */
static void dmaErrorFxn(uintptr_t arg)
{
    int status = MAP_uDMAErrorStatusGet();
    MAP_uDMAErrorStatusClear();

    /* Suppress unused variable warning */
    (void)status;

    while (1);
}

UDMACC32XX_Object udmaCC3220SObject;

const UDMACC32XX_HWAttrs udmaCC3220SHWAttrs = {
    .controlBaseAddr = (void *)dmaControlTable,
    .dmaErrorFxn = (UDMACC32XX_ErrorFxn)dmaErrorFxn,
    .intNum = INT_UDMAERR,
    .intPriority = (~0)
};

const UDMACC32XX_Config UDMACC32XX_config = {
    .object = &udmaCC3220SObject,
    .hwAttrs = &udmaCC3220SHWAttrs
};

/*
 *  =============================== General ===============================
 */
/*
 *  ======== CC3220S_LAUNCHXL_initGeneral ========
 */
void CC3220S_LAUNCHXL_initGeneral(void)
{
    PRCMCC3200MCUInit();
    Power_init();
}

/*
 *  ======== Board_init ========
 */
void Board_init(void)
{
    CC3220S_LAUNCHXL_initGeneral();
}

/*
 *  =============================== GPIO ===============================
 */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOCC32XX.h>

/*
 * Array of Pin configurations
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in CC3220S_LAUNCHXL.h
 * NOTE: Pins not used for interrupts should be placed at the end of the
 *       array.  Callback entries can be omitted from callbacks array to
 *       reduce memory usage.
 */
GPIO_PinConfig gpioPinConfigs[] = {
    /* input pins with callbacks */

   /*Pushbuttons*/
//   GPIOCC32XX_GPIO_30 | GPIO_CFG_INPUT | GPIO_CFG_IN_INT_RISING,
//   GPIOCC32XX_GPIO_09 | GPIO_CFG_INPUT | GPIO_CFG_IN_INT_RISING,
//   GPIOCC32XX_GPIO_00 | GPIO_CFG_INPUT | GPIO_CFG_IN_INT_RISING,
   /* CC3220S_LAUNCHXL_GPIO_SW2 */
    GPIOCC32XX_GPIO_13 | GPIO_CFG_INPUT | GPIO_CFG_IN_INT_RISING,

    /*Rotary Encoder*/
    GPIOCC32XX_GPIO_08 | GPIO_CFG_INPUT | GPIO_CFG_IN_INT_RISING,
    GPIOCC32XX_GPIO_30 | GPIO_CFG_INPUT | GPIO_CFG_IN_INT_RISING,
    /* Push Button */
    GPIOCC32XX_GPIO_11 | GPIO_CFG_INPUT | GPIO_CFG_IN_INT_RISING,
    /* KEYPAD COLUMNS*/
    GPIOCC32XX_GPIO_13 | GPIO_CFG_INPUT | GPIO_CFG_IN_INT_RISING,
    GPIOCC32XX_GPIO_06 | GPIO_CFG_INPUT | GPIO_CFG_IN_INT_RISING,
    GPIOCC32XX_GPIO_07 | GPIO_CFG_INPUT | GPIO_CFG_IN_INT_RISING,

    /* CC3220S_LAUNCHXL_SPI_MASTER_READY */
    GPIOCC32XX_GPIO_28 | GPIO_DO_NOT_CONFIG,
    /* CC3220S_LAUNCHXL_SPI_SLAVE_READY */
    GPIOCC32XX_GPIO_12 | GPIO_DO_NOT_CONFIG,

    /* output pins */
    GPIOCC32XX_GPIO_06 | GPIO_CFG_INPUT | GPIO_CFG_IN_INT_RISING,

    /* LCD Data pin out */
    GPIOCC32XX_GPIO_28 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    GPIOCC32XX_GPIO_17 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    GPIOCC32XX_GPIO_16 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    GPIOCC32XX_GPIO_15 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    GPIOCC32XX_GPIO_25 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    GPIOCC32XX_GPIO_22 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,

    /*KEYPAD ROWS*/
    GPIOCC32XX_GPIO_08 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    GPIOCC32XX_GPIO_30 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    GPIOCC32XX_GPIO_09 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    GPIOCC32XX_GPIO_00 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,




    /*   Motor Out */
    GPIOCC32XX_GPIO_10 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    GPIOCC32XX_GPIO_11 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,

    GPIOCC32XX_GPIO_10 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    GPIOCC32XX_GPIO_11 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    GPIOCC32XX_GPIO_06 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,
    GPIOCC32XX_GPIO_07 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,


//    /*seven segment*/
//    GPIOCC32XX_GPIO_28 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW, //A
//    GPIOCC32XX_GPIO_17 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW, //B
//    GPIOCC32XX_GPIO_16 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW, //C
//    GPIOCC32XX_GPIO_15 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW, //D
//    GPIOCC32XX_GPIO_22 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW, //E
//    GPIOCC32XX_GPIO_00 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW, //F
//    GPIOCC32XX_GPIO_08 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW, //G
//
//    /*transistors*/
//    GPIOCC32XX_GPIO_09 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW, //7S1 - P64
//    GPIOCC32XX_GPIO_30 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW, //7S2 - P50


    /*
     *  CC3220S_LAUNCHXL_GPIO_LED_D8 and CC3220S_LAUNCHXL_GPIO_LED_D9 are shared with the
     *  I2C and PWM peripherals. In order for those examples to work, these
     *  LEDs are taken out of gpioPinConfig[]
     */
    /* CC3220S_LAUNCHXL_GPIO_LED_D9 */
    /* GPIOCC32XX_GPIO_10 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW, */
    /* CC3220S_LAUNCHXL_GPIO_LED_D8 */
    /* GPIOCC32XX_GPIO_11 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW, */

//    GPIOCC32XX_GPIO_30 | GPIO_DO_NOT_CONFIG, /* TMP116 EN */


};

/*
 * Array of callback function pointers
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in CC3220S_LAUNCHXL.h
 * NOTE: Pins not used for interrupts can be omitted from callbacks array to
 *       reduce memory usage (if placed at end of gpioPinConfigs array).
 */
GPIO_CallbackFxn gpioCallbackFunctions[] = {
    NULL,  /* CC3220S_LAUNCHXL_GPIO_SW2 */
    NULL,  /* CC3220S_LAUNCHXL_GPIO_SW3 */
    NULL,  /* CC3220S_LAUNCHXL_SPI_MASTER_READY */
    NULL   /* CC3220S_LAUNCHXL_SPI_SLAVE_READY */
};

/* The device-specific GPIO_config structure */
const GPIOCC32XX_Config GPIOCC32XX_config = {
    .pinConfigs = (GPIO_PinConfig *)gpioPinConfigs,
    .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
    .numberOfPinConfigs = sizeof(gpioPinConfigs)/sizeof(GPIO_PinConfig),
    .numberOfCallbacks = sizeof(gpioCallbackFunctions)/sizeof(GPIO_CallbackFxn),
    .intPriority = (~0)
};

/*
 *  =============================== I2C ===============================
 */
#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CCC32XX.h>

I2CCC32XX_Object i2cCC3220SObjects[CC3220S_LAUNCHXL_I2CCOUNT];

const I2CCC32XX_HWAttrsV1 i2cCC3220SHWAttrs[CC3220S_LAUNCHXL_I2CCOUNT] = {
    {
        .baseAddr = I2CA0_BASE,
        .intNum = INT_I2CA0,
        .intPriority = (~0),
        .clkPin = I2CCC32XX_PIN_01_I2C_SCL,
        .dataPin = I2CCC32XX_PIN_02_I2C_SDA
    }
};

const I2C_Config I2C_config[CC3220S_LAUNCHXL_I2CCOUNT] = {
    {
        .fxnTablePtr = &I2CCC32XX_fxnTable,
        .object = &i2cCC3220SObjects[CC3220S_LAUNCHXL_I2C0],
        .hwAttrs = &i2cCC3220SHWAttrs[CC3220S_LAUNCHXL_I2C0]
    }
};

const uint_least8_t I2C_count = CC3220S_LAUNCHXL_I2CCOUNT;

/*
 *  =============================== I2S ===============================
 */
#include <ti/drivers/I2S.h>
#include <ti/drivers/i2s/I2SCC32XX.h>

I2SCC32XX_Object i2sCC3220SObjects[CC3220S_LAUNCHXL_I2SCOUNT];

const I2SCC32XX_HWAttrs i2sCC3220SHWAttrs[CC3220S_LAUNCHXL_I2SCOUNT] = {
    {
      .pinSD1           =  I2SCC32XX_PIN_50_SD1,
      .pinSD0           =  I2SCC32XX_PIN_64_SD0,
      .pinSCK           =  I2SCC32XX_PIN_53_SCK,
      .pinSCKX          =  I2SCC32XX_PIN_UNUSED,
      .pinWS            =  I2SCC32XX_PIN_63_WS,
      .rxChannelIndex   =  UDMA_CH4_I2S_RX,
      .txChannelIndex   =  UDMA_CH5_I2S_TX,
      .intPriority      =  0x40,
    }
};

const I2S_Config I2S_config[CC3220S_LAUNCHXL_I2SCOUNT] = {
    {
        .object = &i2sCC3220SObjects[CC3220S_LAUNCHXL_I2S0],
        .hwAttrs = &i2sCC3220SHWAttrs[CC3220S_LAUNCHXL_I2S0]
    }
};

const uint_least8_t I2S_count = CC3220S_LAUNCHXL_I2SCOUNT;

/*
 *  =============================== Power ===============================
 */
/*
 * This table defines the parking state to be set for each parkable pin
 * during LPDS. (Device pins must be parked during LPDS to achieve maximum
 * power savings.)  If the pin should be left unparked, specify the state
 * PowerCC32XX_DONT_PARK.  For example, for a UART TX pin, the device
 * will automatically park the pin in a high state during transition to LPDS,
 * so the Power Manager does not need to explictly park the pin.  So the
 * corresponding entries in this table should indicate PowerCC32XX_DONT_PARK.
 */
PowerCC32XX_ParkInfo parkInfo[] = {
/*          PIN                    PARK STATE              PIN ALIAS (FUNCTION)
     -----------------  ------------------------------     -------------------- */
    {PowerCC32XX_PIN01, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* GPIO10              */
    {PowerCC32XX_PIN02, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* GPIO11              */
    {PowerCC32XX_PIN03, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* GPIO12              */
    {PowerCC32XX_PIN04, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* GPIO13              */
    {PowerCC32XX_PIN05, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* GPIO14              */
    {PowerCC32XX_PIN06, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* GPIO15              */
    {PowerCC32XX_PIN07, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* GPIO16              */
    {PowerCC32XX_PIN08, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* GPIO17              */
    {PowerCC32XX_PIN13, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* FLASH_SPI_DIN       */
    {PowerCC32XX_PIN15, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* GPIO22              */
    {PowerCC32XX_PIN16, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* TDI (JTAG DEBUG)    */
    {PowerCC32XX_PIN17, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* TDO (JTAG DEBUG)    */
    {PowerCC32XX_PIN19, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* TCK (JTAG DEBUG)    */
    {PowerCC32XX_PIN20, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* TMS (JTAG DEBUG)    */
    {PowerCC32XX_PIN18, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* GPIO28              */
    {PowerCC32XX_PIN21, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* SOP2                */
    {PowerCC32XX_PIN29, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* ANTSEL1             */
    {PowerCC32XX_PIN30, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* ANTSEL2             */
    {PowerCC32XX_PIN45, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* DCDC_ANA2_SW_P      */
    {PowerCC32XX_PIN50, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* GPIO0               */
    {PowerCC32XX_PIN52, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* RTC_XTAL_N          */
    {PowerCC32XX_PIN53, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* GPIO30              */
    {PowerCC32XX_PIN55, PowerCC32XX_WEAK_PULL_UP_STD},   /* GPIO1 (XDS_UART_RX) */
    {PowerCC32XX_PIN57, PowerCC32XX_WEAK_PULL_UP_STD},   /* GPIO2 (XDS_UART_TX) */
    {PowerCC32XX_PIN58, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* GPIO3               */
    {PowerCC32XX_PIN59, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* GPIO4               */
    {PowerCC32XX_PIN60, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* GPIO5               */
    {PowerCC32XX_PIN61, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* GPIO6               */
    {PowerCC32XX_PIN62, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* GPIO7               */
    {PowerCC32XX_PIN63, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* GPIO8               */
    {PowerCC32XX_PIN64, PowerCC32XX_WEAK_PULL_DOWN_STD}, /* GPIO9               */
};

/*
 *  This structure defines the configuration for the Power Manager.
 *
 *  In this configuration the Power policy is disabled by default (because
 *  enablePolicy is set to false).  The Power policy can be enabled dynamically
 *  at runtime by calling Power_enablePolicy(), or at build time, by changing
 *  enablePolicy to true in this structure.
 */
const PowerCC32XX_ConfigV1 PowerCC32XX_config = {
    .policyInitFxn = &PowerCC32XX_initPolicy,
    .policyFxn = &PowerCC32XX_sleepPolicy,
    .enterLPDSHookFxn = NULL,
    .resumeLPDSHookFxn = NULL,
    .enablePolicy = false,
    .enableGPIOWakeupLPDS = true,
    .enableGPIOWakeupShutdown = true,
    .enableNetworkWakeupLPDS = true,
    .wakeupGPIOSourceLPDS = PRCM_LPDS_GPIO13,
    .wakeupGPIOTypeLPDS = PRCM_LPDS_FALL_EDGE,
    .wakeupGPIOFxnLPDS = NULL,
    .wakeupGPIOFxnLPDSArg = 0,
    .wakeupGPIOSourceShutdown = PRCM_HIB_GPIO13,
    .wakeupGPIOTypeShutdown = PRCM_HIB_RISE_EDGE,
    .ramRetentionMaskLPDS = PRCM_SRAM_COL_1 | PRCM_SRAM_COL_2 |
        PRCM_SRAM_COL_3 | PRCM_SRAM_COL_4,
    .keepDebugActiveDuringLPDS = false,
    .ioRetentionShutdown = PRCM_IO_RET_GRP_1,
    .pinParkDefs = parkInfo,
    .numPins = sizeof(parkInfo) / sizeof(PowerCC32XX_ParkInfo)
};

/*
 *  =============================== PWM ===============================
 */
#include <ti/drivers/PWM.h>
#include <ti/drivers/pwm/PWMTimerCC32XX.h>

PWMTimerCC32XX_Object pwmTimerCC3220SObjects[CC3220S_LAUNCHXL_PWMCOUNT];

const PWMTimerCC32XX_HWAttrsV2 pwmTimerCC3220SHWAttrs[CC3220S_LAUNCHXL_PWMCOUNT] = {
    {    /* CC3220S_LAUNCHXL_PWM6 */
        .pwmPin = PWMTimerCC32XX_PIN_01
    },
    {    /* CC3220S_LAUNCHXL_PWM7 */
        .pwmPin = PWMTimerCC32XX_PIN_02
    },
    {
         .pwmPin = PWMTimerCC32XX_PIN_64
    }
};

const PWM_Config PWM_config[CC3220S_LAUNCHXL_PWMCOUNT] = {
    {
        .fxnTablePtr = &PWMTimerCC32XX_fxnTable,
        .object = &pwmTimerCC3220SObjects[CC3220S_LAUNCHXL_PWM6],
        .hwAttrs = &pwmTimerCC3220SHWAttrs[CC3220S_LAUNCHXL_PWM6]
    },
    {
        .fxnTablePtr = &PWMTimerCC32XX_fxnTable,
        .object = &pwmTimerCC3220SObjects[CC3220S_LAUNCHXL_PWM7],
        .hwAttrs = &pwmTimerCC3220SHWAttrs[CC3220S_LAUNCHXL_PWM7]
    },
    {
        .fxnTablePtr = &PWMTimerCC32XX_fxnTable,
        .object = &pwmTimerCC3220SObjects[CC3220S_LAUNCHXL_PWM8],
        .hwAttrs = &pwmTimerCC3220SHWAttrs[CC3220S_LAUNCHXL_PWM8]
    }
};

const uint_least8_t PWM_count = CC3220S_LAUNCHXL_PWMCOUNT;

/*
 *  =============================== SDFatFS ===============================
 */
#include <ti/drivers/SD.h>
#include <ti/drivers/SDFatFS.h>

/*
 * Note: The SDFatFS driver provides interface functions to enable FatFs
 * but relies on the SD driver to communicate with SD cards.  Opening a
 * SDFatFs driver instance will internally try to open a SD driver instance
 * reusing the same index number (opening SDFatFs driver at index 0 will try to
 * open SD driver at index 0).  This requires that all SDFatFs driver instances
 * have an accompanying SD driver instance defined with the same index.  It is
 * acceptable to have more SD driver instances than SDFatFs driver instances
 * but the opposite is not supported & the SDFatFs will fail to open.
 */
SDFatFS_Object sdfatfsObjects[CC3220S_LAUNCHXL_SDFatFSCOUNT];

const SDFatFS_Config SDFatFS_config[CC3220S_LAUNCHXL_SDFatFSCOUNT] = {
    {
        .object = &sdfatfsObjects[CC3220S_LAUNCHXL_SDFatFS0]
    }
};

const uint_least8_t SDFatFS_count = CC3220S_LAUNCHXL_SDFatFSCOUNT;

/*
 *  =============================== SD ===============================
 */
#include <ti/drivers/SD.h>
#include <ti/drivers/sd/SDHostCC32XX.h>

SDHostCC32XX_Object sdhostCC3220SObjects[CC3220S_LAUNCHXL_SDCOUNT];

/* SDHost configuration structure, describing which pins are to be used */
const SDHostCC32XX_HWAttrsV1 sdhostCC3220SHWattrs[CC3220S_LAUNCHXL_SDCOUNT] = {
    {
        .clkRate = 8000000,
        .intPriority = ~0,
        .baseAddr = SDHOST_BASE,
        .rxChIdx = UDMA_CH23_SDHOST_RX,
        .txChIdx = UDMA_CH24_SDHOST_TX,
        .dataPin = SDHostCC32XX_PIN_06_SDCARD_DATA,
        .cmdPin = SDHostCC32XX_PIN_08_SDCARD_CMD,
        .clkPin = SDHostCC32XX_PIN_07_SDCARD_CLK
    }
};

const SD_Config SD_config[CC3220S_LAUNCHXL_SDCOUNT] = {
    {
        .fxnTablePtr = &sdHostCC32XX_fxnTable,
        .object = &sdhostCC3220SObjects[CC3220S_LAUNCHXL_SD0],
        .hwAttrs = &sdhostCC3220SHWattrs[CC3220S_LAUNCHXL_SD0]
    },
};

const uint_least8_t SD_count = CC3220S_LAUNCHXL_SDCOUNT;

/*
 *  =============================== SPI ===============================
 */
#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPICC32XXDMA.h>

SPICC32XXDMA_Object spiCC3220SDMAObjects[CC3220S_LAUNCHXL_SPICOUNT];

uint32_t spiCC3220SDMAscratchBuf[CC3220S_LAUNCHXL_SPICOUNT];

const SPICC32XXDMA_HWAttrsV1 spiCC3220SDMAHWAttrs[CC3220S_LAUNCHXL_SPICOUNT] = {
    /* index 0 is reserved for LSPI that links to the NWP */
    {
        .baseAddr = LSPI_BASE,
        .intNum = INT_LSPI,
        .intPriority = (~0),
        .spiPRCM = PRCM_LSPI,
        .csControl = SPI_SW_CTRL_CS,
        .csPolarity = SPI_CS_ACTIVEHIGH,
        .pinMode = SPI_4PIN_MODE,
        .turboMode = SPI_TURBO_OFF,
        .scratchBufPtr = &spiCC3220SDMAscratchBuf[CC3220S_LAUNCHXL_SPI0],
        .defaultTxBufValue = 0,
        .rxChannelIndex = UDMA_CH12_LSPI_RX,
        .txChannelIndex = UDMA_CH13_LSPI_TX,
        .minDmaTransferSize = 100,
        .mosiPin = SPICC32XXDMA_PIN_NO_CONFIG,
        .misoPin = SPICC32XXDMA_PIN_NO_CONFIG,
        .clkPin = SPICC32XXDMA_PIN_NO_CONFIG,
        .csPin = SPICC32XXDMA_PIN_NO_CONFIG
    },
    {
        .baseAddr = GSPI_BASE,
        .intNum = INT_GSPI,
        .intPriority = (~0),
        .spiPRCM = PRCM_GSPI,
        .csControl = SPI_HW_CTRL_CS,
        .csPolarity = SPI_CS_ACTIVELOW,
        .pinMode = SPI_4PIN_MODE,
        .turboMode = SPI_TURBO_OFF,
        .scratchBufPtr = &spiCC3220SDMAscratchBuf[CC3220S_LAUNCHXL_SPI1],
        .defaultTxBufValue = 0,
        .rxChannelIndex = UDMA_CH6_GSPI_RX,
        .txChannelIndex = UDMA_CH7_GSPI_TX,
        .minDmaTransferSize = 10,
        .mosiPin = SPICC32XXDMA_PIN_07_MOSI,
        .misoPin = SPICC32XXDMA_PIN_06_MISO,
        .clkPin = SPICC32XXDMA_PIN_05_CLK,
        .csPin = SPICC32XXDMA_PIN_08_CS
    }
};

const SPI_Config SPI_config[CC3220S_LAUNCHXL_SPICOUNT] = {
    {
        .fxnTablePtr = &SPICC32XXDMA_fxnTable,
        .object = &spiCC3220SDMAObjects[CC3220S_LAUNCHXL_SPI0],
        .hwAttrs = &spiCC3220SDMAHWAttrs[CC3220S_LAUNCHXL_SPI0]
    },
    {
        .fxnTablePtr = &SPICC32XXDMA_fxnTable,
        .object = &spiCC3220SDMAObjects[CC3220S_LAUNCHXL_SPI1],
        .hwAttrs = &spiCC3220SDMAHWAttrs[CC3220S_LAUNCHXL_SPI1]
    }
};

const uint_least8_t SPI_count = CC3220S_LAUNCHXL_SPICOUNT;

/*
 *  =============================== Timer ===============================
 */
#include <ti/drivers/Timer.h>
#include <ti/drivers/timer/TimerCC32XX.h>

TimerCC32XX_Object timerCC3220SObjects[CC3220S_LAUNCHXL_TIMERCOUNT];

const TimerCC32XX_HWAttrs timerCC3220SHWAttrs[CC3220S_LAUNCHXL_TIMERCOUNT] = {
    {
        .baseAddress = TIMERA0_BASE,
        .subTimer = TimerCC32XX_timer32,
        .intNum = INT_TIMERA0A,
        .intPriority = ~0
    },
    {
        .baseAddress = TIMERA1_BASE,
        .subTimer = TimerCC32XX_timer16A,
        .intNum = INT_TIMERA1A,
        .intPriority = ~0
    },
    {
         .baseAddress = TIMERA1_BASE,
         .subTimer = TimerCC32XX_timer16B,
         .intNum = INT_TIMERA1B,
         .intPriority = ~0
    },
};

const Timer_Config Timer_config[CC3220S_LAUNCHXL_TIMERCOUNT] = {
    {
        .fxnTablePtr = &TimerCC32XX_fxnTable,
        .object = &timerCC3220SObjects[CC3220S_LAUNCHXL_TIMER0],
        .hwAttrs = &timerCC3220SHWAttrs[CC3220S_LAUNCHXL_TIMER0]
    },
    {
        .fxnTablePtr = &TimerCC32XX_fxnTable,
        .object = &timerCC3220SObjects[CC3220S_LAUNCHXL_TIMER1],
        .hwAttrs = &timerCC3220SHWAttrs[CC3220S_LAUNCHXL_TIMER1]
    },
    {
        .fxnTablePtr = &TimerCC32XX_fxnTable,
        .object = &timerCC3220SObjects[CC3220S_LAUNCHXL_TIMER2],
        .hwAttrs = &timerCC3220SHWAttrs[CC3220S_LAUNCHXL_TIMER2]
    },
};

const uint_least8_t Timer_count = CC3220S_LAUNCHXL_TIMERCOUNT;

/*
 *  =============================== UART ===============================
 */
#include <ti/drivers/UART.h>
#if TI_DRIVERS_UART_DMA
#include <ti/drivers/uart/UARTCC32XXDMA.h>

UARTCC32XXDMA_Object uartCC3220SDmaObjects[CC3220S_LAUNCHXL_UARTCOUNT];

/* UART configuration structure */
const UARTCC32XXDMA_HWAttrsV1 uartCC3220SDmaHWAttrs[CC3220S_LAUNCHXL_UARTCOUNT] = {
    {
        .baseAddr = UARTA0_BASE,
        .intNum = INT_UARTA0,
        .intPriority = (~0),
        .flowControl = UARTCC32XXDMA_FLOWCTRL_NONE,
        .rxChannelIndex = UDMA_CH8_UARTA0_RX,
        .txChannelIndex = UDMA_CH9_UARTA0_TX,
        .rxPin = UARTCC32XXDMA_PIN_57_UART0_RX,
        .txPin = UARTCC32XXDMA_PIN_55_UART0_TX,
        .ctsPin = UARTCC32XXDMA_PIN_UNASSIGNED,
        .rtsPin = UARTCC32XXDMA_PIN_UNASSIGNED,
        .errorFxn = NULL
    },
    {
        .baseAddr = UARTA1_BASE,
        .intNum = INT_UARTA1,
        .intPriority = (~0),
        .flowControl = UARTCC32XXDMA_FLOWCTRL_NONE,
        .rxChannelIndex = UDMA_CH10_UARTA1_RX,
        .txChannelIndex = UDMA_CH11_UARTA1_TX,
        .rxPin = UARTCC32XXDMA_PIN_08_UART1_RX,
        .txPin = UARTCC32XXDMA_PIN_07_UART1_TX,
        .ctsPin = UARTCC32XXDMA_PIN_UNASSIGNED,
        .rtsPin = UARTCC32XXDMA_PIN_UNASSIGNED,
        .errorFxn = NULL
    }
};

const UART_Config UART_config[CC3220S_LAUNCHXL_UARTCOUNT] = {
    {
        .fxnTablePtr = &UARTCC32XXDMA_fxnTable,
        .object = &uartCC3220SDmaObjects[CC3220S_LAUNCHXL_UART0],
        .hwAttrs = &uartCC3220SDmaHWAttrs[CC3220S_LAUNCHXL_UART0]
    },
    {
        .fxnTablePtr = &UARTCC32XXDMA_fxnTable,
        .object = &uartCC3220SDmaObjects[CC3220S_LAUNCHXL_UART1],
        .hwAttrs = &uartCC3220SDmaHWAttrs[CC3220S_LAUNCHXL_UART1]
    }
};

#else
#include <ti/drivers/uart/UARTCC32XX.h>

UARTCC32XX_Object uartCC3220SObjects[CC3220S_LAUNCHXL_UARTCOUNT];
unsigned char uartCC3220SRingBuffer[CC3220S_LAUNCHXL_UARTCOUNT][32];

/* UART configuration structure */
const UARTCC32XX_HWAttrsV1 uartCC3220SHWAttrs[CC3220S_LAUNCHXL_UARTCOUNT] = {
    {
        .baseAddr = UARTA0_BASE,
        .intNum = INT_UARTA0,
        .intPriority = (~0),
        .flowControl = UARTCC32XX_FLOWCTRL_NONE,
        .ringBufPtr  = uartCC3220SRingBuffer[CC3220S_LAUNCHXL_UART0],
        .ringBufSize = sizeof(uartCC3220SRingBuffer[CC3220S_LAUNCHXL_UART0]),
        .rxPin = UARTCC32XX_PIN_04_UART0_RX,
        .txPin = UARTCC32XX_PIN_03_UART0_TX,
        .ctsPin = UARTCC32XX_PIN_UNASSIGNED,
        .rtsPin = UARTCC32XX_PIN_UNASSIGNED,
        .errorFxn = NULL
    },
    {
        .baseAddr = UARTA1_BASE,
        .intNum = INT_UARTA1,
        .intPriority = (~0),
        .flowControl = UARTCC32XX_FLOWCTRL_NONE,
        .ringBufPtr  = uartCC3220SRingBuffer[CC3220S_LAUNCHXL_UART1],
        .ringBufSize = sizeof(uartCC3220SRingBuffer[CC3220S_LAUNCHXL_UART1]),
        .rxPin = UARTCC32XX_PIN_57_UART1_RX,
        .txPin = UARTCC32XX_PIN_55_UART1_TX,
        .ctsPin = UARTCC32XX_PIN_UNASSIGNED,
        .rtsPin = UARTCC32XX_PIN_UNASSIGNED,
        .errorFxn = NULL
    }
};

const UART_Config UART_config[CC3220S_LAUNCHXL_UARTCOUNT] = {
    {
        .fxnTablePtr = &UARTCC32XX_fxnTable,
        .object = &uartCC3220SObjects[CC3220S_LAUNCHXL_UART0],
        .hwAttrs = &uartCC3220SHWAttrs[CC3220S_LAUNCHXL_UART0]
    },
    {
        .fxnTablePtr = &UARTCC32XX_fxnTable,
        .object = &uartCC3220SObjects[CC3220S_LAUNCHXL_UART1],
        .hwAttrs = &uartCC3220SHWAttrs[CC3220S_LAUNCHXL_UART1]
    }
};
#endif /* TI_DRIVERS_UART_DMA */

const uint_least8_t UART_count = CC3220S_LAUNCHXL_UARTCOUNT;

/*
 *  =============================== Watchdog ===============================
 */
#include <ti/drivers/Watchdog.h>
#include <ti/drivers/watchdog/WatchdogCC32XX.h>

WatchdogCC32XX_Object watchdogCC3220SObjects[CC3220S_LAUNCHXL_WATCHDOGCOUNT];

const WatchdogCC32XX_HWAttrs watchdogCC3220SHWAttrs[CC3220S_LAUNCHXL_WATCHDOGCOUNT] = {
    {
        .baseAddr = WDT_BASE,
        .intNum = INT_WDT,
        .intPriority = (~0),
        .reloadValue = 80000000 /* 1 second period at default CPU clock freq */
    }
};

const Watchdog_Config Watchdog_config[CC3220S_LAUNCHXL_WATCHDOGCOUNT] = {
    {
        .fxnTablePtr = &WatchdogCC32XX_fxnTable,
        .object = &watchdogCC3220SObjects[CC3220S_LAUNCHXL_WATCHDOG0],
        .hwAttrs = &watchdogCC3220SHWAttrs[CC3220S_LAUNCHXL_WATCHDOG0]
    }
};

const uint_least8_t Watchdog_count = CC3220S_LAUNCHXL_WATCHDOGCOUNT;
