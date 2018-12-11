
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "joystick.h"
#include "lcd_menu.h"
#include "lcd_menu_definition.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "structs_pincinato.h"
#include "lcd_pincinato.h"
#include "interrupt_TIMER_pincinato.h"
#include "interface_ACCEL_pincinato.h"
#include "interface_ECG_pincinato.h"
#include "interface_USART_pincinato.h"
#include "interface_ANALYSIS_pincinato.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//TEST
typedef struct MainProcessData_{
 
	
	char bufHR[9];
	char bufActivity[9];
	double distance;
	float HeartRate;
	
} MainProcessData;

typedef struct AppData_{
	MainProcessData current;
	MainProcessData previous;
} AppData;

volatile uint32_t msMenuTicks=0;
static AppData myData;
static int MainEvent;
static int last_MainEvent;
//

FIL myfile;
FATFS fs = {0};

/* Private variables ---------------------------------------------------------*/
//#define POT_1_HANDLE hadc1
#define POT_2_HANDLE hadc2 


void drawMenuItem_Callback2(int event, void* data);
void handleEvent(int event, AppData *data);
void updateData(AppData *data);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void ClearString(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_ADC2_Init();
  MX_TIM3_Init();
  MX_TIM5_Init();
  MX_I2C1_Init();
  MX_ADC1_Init();
  MX_SPI2_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	HAL_GPIO_WritePin(LedBlue_GPIO_Port,LedBlue_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LedGreen_GPIO_Port,LedGreen_Pin,GPIO_PIN_SET);
	
	//Init Pincinato's Interface
	Table myTable;
	initTable(&myTable);
	initInterface(&myTable,&huart2);
	initANALYSISInterface(&myTable);
	initLCD();
	initECGInterface();
	initACCELInterface();
	lcd_clear();
	lcd_setString(2,10," Start ",LCD_FONT_8,false);
	lcd_show();
	HAL_Delay(100);
	lcd_clear();
	
/*
	//SD test
	disk_initialize(fs.drv);
	f_mount(&fs,"SDCARD",FA_WRITE);
	f_open(&myfile,"data.txt",FA_WRITE);
	uint32_t written;
	f_write(&myfile,"test ",5,&written);
	f_close(&myfile);
	f_mount(0,NULL,FA_WRITE);	
	
*/	
	
	//Menu initialization
	menu_setMainMenu(&mainMenu);
	Joystick joystick;
  Joystick_init(&joystick);
  menu_navigation nextNavigation = MENU_NOP;
	menu_event event = MENU_NO_EVENT;
	menu_registerDrawMenuItem2(drawMenuItem_Callback2, (void *) &myData); 
	//
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

		joystick.sample(&joystick);
		updateData(&myData);
		if (msMenuTicks >= 100) {  // LCD display ok with 50ms, otherwise stray characters
			msMenuTicks = 0;
      nextNavigation = (menu_navigation) joystick.getDirection(&joystick);
      event = menu_update(nextNavigation);
			menu_show();
			handleEvent((int) event, &myData);
		}		
	}
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

void drawMenuItem_Callback2(int event, void* data){
  
	char buf[30]="";
	last_MainEvent=MainEvent;
	MainEvent=event;
  AppData* ad = data; //!< Convert to our application data structure 
	if(last_MainEvent == MainEvent){
			switch (MainEvent) {
			case 1:  sprintf(buf, "%.5f g",ad->current.distance);
								lcd_setString(12,10,(const char*) &buf,LCD_FONT_8,false);
				break;
			case 2:		sprintf(buf, "%d          ",(uint32_t)ad->current.HeartRate);
								lcd_setString(20,10,(const char*) &buf,LCD_FONT_8,false);
				break;
			case 3:	lcd_setString(20,10,"        ",LCD_FONT_8,false);
							lcd_setString(20,10,(const char*) &ad->current.bufHR,LCD_FONT_8,false);
							lcd_setString(28,20,"        ",LCD_FONT_8,false);
							lcd_setString(28,20,(const char*) &ad->current.bufActivity,LCD_FONT_8,false);
				break;
			case 4: 
				break;
			default:
				break;
		}
	}		
	if (last_MainEvent != MainEvent){
		switch (MainEvent) {
			case 1: lcd_setString(2,10, "Calibrating ...   ",LCD_FONT_8,false); 
							lcd_show();
				break;
			case 2: lcd_setString(2,10, "Doing 1 measure     ",LCD_FONT_8,false);
							lcd_show();
			case 3:	lcd_setString(2,10, "Calibrating ...",LCD_FONT_8,false);
							lcd_show();
				break;
			case 4: lcd_setString(2,10, "Usb connection",LCD_FONT_8,false);
							lcd_show();
				break;
			default:
				break;
		}
	}
}


/**
* @param event The menu event to be handled
* @param *data Pointer to the application data
*/
void handleEvent(int event, AppData *data) {
}

/**
* Updates all data
* @param data Pointer to application data
*/
void updateData(AppData *data) {
	
	double distance=0.0;
	float HeartRate=0.0;
	char bufHR[9]="";
	char bufActivity[9]="";
	bool abnormality;
	  // Keep all data of last process image
	data->previous = data->current;

	if(last_MainEvent != MainEvent){
		switch (last_MainEvent) {
		case 1: stopACCELInterface();
			break;
		case 2:	stopECGInterface();
			break;
		case 3: stopANALYSISInterface();						
						HAL_GPIO_WritePin(LedBlue_GPIO_Port,LedBlue_Pin,GPIO_PIN_SET);
						HAL_GPIO_WritePin(LedGreen_GPIO_Port,LedGreen_Pin,GPIO_PIN_SET);
			break;
		case 4: 
		default:stopACCELInterface();
						stopECGInterface();
						stopANALYSISInterface();
			break;
		}
		switch (MainEvent) {
		case 1: startACCELInterface();
						data->current.distance=0;
						lcd_setString(2,10, "Y:              ",LCD_FONT_8,false); 
						lcd_show();
			break;
		case 2:	startECGInterface();
						lcd_setString(2,10, "HR:              ",LCD_FONT_8,false); 
						lcd_show();
			break;
		case 3: startANALYSISInterface();
						HAL_GPIO_WritePin(LedGreen_GPIO_Port,LedGreen_Pin,GPIO_PIN_RESET);						
						lcd_setString(2,10, "HR:              ",LCD_FONT_8,false); 					
						lcd_setString(2,20, "Act:              ",LCD_FONT_8,false); 
						lcd_show();
			break;
		default:
			break;
		}
	}
	switch (MainEvent) {
		case 1: readAccel();
						if(getDistance(&distance)){
							if(distance!=data->current.distance ){
								data->current.distance=distance+data->previous.distance;// +data->previous.distance;
							}
						}
			break;
		case 2:	if(updateHeartRate(&HeartRate)){
							if(HeartRate!=data->current.HeartRate){
								data->current.HeartRate=HeartRate;
							}
						}
			break;
		case 3: if(getAnalysis(&bufHR[0],&bufActivity[0],&abnormality)){
							strcpy(data->current.bufHR,bufHR);
							strcpy(data->current.bufActivity,bufActivity);
							if(abnormality){
								HAL_GPIO_WritePin(LedGreen_GPIO_Port,LedGreen_Pin,GPIO_PIN_SET);
								HAL_GPIO_WritePin(LedBlue_GPIO_Port,LedBlue_Pin,GPIO_PIN_RESET);						
							}						
			}							
			break;
		case 4: check_RX();
			break;
		default:
			break;
		}
		
}

void HAL_SYSTICK_Callback(void) {
	msMenuTicks++;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
