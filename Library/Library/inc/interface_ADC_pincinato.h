#ifndef INTERFACE_ADC_PINCINATO_H
#define INTERFACE_ADC_PINCINATO_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "adc.h"
#include "tim.h"
#include "filter_math_pincinato.h"


#define ADCBUFFERLENGTH 800
#define lowFilterItemCount 5
#define TIMER_ADC htim5
#define ADC_INPUT hadc1

typedef struct ProcessData__{    
		BiquadsFilter HighPass;
		float Highbuf[ADCBUFFERLENGTH];
		BiquadsFilter LowPass;
		float Lowbuf[ADCBUFFERLENGTH];
		AverageFilter HR;
		float HR_threshold;
		float HRbuf[lowFilterItemCount];
 } _ProcessData;

static _ProcessData ADCprocess;
static bool readyToUpdate;
void initFilter(void);
void initADCInterface(void);
void startADCInterface(void);
void stopADCInterface(void);
void interruptTimerADCCallback(void);
float getHeartRate(void);
bool updateHeartRate(float* destinationValue);
 
#ifdef __cplusplus
}
#endif

#endif /*INTERFACE_ADC_PINCINATO_H*/
