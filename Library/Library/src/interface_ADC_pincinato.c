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




void initADCInterface(void){
	float coef_A_HighPass[3]={ 0.09763076084032914,0.19526152168065827, 0.09763076084032914};
	float coef_B_HighPass[2]={ -0.9428060277021066,0.3333290710634233};
	float coef_A_LowPass[3]={0.9944617362863962, -1.9889234725727925, 0.9944617362863962};
	float coef_B_LowPass[2]={ -1.9888928005576803, 0.9889541445879048};
	setCoef(&ADCprocess.HighPass,coef_A_HighPass,coef_B_HighPass);
	setCoef(&ADCprocess.LowPass,coef_A_LowPass,coef_B_LowPass);

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
void setCoef(BiquadsFilter * filter,float Acoef[],float Bcoef[]){

	filter->a0=Acoef[0];
	filter->a1=Acoef[1];
	filter->a2=Acoef[2];
	filter->b1=Bcoef[0];
	filter->b2=Bcoef[1];
}
void clearBiquadsFilter(BiquadsFilter * filter){
		
		filter->index=0;
		filter->n_1=0;
		filter->n_2=0;
		filter->dataN_1=0;
		filter->dataN_2=0;
		for(uint32_t i = 0 ; i< ADCBUFFERLENGTH; i++){
			filter->data[i]=0;
		}
}


void interruptTimerADCCallback(void){
    HAL_ADC_Start_IT(&ADC_INPUT);
}

float FilterComputeUint(BiquadsFilter *filter, uint32_t adcValue){

	if(!readyToUpdate){
			filter->data[filter->index] = (filter->a0*adcValue) + (filter->a1*filter->n_1) + (filter->a2*filter->n_2) - (filter->b1*filter->dataN_1) - (filter->b2*filter->dataN_2);
			filter->n_2 = filter->n_1;
			filter->n_1 = adcValue;
			filter->dataN_2 = filter->dataN_1;
			filter->dataN_1 = filter->data[filter->index];	
			++filter->index;		
			if(filter->index>=ADCBUFFERLENGTH){
				filter->index=0;
				//if(filter==&ADCprocess.LowPass){
				//	HAL_TIM_Base_Stop_IT(&TIMER_ADC);
				//	readyToUpdate=true;
				//}					
			}
		}		
			return filter->dataN_1;// first return index after increments
}


void FilterComputeFloat(BiquadsFilter *filter, float value){
	
	if(!readyToUpdate){
			filter->data[filter->index] = (filter->a0*value) + (filter->a1*filter->n_1) + (filter->a2*filter->n_2) - (filter->b1*filter->dataN_1) - (filter->b2*filter->dataN_2);
			filter->n_2 = filter->n_1;
			filter->n_1 = value;
			filter->dataN_2 = filter->dataN_1;
			filter->dataN_1 = filter->data[filter->index];
			++filter->index;
			if(filter->index>=ADCBUFFERLENGTH){
				filter->index=0;
				HAL_TIM_Base_Stop_IT(&TIMER_ADC);
				readyToUpdate=true;
			}
		}		
}

float getHeartRate(){

	/*
	int heart_bits=0;
	int fs=200;
	static float ret;
	for(int i=0;i<ADCBUFFERLENGTH;i=i+40){
		bool once_flag=false;
		for(int j=i;j<i+40;j++){
			if((ADCprocess.Data.data[j]>300)& (!once_flag)){
				once_flag=true;
				heart_bits++;
			}
		}
	}
	//DEtect heart bit
	//Compute Heart rate
	ret =((heart_bits)/(ADCBUFFERLENGTH/fs))*60;
	return ret; //qty of bit / time of measure *60 - bpm
	*/
}

bool updateHeartRate(float* destinationValue){
	bool ack=false;
	if (readyToUpdate){
		*destinationValue=ADCprocess.LowPass.dataN_1;
		readyToUpdate=false;
		ack=true;		
		startADCInterface();
	}
	return ack;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle){
	HAL_ADC_Stop_IT(&ADC_INPUT);
  FilterComputeFloat(&ADCprocess.LowPass,(uint32_t)FilterComputeUint(&ADCprocess.HighPass,HAL_ADC_GetValue(AdcHandle)));
				
}
/****** END OF FILE ******/
