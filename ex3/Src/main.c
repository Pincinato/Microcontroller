
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Menu gives us two option.
	* @brief					: Option LEd Control :
	* @brief   				: The User can send a message to turn on or off a led via USB
	* @brief   				: The messages should start with ? end and with !
	* @brief   				: The option of led are Red(red) Gre(Green) Blu(Blue).
	* @brief   				: To turn off 0 to turn on 1.
	* @brief   				: Exemple of message : ?Blu1! turn on the led blue
	* @brief   				: the option of led are Red(red) Gre(Green) Blu(Blue).
	
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "dma.h"
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
#include "lcd_driver.h"
#include "interface_USART_pincinato.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//TEST
typedef struct MainProcessData_{
 
	bool LedRed;
	bool LedGreen;
	bool LedBlue;
	
} MainProcessData;

typedef struct AppData_{
	MainProcessData current;
	MainProcessData previous;
} AppData;

volatile uint32_t msMenuTicks=0;
static AppData myData;
static int MainEvent;
//

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
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	HAL_GPIO_WritePin(LedBlue_GPIO_Port,LedBlue_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LedGreen_GPIO_Port,LedGreen_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET);
	
	initInterface(&huart2);
	lcd_init();
	
	//startECGInterface();
	
	//startACCELInterface();
	lcd_clear();
	lcd_setString(2,10," Start ",LCD_FONT_8,false);
	lcd_show();
	HAL_Delay(100);
	lcd_clear();
	//TEST
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
  
	char bufRed[20]="";
	char bufBlue[20]="";
	char bufGreen[20]="";
	MainEvent=event;
  AppData* ad = data; //!< Convert to our application data structure 
	if(event==2){		
		sprintf(bufRed,"LedRED:%d",ad->current.LedRed);
		sprintf(bufBlue,"LedBLUE:%d",ad->current.LedBlue);
		sprintf(bufGreen,"LedGREEN%d",ad->current.LedGreen);		
		lcd_setString(2,10,(const char*) &bufRed,LCD_FONT_8,false);
		lcd_setString(2,18,(const char*) &bufBlue,LCD_FONT_8,false);		
		lcd_setString(55,10,(const char*) &bufGreen,LCD_FONT_8,false);		
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
	

	  // Keep all data of last process image
	data->previous = data->current;

	switch (MainEvent) {
		case 1: check_RX();
			break;
		case 2:  data->current.LedBlue= !HAL_GPIO_ReadPin(LedBlue_GPIO_Port,LedBlue_Pin);
						 data->current.LedGreen=!HAL_GPIO_ReadPin(LedGreen_GPIO_Port,LedGreen_Pin);	
						 data->current.LedRed=!HAL_GPIO_ReadPin(LED_RED_GPIO_Port,LED_RED_Pin);					
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