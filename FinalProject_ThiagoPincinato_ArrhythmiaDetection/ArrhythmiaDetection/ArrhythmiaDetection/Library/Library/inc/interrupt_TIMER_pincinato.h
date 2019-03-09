/******************************************************************************
* @file     interface_TIMER_pincinato.h
* @brief    This file handle timer interruptions
* @author   Thiago Henrique Pincinato
* @version  V1.0
* @date     10.12.2012
* @hardware STM32F446RET6
******************************************************************************/
#ifndef INTERRUPT_TIMER_PINCINATO_H
#define INTERRUPT_TIMER_PINCINATO_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "interface_ADC_pincinato.h"
#include "interface_ACCEL_pincinato.h"

 /*!
 * \brief HAL_TIM_PeriodElapsedCallback         handle interruptions of timer
 * \param htim                                  timer of interruption
 */
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
