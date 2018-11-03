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
#define filterItemCount 100 //to have measure in 1 s

 typedef struct FilterData_{
   float sum;
   int index;
   float data[filterItemCount];
 } FilterData;

 typedef struct ProcessData_{
    float Accelerometer_X;
    float Accelerometer_Y;
    float Accelerometer_Z;
    float Temp_Displacement;
    FilterData X;
    FilterData Y;
    FilterData Z;    
    FilterData SquareSumX2Y2Z2;
 } ProcessData;

static ProcessData AccelProcess;

void initACCELInterface(void);
void startACCELInterface(void);
void stopACCELInterface(void);
bool getACCELCategory(double* valueDestination);
void clearData(void);
float filterAdd(FilterData *filter, float value);
float valueToG(uint8_t value);
float getACCELX(void);
void interruptTimerACCELCallback(void);

#ifdef __cplusplus
}
#endif

#endif /*INTERFACE_ACCEL_PINCINATO_H*/
