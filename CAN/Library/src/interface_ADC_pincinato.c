/***************************************************************************************************
  * @file
  * @author     ?? original author has to be given as well !!	
  * @version    V0.4
	* @date       30.09.2018
	* @date       20.04.2018
  * @brief      Display setup and communication for LCD on mbed16-application-shield
	* @brief      V0.4;dnd1;add;drawLine() function
  * @hardware   NucleoF446RE with mbed-016.1-application-shield
  **************************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2018 BFH-TI </center></h2>
  *
  */
#include "interface_ADC_pincinato.h"
#include "string.h"
//#include "lcd_pincinato.h"

#define ADCBUFFERLENGTH 10
#define TIMER_ADC htim5
#define ADC_INPUT hadc1

uint32_t ADCBuf[ADCBUFFERLENGTH];
uint32_t last_read = 0;
uint32_t last_added = 0;

void initADCInterface(void){

	MX_TIM5_Init();
  MX_ADC1_Init();
}
void startADCInterface(void){

	clearBuffer();
	HAL_TIM_Base_Start_IT(&TIMER_ADC);

}
void stopADCInterface(void){
	HAL_TIM_Base_Stop_IT(&TIMER_ADC);
	HAL_ADC_Stop_IT(&ADC_INPUT);
}

bool setFs(uint32_t valueInHz){	
	TIMER_ADC.Instance->CCR1=(100000/valueInHz)/2;
	TIMER_ADC.Instance->ARR = (100000/valueInHz); //100000= 100Khz = time after pre scale
	TIMER_ADC.Instance->CNT=0;
	return true;

	
}
bool getADCValue(uint32_t *valueDestination){

	bool ACK =false;
	if ((ADCIsEmpty()==false) & (last_read < last_added)){
			*valueDestination = ADCBuf[last_read];
			last_read++;
			checkEndBuffer();
			ACK=true;
	}
	return ACK;
}

bool ADCIsEmpty(void){

		if(last_added == 0)
			return true;
		return false;
}

bool ADCIsFull(void){	
			if (last_added >= ADCBUFFERLENGTH)
					return true;
			return false;
}

bool IncludeADCValue(uint32_t value){
			//drawEachPixelGraph(value);
			bool ACK= false;
			if(ADCIsFull()==false){
				ADCBuf[last_added]=value;
				last_added++;
				ACK=true;
			}
			return ACK;
}

void clearBuffer(void){
		
		last_read= 0;
		last_added= 0;	
		for (uint32_t i = 0 ; i< ADCBUFFERLENGTH; i++)
			ADCBuf[i]=0;
}

void checkEndBuffer(void){

	if ((last_added >= ADCBUFFERLENGTH) &(last_read >= ADCBUFFERLENGTH )){
		last_added=0;
		last_read=0;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if (htim->Instance == TIMER_ADC.Instance)
    {
			HAL_ADC_Start_IT(&ADC_INPUT);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle){
        IncludeADCValue(HAL_ADC_GetValue(AdcHandle));
				HAL_ADC_Stop_IT(&ADC_INPUT);
}
/****** END OF FILE ******/
