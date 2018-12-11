/******************************************************************************
* @file     interface_ECG_pincinato.h
* @brief    This file handle ADC interafce
* @author   Thiago Henrique Pincinato
* @version  V1.0
* @date     10.12.2012
* @hardware STM32F446RET6
******************************************************************************/
#ifndef INTERFACE_ECG_PINCINATO_H
#define INTERFACE_ECG_PINCINATO_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "filter_math_pincinato.h"

#define lowFilterItemCount 5

typedef struct ProcessData___{
                AverageFilter HR;
                float HR_threshold;
                float HRbuf[lowFilterItemCount];
 } __ProcessData;

static __ProcessData ECGprocess;


/*!
 * \brief initFilterHR         not used
 */
void initFilterHR(void);

/*!
 * \brief initECGInterface      initialize variables, filter and ADC interafece
 */
void initECGInterface(void);


/*!
 * \brief startECGInterface     start ADC interface
 */
void startECGInterface(void);

/*!
 * \brief stopECGInterface      stop ADC interface
 */
void stopECGInterface(void);

/*!
 * \brief getHeartRate          Compute the Heart Rate based on the result of the ADC interfeace filter
 * \return                      float with the Heart Rate
 */
float getHeartRate(void);

/*!
 * \brief updateHeartRate       return the last computed Heart Rate
 * \param destinationValue      float pointer that recieves the Heart Rate value
 * \return                      true if the value was updated , otherwise false
 */
bool updateHeartRate(float* destinationValue);
 
#ifdef __cplusplus
}
#endif

#endif /*INTERFACE_ECG_PINCINATO_H*/
