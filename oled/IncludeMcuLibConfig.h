/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef INCLUDEMCULIBCONFIG_H_
#define INCLUDEMCULIBCONFIG_H_

/* ---------------------------------------------------------------------------------------*/
/* SDK */
#define McuLib_CONFIG_CPU_IS_KINETIS                (0)
#define McuLib_CONFIG_SDK_VERSION_USED              McuLib_CONFIG_SDK_LINUX
/* ---------------------------------------------------------------------------------------*/
/* FreeRTOS */
#define McuLib_CONFIG_SDK_USE_FREERTOS              (0)
/* ---------------------------------------------------------------------------------------*/
/* McuWait */
#define McuWait_CONFIG_USE_CYCLE_COUNTER  (0)
/* -------------------------------------------------*/
/* I2C */
#define MCUI2CLIB_CONFIG_I2C_DEVICE                 "/dev/i2c-1"
//#define MCUI2CLIB_CONFIG_SDA_GPIO_PIN               16u
//#define MCUI2CLIB_CONFIG_SCL_GPIO_PIN               17u
#define MCUI2CLIB_CONFIG_ADD_DELAY_US               (0)
#define MCUI2CLIB_CONFIG_TIMEOUT_BYTE_US            (1000)
#define MCUI2CLIB_CONFIG_I2C_RELEASE_BUS            (0)
/* -------------------------------------------------*/
/* McuGenericI2C */
#define McuGenericI2C_CONFIG_USE_ON_ERROR_EVENT       (0)
#define McuGenericI2C_CONFIG_USE_ON_RELEASE_BUS_EVENT (0)
#define McuGenericI2C_CONFIG_USE_ON_REQUEST_BUS_EVENT (0)
#define McuGenericI2C_CONFIG_USE_MUTEX                (1 && McuLib_CONFIG_SDK_USE_FREERTOS)

#define McuLib_CONFIG_MCUI2CLIB_ENABLED                       (1)
#define McuGenericI2C_CONFIG_INTERFACE_HEADER_FILE            "McuI2cLib.h"
#define McuGenericI2C_CONFIG_RECV_BLOCK                       McuI2cLib_RecvBlock
#define McuGenericI2C_CONFIG_SEND_BLOCK                       McuI2cLib_SendBlock
#if McuGenericI2C_CONFIG_SUPPORT_STOP_NO_START
#define McuGenericI2C_CONFIG_SEND_BLOCK_CONTINUE              McuI2cLib_SendBlockContinue
#endif
#define McuGenericI2C_CONFIG_SEND_STOP                        McuI2cLib_SendStop
#define McuGenericI2C_CONFIG_SELECT_SLAVE                     McuI2cLib_SelectSlave
#define McuGenericI2C_CONFIG_RECV_BLOCK_CUSTOM_AVAILABLE      (0)
#define McuGenericI2C_CONFIG_RECV_BLOCK_CUSTOM                McuI2cLib_RecvBlockCustom

#define MCUI2CLIB_CONFIG_ADD_DELAY                            (0)
/* ---------------------------------------------------------------------------------------*/
/* McuSSD1306 */
#define McuSSD1306_CONFIG_INIT_DELAY_MS           (0)
#define McuSSD1306_CONFIG_SSD1306_DRIVER_TYPE     (1106)
#define McuSSD1306_CONFIG_SSD1306_I2C_DELAY_US    (0)

#define McuLog_CONFIG_IS_ENABLED   (0)

#endif /* INCLUDEMCULIBCONFIG_H_ */
