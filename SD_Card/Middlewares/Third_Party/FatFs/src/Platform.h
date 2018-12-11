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
//#define USE_STM32_P103
//#define USE_MCBSTM32C
//#define USE_STM3210C
#define USE_STM32F4

/* Include -------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdio.h>

/* Olimex STM32-P103 Board ---------------------------------------------------*/
#if defined(USE_STM32_P103)
	/* LED */
	#define GPIO_LED		GPIOC
	#define LED0				GPIO_Pin_12
	
	/* Button */
	#define GPIO_Button	GPIOA
	#define Button0			GPIO_Pin_0

	/* SPI SD */
	#define SD_SPI												SPI2
	#define SD_GPIO												GPIOB
	#define RCC_APBPeriphClockCmd_SD_SPI	RCC_APB2PeriphClockCmd
	#define RCC_APBPeriph_SD_SPI					RCC_APB2Periph_SPI2
	#define RCC_APB2Periph_SD_IO					RCC_APB2Periph_GPIOB
	#define SPI_SD_PIN_SCK								GPIO_Pin_13
	#define SPI_SD_PIN_MISO								GPIO_Pin_14
	#define SPI_SD_PIN_MOSI								GPIO_Pin_15

	/* I2C1 */
	#define I2C1_GPIO										GPIOB
	#define I2C1_GPIO_PIN_SCL						GPIO_Pin_6
	#define I2C1_GPIO_PIN_SDA						GPIO_Pin_7
	#define I2C1_REMAP_GPIO_PIN_SCL			GPIO_Pin_8
	#define I2C1_REMAP_GPIO_PIN_SDA			GPIO_Pin_9
	#define RCC_APBPeriphClockCmd_I2C1	RCC_APB2PeriphClockCmd
	#define RCC_APB2Periph_I2C1_IO			RCC_APB2Periph_GPIOB
	
	/* I2C2 */
	#define I2C2_GPIO										GPIOB
	#define I2C2_GPIO_PIN_SCL						GPIO_Pin_10
	#define I2C2_GPIO_PIN_SDA						GPIO_Pin_11
	#define RCC_APBPeriphClockCmd_I2C2	RCC_APB2PeriphClockCmd
	#define RCC_APB2Periph_I2C2_IO			RCC_APB2Periph_GPIOB
	
	/* Remaps */

/* Keil MCBSTM32C Board ------------------------------------------------------*/
#elif defined(USE_MCBSTM32C)
	/* LED */
	#define GPIO_LED				GPIOE
	#define LED0						GPIO_Pin_8
	#define LED1						GPIO_Pin_9
	#define LED2						GPIO_Pin_10
	#define LED3						GPIO_Pin_11
	#define LED4						GPIO_Pin_12
	#define LED5						GPIO_Pin_13
	#define LED6						GPIO_Pin_14
	#define LED7						GPIO_Pin_15

	/* SPI SD */
	#define SD_SPI												SPI1
	#define SD_GPIO												GPIOA
	#define RCC_APBPeriphClockCmd_SD_SPI	RCC_APB2PeriphClockCmd
	#define RCC_APBPeriph_SD_SPI					RCC_APB2Periph_SPI1
	#define RCC_APB2Periph_SD_IO					RCC_APB2Periph_GPIOA
	#define SPI_SD_PIN_SCK								GPIO_Pin_5
	#define SPI_SD_PIN_MISO								GPIO_Pin_6
	#define SPI_SD_PIN_MOSI								GPIO_Pin_7

	/* I2C1 */
	#define I2C1_GPIO									GPIOB
	#define I2C1_GPIO_PIN_SCL					GPIO_Pin_6
	#define I2C1_GPIO_PIN_SDA					GPIO_Pin_7
	#define I2C1_REMAP_GPIO_PIN_SCL		GPIO_Pin_8
	#define I2C1_REMAP_GPIO_PIN_SDA		GPIO_Pin_9
	#define RCC_APBPeriphClockCmd_I2C1	RCC_APB2PeriphClockCmd
	#define RCC_APB2Periph_I2C1_IO			RCC_APB2Periph_GPIOB
	
	/* I2C2 */
	#define I2C2_GPIO									GPIOB
	#define I2C2_GPIO_PIN_SCL					GPIO_Pin_10
	#define I2C2_GPIO_PIN_SDA					GPIO_Pin_11
	#define RCC_APBPeriphClockCmd_I2C2	RCC_APB2PeriphClockCmd
	#define RCC_APB2Periph_I2C2_IO			RCC_APB2Periph_GPIOB

	/* I2C TS */
	#define	TS_I2C										I2C1
	#define RCC_APB1Periph_TS_I2C			RCC_APB1Periph_I2C1
	#define RCC_APB2Periph_TS_I2C_IO	RCC_APB2Periph_GPIOB
	
	/* I2C Accelerometer */
