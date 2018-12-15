#include "interface_ADC_pincinato.h"
#include "string.h"
#include "lcd_pincinato.h"
#include "lcd_pincinato.h"



void initADCInterface(void){
	float coef_A_LowPass[3]={ 0.09763076084032914,0.19526152168065827, 0.09763076084032914}; //fc 23
	float coef_B_LowPass[2]={ -0.9428060277021066,0.3333290710634233};
	float coef_A_HighPass[3]={ 0.8948577513857248, -1.7897155027714495,  0.8948577513857248}; //fc 4
	float coef_B_HighPass[2]={ -1.7786300789392977, 0.8008009266036016};
	initBiquadsFilter(&ADCprocess.HighPass,&ADCprocess.Highbuf[0],ADCBUFFERLENGTH,&coef_A_HighPass[0],&coef_B_HighPass[0]);
	initBiquadsFilter(&ADCprocess.LowPass,&ADCprocess.Lowbuf[0],ADCBUFFERLENGTH,&coef_A_LowPass[0],&coef_B_LowPass[0]);
}

void startADCInterface(void){

	initFilter();
	HAL_TIM_Base_Start_IT(&TIMER_ADC);

}
void stopADCInterface(void){
	HAL_TIM_Base_Stop_IT(&TIMER_ADC);
	HAL_ADC_Stop_IT(&ADC_INPUT);
}

void initFilter(void){
	readyToUpdate=false;
	clearBiquadsFilter(&ADCprocess.HighPass);
	clearBiquadsFilter(&ADCprocess.LowPass);
}

const float *getADCData(void){
	return &ADCprocess.HighPass.data[0];
}


int getADCDataSize(void){
	int bufSize =ADCBUFFERLENGTH;
	return bufSize;
}

void removeADCUnstableValues(void){
	int z=0;
	while(ADCprocess.HighPass.data[z]>0){ // delete initial invalid data
		ADCprocess.HighPass.data[z]=0;
		++z;
	}
}

void normalizeADCData(void){
	normalize(&ADCprocess.HighPass.data[0],ADCBUFFERLENGTH);
}


bool getReadyToUpdate(void){
	return readyToUpdate;
}

void setReadyToUpdate(bool value){
	readyToUpdate=value;
}
void interruptTimerADCCallback(void){
    HAL_ADC_Start_IT(&ADC_INPUT);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle){
	HAL_ADC_Stop_IT(&ADC_INPUT);
  if(!readyToUpdate){
		filterBiquadsCompute(&ADCprocess.HighPass,(uint32_t)filterBiquadsCompute(&ADCprocess.LowPass,HAL_ADC_GetValue(AdcHandle)));
		if(ADCprocess.HighPass.index==0){
					HAL_TIM_Base_Stop_IT(&TIMER_ADC);
					readyToUpdate=true;
		}
	}
}
/****** END OF FILE ******/
