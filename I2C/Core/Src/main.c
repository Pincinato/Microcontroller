
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "stdio.h"
#include "string.h"
#include "lcd_driver.h"
#include "stdbool.h"
#include "joystick.h"
#include "lcd_menu.h"
#include "lcd_menu_definition.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define STATUSREGISTER 7
#define temperatureSensorSlaveAdress 0x90
#define temperatureSensorRegisterTemp 0x00
#define acellSlaveAdress 0x98
#define accelXoutRegister 0x00
#define filterItemCount 100

typedef struct FilterData_{
  float sum;
  int index;
  float data[filterItemCount];
} FilterData;

typedef struct ProcessData_{
  	float temperature;
	  float Accelerometer_X;
	  float Accelerometer_Y;
	  float Accelerometer_Z;
		FilterData X;
		FilterData Y;
		FilterData Z;
} ProcessData;

typedef struct AppData_{
	ProcessData current;
	ProcessData previous;
} AppData;

volatile uint32_t msMenuTicks=0; //!< Menu Update timer being modified by the SysTick interrupt
static AppData myData;  //must be static such that the compiler won't optimise. Used with callback

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
 float valueToG(uint8_t value);
void updateData(AppData *data);
void drawMenuItem_Callback2(int event, void* data);
void handleEvent(int event, AppData *data);
float filterAdd(FilterData *filter, float value);
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
	uint8_t accelSetupData[4]={STATUSREGISTER,0,STATUSREGISTER,1};
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
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */	
	HAL_GPIO_WritePin(LedGreen_GPIO_Port,LedGreen_Pin,GPIO_PIN_SET);
	lcd_init ();
	lcd_clear();
	lcd_setString(0,0,"Menu and callback",LCD_FONT_8,false);
	lcd_show();
	HAL_Delay(1000);
	HAL_GPIO_WritePin(LedBlue_GPIO_Port,LedBlue_Pin,GPIO_PIN_SET);
	HAL_I2C_Master_Transmit(&hi2c1, acellSlaveAdress,&accelSetupData[0],2,10);
	HAL_I2C_Master_Transmit(&hi2c1, acellSlaveAdress,&accelSetupData[2],2,10);
	menu_setMainMenu(&mainMenu);
	Joystick joystick;
  Joystick_init(&joystick);
  menu_navigation nextNavigation = MENU_NOP;
	menu_event event = MENU_NO_EVENT;
	menu_registerDrawMenuItem2(drawMenuItem_Callback2, (void *) &myData);  //!< define the callback function
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		joystick.sample(&joystick);
		updateData(&myData);
		HAL_Delay(5);
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

 float valueToG(uint8_t value){
	 static float returnValue;
	 returnValue= ((float)value)/21.33F;
	 static uint8_t temp;
	 if(value>31){
		 temp= ~(value);
		 temp++;
		 temp= temp & 0x1f;
		 returnValue= (-(float)temp)/21.33F;
 }
	 return returnValue;
}
 
void drawMenuItem_Callback2(int event, void* data){
  
  char buf[10]="";
	char bufY[10]="";
	char bufZ[10]="";
  AppData* ad = data; //!< Convert to our application data structure 
	switch (event) {
	case 1: 
		sprintf(buf, "%.3f", ad->current.temperature);
		strcat(buf," C ");
		lcd_setString(2,10,(const char*) &buf,LCD_FONT_8,false); 
	  break;	
	case 2:		
		sprintf(buf, "%.3f", ad->current.Accelerometer_X);
		strcat(buf,"g");
		lcd_setString(2,10,"                 ",LCD_FONT_8,false);
		lcd_setString(2,10,"X: ",LCD_FONT_8,false);	
	  lcd_setString(10,10,(const char*) &buf,LCD_FONT_8,false);
		sprintf(bufY, "%.3f", ad->current.Accelerometer_Y);
		strcat(bufY,"g");
		lcd_setString(2,20,"                 ",LCD_FONT_8,false);
		lcd_setString(2,20,"Y: ",LCD_FONT_8,false);	
	  lcd_setString(10,20,(const char*) &bufY,LCD_FONT_8,false);
		sprintf(bufZ, "%.3f", ad->current.Accelerometer_Z);
		strcat(bufZ,"g");
		lcd_setString(60,10,"                ",LCD_FONT_8,false);
		lcd_setString(60,10,"Z  ",LCD_FONT_8,false);	
	  lcd_setString(70,10,(const char*) &bufZ,LCD_FONT_8,false);			
    break;
	default: ;
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
	
  static float accelX_value;
  static float accelY_value;
  static float accelZ_value;
	uint8_t bufRX[4]="";
	static float temperature;
	static uint16_t temp = 0;
	uint32_t i2cstatus =0;

	  // Keep all data of last process image
	data->previous = data->current;
	i2cstatus =HAL_I2C_Master_Transmit(&hi2c1,temperatureSensorSlaveAdress ,(uint8_t *)temperatureSensorRegisterTemp,1,10);
	if(i2cstatus == HAL_OK){
		i2cstatus =HAL_I2C_Master_Receive(&hi2c1, temperatureSensorSlaveAdress,(uint8_t *)&bufRX[0],2,100);
		if(i2cstatus == HAL_OK){
			temp = bufRX[1] + bufRX[0]*256;
			temp = temp>>5;
			if(temp & (1<<10)) temp |= 0xF800;
			temperature = 0.125F * (float)temp;
			data->current.temperature=temperature;
		}
	}
	i2cstatus =HAL_I2C_Master_Transmit(&hi2c1, acellSlaveAdress,(uint8_t *)accelXoutRegister,1,10);
	if( i2cstatus	 == HAL_OK){
		i2cstatus =HAL_I2C_Master_Receive(&hi2c1, acellSlaveAdress,(uint8_t *)&bufRX[0],3,100);
		if( i2cstatus	 == HAL_OK){
			if ((((uint8_t)bufRX[0]>>7) &0x01)==0){
				accelX_value = valueToG(((uint8_t)bufRX[0]<<2)>>2);
				data->current.Accelerometer_X=filterAdd(&data->current.X,accelX_value);				
			}	
			if ((((uint8_t)bufRX[1]>>7) &0x01)==0){
				accelY_value = valueToG(((uint8_t)bufRX[1]<<2)>>2);
				data->current.Accelerometer_Y=filterAdd(&data->current.Y,accelY_value);
			}
			if ((((uint8_t)bufRX[2]>>7) &0x01)==0){
				accelZ_value = valueToG(((uint8_t)bufRX[2]<<2)>>2);
				data->current.Accelerometer_Z=filterAdd(&data->current.Z,accelZ_value);
			}
		}
	}
}

float filterAdd(FilterData *filter, float value){
	filter->sum -= filter->data[filter->index]; // throw away last value in ring
	filter->sum += value;                   // add new value to the sum of all values
	filter->data[filter->index++] = value;    // store current value and increment list index
  if (filter->index >= filterItemCount) { // wrap around after last index
    filter->index = 0;
	}
  return filter->sum / filterItemCount;	  // return average value
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