//	#define	Acc_I2C											I2C1
//	#define RCC_APB1Periph_Acc_I2C			RCC_APB1Periph_I2C1
//	#define RCC_APB2Periph_Acc_I2C_IO		RCC_APB2Periph_GPIOB
	
	/* Remaps */
	#define USE_I2C1_Remap
	#define Remap_I2C1				GPIO_Remap_I2C1

/* ST STM3210C Board ---------------------------------------------------------*/
#elif defined(USE_STM3210C)
	/* LED */
	#define GPIO_LED		GPIOD
	#define LED1				GPIO_Pin_7
	#define LED2				GPIO_Pin_13
	#define LED3				GPIO_Pin_3
	#define LED4				GPIO_Pin_4

	/* SPI SD */
	#define SD_SPI												SPI3
	#define SD_GPIO												GPIOC
	#define RCC_APBPeriphClockCmd_SD_SPI	RCC_APB1PeriphClockCmd
	#define RCC_APBPeriph_SD_SPI					RCC_APB1Periph_SPI3
	#define RCC_APB2Periph_SD_IO					RCC_APB2Periph_GPIOC
	#define SPI_SD_PIN_SCK								GPIO_Pin_10
	#define SPI_SD_PIN_MISO								GPIO_Pin_11
	#define SPI_SD_PIN_MOSI								GPIO_Pin_12

	/* I2C1 */
	#define I2C1_GPIO									GPIOB
	#define I2C1_GPIO_PIN_SCL					GPIO_Pin_6
	#define I2C1_GPIO_PIN_SDA					GPIO_Pin_7
	#define I2C1_REMAP_GPIO_PIN_SCL		GPIO_Pin_8
	#define I2C1_REMAP_GPIO_PIN_SDA		GPIO_Pin_9
	#define RCC_APBPeriphClockCmd_I2C1	RCC_APB2PeriphClockCmd
	#define RCC_APB2Periph_I2C1_IO			RCC_APB2Periph_GPIOB
	
	/* I2C2 */
	#define I2C2_GPIO									GPIOB
	#define I2C2_GPIO_PIN_SCL					GPIO_Pin_10
	#define I2C2_GPIO_PIN_SDA					GPIO_Pin_11
	#define RCC_APBPeriphClockCmd_I2C2	RCC_APB2PeriphClockCmd
	#define RCC_APB2Periph_I2C2_IO			RCC_APB2Periph_GPIOB

	/* I2C TS */
	#define	TS_I2C										I2C1
	#define RCC_APB1Periph_TS_I2C			RCC_APB1Periph_I2C1
	#define RCC_APB2Periph_TS_I2C_IO	RCC_APB2Periph_GPIOB

	/* Remaps */
	#define USE_SPI_SD_Remap
	#define Remap_SPI_SD			GPIO_Remap_SPI3
/* ST STM32F4 Board ---------------------------------------------------------*/
#elif defined(USE_STM32F4)
        /* SPI SD */
        #define SD_SPI				SPI2
        #define SD_GPIO                         GPIOB
        #define RCC_APBPeriphClockCmd_SD_SPI	RCC_APB1PeriphClockCmd
        #define RCC_APBPeriph_SD_SPI		RCC_APB1Periph_SPI2
        #define RCC_APB2Periph_SD_IO		RCC_APB2Periph_GPIOB
        #define SPI_SD_PIN_SCK			GPIO_Pin_13
        #define SPI_SD_PIN_MISO			GPIO_Pin_14
        #define SPI_SD_PIN_MOSI			GPIO_Pin_15

        /* Remaps */
        #define USE_SPI_SD_Remap
        #define Remap_SPI_SD			GPIO_Remap_SPI2


#else
	#error "unsupported board"
#endif
