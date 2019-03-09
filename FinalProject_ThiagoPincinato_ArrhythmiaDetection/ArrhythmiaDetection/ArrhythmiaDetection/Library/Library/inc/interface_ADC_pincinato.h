/******************************************************************************
* @file     interface_ADC_pincinato.h
* @brief    This file handles ADC
* @author   Thiago Henrique Pincinato
* @version  V1.0
* @date     10.12.2012
* @hardware STM32F446RET6
******************************************************************************/
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
//#define lowFilterItemCount 5
#define TIMER_ADC htim5
#define ADC_INPUT hadc1

typedef struct ProcessData__{    
		BiquadsFilter HighPass;
		float Highbuf[ADCBUFFERLENGTH];
		BiquadsFilter LowPass;
		float Lowbuf[ADCBUFFERLENGTH];
 } _ProcessData;

static _ProcessData ADCprocess;
static bool readyToUpdate;

/*!
 * \brief initFilter            clear filters
 */
void initFilter(void);

/*!
 * \brief initADCInterface      initializes ADC intefac; initializes filter, set A and B Biquads coeff
 */
void initADCInterface(void);

/*!
 * \brief startADCInterface     call initfilter() and start interruption timer
 */
void startADCInterface(void);

/*!
 * \brief stopADCInterface      stop ADC and timer interruption
 */
void stopADCInterface(void);

/*!
 * \brief interruptTimerADCCallback      handle interruption and start ADC interruption
 */
void interruptTimerADCCallback(void);


/*!
 * \brief getReadyToUpdate      return the value that indicates if the interface is ready to update
 * \return                      true if it is ready, otherwise false.
 */
bool getReadyToUpdate(void);

/*!
 * \brief setReadyToUpdate      set the variable readyToUpdate
 * \param value                 bool value that will be attribuated to readyToUpdate
 */
void setReadyToUpdate(bool value);

/*!
 * \brief getADCData            returns a poitner to the float array of the filtered signal
 * \return                      pointer to an array of float
 */

const float *getADCData(void);

/*!
 * \brief getADCDataSize        returns the size of the array given by getADCData
 * \return                      int
 */
int getADCDataSize(void);

/*!
 * \brief normalizeADCData      normalize the actual filtered signal
 */
void normalizeADCData(void);

/*!
 * \brief removeADCUnstableValues   removes initial unstable values due to the IIR filters.
 */
void removeADCUnstableValues(void);
 
#ifdef __cplusplus
}
#endif

#endif /*INTERFACE_ADC_PINCINATO_H*/
