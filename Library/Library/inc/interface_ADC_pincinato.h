#ifndef INTERFACE_ADC_PINCINATO_H
#define INTERFACE_ADC_PINCINATO_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "adc.h"
#include "tim.h"

#define ADCBUFFERLENGTH 10
#define TIMER_ADC htim5
#define ADC_INPUT hadc1

void initADCInterface(void);
void startADCInterface(void);
void stopADCInterface(void);
bool setFs(uint32_t valueInHz);
bool getADCValue(uint32_t *valueDestination);
bool ADCIsEmpty(void);
bool ADCIsFull(void);
bool IncludeADCValue(uint32_t value);
void clearBuffer(void);
void checkEndBuffer(void);
void interruptTimerADCCallback(void);

#ifdef __cplusplus
}
#endif

#endif /*INTERFACE_ADC_PINCINATO_H*/
