/**
****************************************************************************
* @file			Platform.h
* @brief		Definition of the used Board
* @author		BFH-TI
* @version	V1.1
* @date			21.5.2012
* @hardware	STM32-P103, MCBSTM32C, STM3210C
*******************************************************************************/

/* Define Board --------------------------------------------------------------*/
#define USE_STM32_P103
//#define USE_MCBSTM32C
//#define USE_STM3210C

/* Include -------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdio.h>

/* Olimex STM32-P103 Board ---------------------------------------------------*/
#if defined(USE_STM32_P103)

        /* SPI SD */
        #define SD_SPI												SPI2
        #define SD_GPIO												GPIOB
        #define RCC_APBPeriphClockCmd_SD_SPI                            RCC_APB2PeriphClockCmd
        #define RCC_APBPeriph_SD_SPI					RCC_APB2Periph_SPI2
        #define RCC_APB2Periph_SD_IO					RCC_APB2Periph_GPIOB
        #define SPI_SD_PIN_SCK                                          GPIO_Pin_13
        #define SPI_SD_PIN_MISO                                         GPIO_Pin_14
        #define SPI_SD_PIN_MOSI						GPIO_Pin_15

        /* Remaps */

/* Keil MCBSTM32C Board ------------------------------------------------------*/
#elif defined(USE_MCBSTM32C)

    #define SD_SPI												SPI2
    #define SD_GPIO												GPIOB
    #define RCC_APBPeriphClockCmd_SD_SPI                            RCC_APB2PeriphClockCmd
    #define RCC_APBPeriph_SD_SPI					RCC_APB2Periph_SPI2
    #define RCC_APB2Periph_SD_IO					RCC_APB2Periph_GPIOB
    #define SPI_SD_PIN_SCK                                          GPIO_Pin_13
    #define SPI_SD_PIN_MISO                                         GPIO_Pin_14
    #define SPI_SD_PIN_MOSI						GPIO_Pin_15
        #define USE_I2C1_Remap
        #define Remap_I2C1				GPIO_Remap_I2C1

/* ST STM3210C Board ---------------------------------------------------------*/
#elif defined(USE_STM3210C)

    #define SD_SPI												SPI2
    #define SD_GPIO												GPIOB
    #define RCC_APBPeriphClockCmd_SD_SPI                            RCC_APB2PeriphClockCmd
    #define RCC_APBPeriph_SD_SPI					RCC_APB2Periph_SPI2
    #define RCC_APB2Periph_SD_IO					RCC_APB2Periph_GPIOB
    #define SPI_SD_PIN_SCK                                          GPIO_Pin_13
    #define SPI_SD_PIN_MISO                                         GPIO_Pin_14
    #define SPI_SD_PIN_MOSI						GPIO_Pin_15
        #define Remap_SPI_SD			GPIO_Remap_SPI3

#else
        #error "unsupported board"
#endif
