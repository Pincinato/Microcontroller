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
#include "lcd_pincinato.h"
#include "lcd_pincinato.h"



void initADCInterface(void){
	float coef_A_LowPass[3]={ 0.09763076084032914,0.19526152168065827, 0.09763076084032914}; //fc 23
	float coef_B_LowPass[2]={ -0.9428060277021066,0.3333290710634233};
	float coef_A_HighPass[3]={ 0.8948577513857248, -1.7897155027714495,  0.8948577513857248}; //fc 5
	float coef_B_HighPass[2]={ -1.7786300789392977, 0.8008009266036016};
	initBiquadsFilter(&ADCprocess.HighPass,&ADCprocess.Highbuf[0],ADCBUFFERLENGTH,&coef_A_HighPass[0],&coef_B_HighPass[0]);
	initBiquadsFilter(&ADCprocess.LowPass,&ADCprocess.Lowbuf[0],ADCBUFFERLENGTH,&coef_A_LowPass[0],&coef_B_LowPass[0]);
	claerAverageFilter (&ADCprocess.HR);
	ADCprocess.HR_threshold=0.75;

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

void claerAverageFilter (AverageFilterData *filter){
	
		filter->index=0;
	  filter->sum=lowFilterItemCount*60;
		for(uint32_t i = 0 ; i< lowFilterItemCount; i++){
			filter->data[i]=60;
		}
}

float FilterAverageAdd(AverageFilterData *filter, float value){

        filter->sum -= filter->data[filter->index]; // throw away last value in ring
        filter->sum += value;                   // add new value to the sum of all values
        filter->data[filter->index++] = value;    // store current value and increment list index
        if (filter->index >= lowFilterItemCount) { // wrap around after last index
           filter->index = 0;
        }
  return filter->sum /lowFilterItemCount;	  // return average value
}

float getHeartRate(){
	
	static float ret;
	int heart_bits=0;
	int fs=200;
	int z=0;
	float localThreshold=ADCprocess.HR_threshold;
	while(ADCprocess.HighPass.data[z]>0){ // delete initial invalid data
		ADCprocess.HighPass.data[z]=0;
		++z;
	}
	normalize(&ADCprocess.HighPass.data[0],ADCBUFFERLENGTH);	
	//Detect heart bit
	while((heart_bits<2) & (localThreshold>0.55)){ //Adaptive threshold at lower 0.55
		for(int i=0;i<ADCBUFFERLENGTH;i=i+40){
			bool once_flag=false; 
			for(int j=i;j<i+40;j++){ //interval in which the max heart freq is contained -> avoid count of two peaks too close.
				if((ADCprocess.HighPass.data[j]>localThreshold)& (!once_flag)){
					once_flag=true;
					heart_bits++;
				}
			}
		}
		if(heart_bits<2){
			localThreshold=localThreshold-0.05; //decrement of 0.05
		}
	}
	//Compute Heart rate
	ret =((heart_bits)/((ADCBUFFERLENGTH-z)/fs))*60;
	return ret; //qty of bit / time of measure *60 - bpm

}

bool updateHeartRate(float* destinationValue){
	bool ack=false;
	if (readyToUpdate){
		*destinationValue=FilterAverageAdd(&ADCprocess.HR,getHeartRate());
		readyToUpdate=false;
		ack=true;		
		startADCInterface();
	}
	return ack;
}

void interruptTimerADCCallback(void){
    HAL_ADC_Start_IT(&ADC_INPUT);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle){
	HAL_ADC_Stop_IT(&ADC_INPUT);
  //FilterCompute(&ADCprocess.HighPass,(uint32_t)FilterCompute(&ADCprocess.LowPass,HAL_ADC_GetValue(AdcHandle)));
	if(!readyToUpdate){
		filterBiquadsCompute(&ADCprocess.HighPass,(uint32_t)filterBiquadsCompute(&ADCprocess.LowPass,HAL_ADC_GetValue(AdcHandle)));
		if(ADCprocess.HighPass.index==0){
					HAL_TIM_Base_Stop_IT(&TIMER_ADC);
					readyToUpdate=true;
		}
	}
}
/****** END OF FILE ******/
