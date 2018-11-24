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


#define STATUSREGISTER 0x07
#define acellSlaveAdress 0x98
#define accelXoutRegister 0x00
#define TIMER_ACCEL htim3
#define ACCELEROMETER hi2c1
#define filterItemCount 30
#define integrationLength 10
#define accel_Category uint8_t
#define Category_HIGH 0xaa
#define Category_MEDIUM 0xc0
#define Category_LOW 0x0f

 typedef struct FilterData_{
   float sum;
   bool filterControl;
   int index;
   float data[filterItemCount];
 } FilterData;

 typedef struct IntegrationData_{
   int index;
   float n_1;
   float sum;
   float data[integrationLength];
 } IntegrationData;

 typedef struct ProcessData_{
    float Accelerometer_X;
    float calibrationY;
    float Accelerometer_Y;
    float Accelerometer_Z;
    float Temp_Displacement;
    FilterData X;
    FilterData Y;
    FilterData Z;
    IntegrationData VelocityY;
    IntegrationData DistanceY;
 } ProcessData;

static ProcessData AccelProcess;
static bool readValid;
static uint32_t AccelFreqSampling;
static uint32_t count_interrupt;

void initACCELInterface(void);
void startACCELInterface(void);
void stopACCELInterface(void);
bool getACCELCategory(accel_Category* valueDestination);
bool getDistance(double* valueDestination);
void clearData(void);
void clearFilter(void);
void clearIntegration(void);
float filterAdd(FilterData *filter, float value);
float integrate(IntegrationData *integral, float n);
float integrateDistance(IntegrationData *integral, float n);
float valueToG(uint8_t value);
float getACCELX(void);
void interruptTimerACCELCallback(void);
bool readAccel(void);
bool calibrate(void);

#ifdef __cplusplus
}
#endif

#endif /*INTERFACE_ACCEL_PINCINATO_H*/