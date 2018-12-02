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

void initFilterHR(void);
void initECGInterface(void);
void startECGInterface(void);
void stopECGInterface(void);
float getHeartRate(void);
bool updateHeartRate(float* destinationValue);
 
#ifdef __cplusplus
}
#endif

#endif /*INTERFACE_ECG_PINCINATO_H*/
