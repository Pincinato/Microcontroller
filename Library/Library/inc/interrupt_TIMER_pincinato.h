#ifndef INTERRUPT_TIMER_PINCINATO_H
#define INTERRUPT_TIMER_PINCINATO_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "interface_ADC_pincinato.h"
#include "interface_ACCEL_pincinato.h"

 void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
     if (htim->Instance == TIMER_ADC.Instance){
        interruptTimerADCCallback();
     }
     else if (htim->Instance == TIMER_ACCEL.Instance){
        interruptTimerACCELCallback();
     }
 }

#ifdef __cplusplus
}
#endif

#endif /*INTERRUPT_TIMER_PINCINATO_H*/
