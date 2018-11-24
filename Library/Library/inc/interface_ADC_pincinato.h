#ifndef INTERFACE_ADC_PINCINATO_H
#define INTERFACE_ADC_PINCINATO_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "adc.h"
#include "tim.h"


#define ADCBUFFERLENGTH 800
#define lowFilterItemCount 5
#define TIMER_ADC htim5
#define ADC_INPUT hadc1

typedef struct BiquadsFilter_{
   
	 float a0;
	 float a1;
	 float a2;
	 float b1;
	 float b2;
	 uint32_t n_1;
	 uint32_t n_2;
	 float dataN_1;
	 float dataN_2;
   int index;
   float data[ADCBUFFERLENGTH];
} BiquadsFilter;

  typedef struct _FilterData_{
   float sum;
   int index;
   float data[lowFilterItemCount];
 } AverageFilterData;
	
  typedef struct ProcessData__{    
		BiquadsFilter HighPass;
		BiquadsFilter LowPass;
		AverageFilterData HR;
 } _ProcessData;

static _ProcessData ADCprocess;
static bool readyToUpdate;
void initFilter(void);
void initADCInterface(void);
void startADCInterface(void);
void stopADCInterface(void);
void clearBiquadsFilter(BiquadsFilter * filter);
void claerAverageFilter (AverageFilterData *filter);
void interruptTimerADCCallback(void);
float FilterCompute(BiquadsFilter *filter, uint32_t adcValue);
float FilterAverageAdd(AverageFilterData *filter, float value);
float getHeartRate(void);
bool updateHeartRate(float* destinationValue);
void setCoef(BiquadsFilter * filter,float Acoef[],float Bcoef[]);
#ifdef __cplusplus
}
#endif

#endif /*INTERFACE_ADC_PINCINATO_H*/
