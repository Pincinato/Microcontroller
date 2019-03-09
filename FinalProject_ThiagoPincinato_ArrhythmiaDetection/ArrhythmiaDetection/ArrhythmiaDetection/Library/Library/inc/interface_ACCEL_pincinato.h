/******************************************************************************
* @file     interface_ACCEL_pincinato.h
* @brief    This file handles accelometer functions
* @author   Thiago Henrique Pincinato
* @version  V1.0
* @date     10.12.2012
* @hardware mbef 0.16-1 with STM32F446RET6
******************************************************************************/
#ifndef INTERFACE_ACCEL_PINCINATO_H
#define INTERFACE_ACCEL_PINCINATO_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "i2c.h"
#include "tim.h"
#include "math.h"
#include "filter_math_pincinato.h"


#define STATUSREGISTER 0x07
#define acellSlaveAdress 0x98
#define accelXoutRegister 0x00
#define accelYoutRegister 0x01
#define accelZoutRegister 0x02
#define TIMER_ACCEL htim3
#define ACCELEROMETER hi2c1
#define filterItemCount 300
#define integrationLength 100 //  0.01*integrationLength= time of measurament.
#define accel_Category uint8_t
#define Category_HIGH 0xaa
#define Category_MEDIUM 0xc0
#define Category_LOW 0x0f


typedef struct ProcessData_{
    float Accelerometer_X;
    float calibrationY;
    float Accelerometer_Y;
    float Accelerometer_Z;
    float Temp_Displacement;
    AverageFilter X;
    AverageFilter Y;
	  float DataY[filterItemCount];
    AverageFilter Z;
    IntegrationData VelocityY;
		float DataVelocityY[integrationLength];
    IntegrationData DistanceY;
		float DataDistanceY[integrationLength];
 } ProcessData;

static ProcessData AccelProcess;
static bool readValid;
static uint32_t AccelFreqSampling;
static uint32_t count_interrupt;

/*!
 * \brief initACCELInterface        Initialize interface, initialize filter, intregationData, and I2C
 */
void initACCELInterface(void);

/*!
 * \brief startACCELInterface      Start interruption time and makes a callibration.
 */
void startACCELInterface(void);

/*!
 * \brief stopACCELInterface       Stop timer interruption
 */
void stopACCELInterface(void);

bool getACCELCategory(accel_Category* valueDestination);

/*!
 * \brief getDistance           read distance and clear integation
 * \param valueDestination      double pointer that receives the distance
 * \return                      true if value was set, othrwise false.
 */
bool getDistance(double* valueDestination);

/*!
 * \brief clearData             clear intergation, filter and varibles (set to zero)
 */
void clearData(void);

/*!
 * \brief valueToG              Converts the value of sensor to g value
 * \param value                 uint8_t value that contains the information to be converted
 * \return                      float value in g reference
 */
float valueToG(uint8_t value);

/*!
 * \brief getACCELX             get the value of the last value of  accelometer in X.
 * \return                      float value in "g" units
 */
float getACCELX(void);

/*!
 * \brief interruptTimerACCELCallback       Handles interruption of time. Set variable to indicate ready to read.
 */
void interruptTimerACCELCallback(void);

/*!
 * \brief readAccel             If a value is valid, it read the last accelometer data, convertes, and in the filter and integration
 * \return                      return true if operation was succesfull, otherwise false
 */
bool readAccel(void);

/*!
 * \brief calibrate             set the value of calibration by doing a fisrt measurement
 * \return                      return true if operation was succesfull, otherwise false
 */
bool calibrate(void);

#ifdef __cplusplus
}
#endif

#endif /*INTERFACE_ACCEL_PINCINATO_H*/
