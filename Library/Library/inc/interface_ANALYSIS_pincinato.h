#ifndef INTERFACE_ANALYSIS_PINCINATO_H
#define INTERFACE_ANALYSIS_PINCINATO_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "interface_ECG_pincinato.h"
#include "interface_ACCEL_pincinato.h"
#include "structs_pincinato.h"

#define LOWHEARTRATE    0
#define MEDIUMHEARTRATE 1
#define HIGHHEARTRATE   2
#define LOWACTIVITY     0
#define MEDIUMACTIVITY 1
#define HIGHACTIVITY   2

#define MEDIUMDISTANCE 1
#define LONGDISTANCE	3

#define BUFFERSIZEANALYSIS 5
#define BUFFERSIZEABNORMALITIES 5

typedef struct DataBuffer{

    uint8_t index;
    bool actual;
    uint32_t buf[BUFFERSIZEANALYSIS];
} myBuffer;

typedef struct DataBuffer_{

    uint8_t index;
    bool buf[BUFFERSIZEABNORMALITIES];
} myBuffer_;


typedef struct DataProcess___{

    myBuffer Activity;
    myBuffer HRLevel;
    myBuffer_ Abnormalities;

} AnalysisDataProcess;

static AnalysisDataProcess AnalysisProcess;

void initANALYSISInterface(Table *newTable);
void startANALYSISInterface(void);
void stopANALYSISInterface(void);
void clearANALYSISData(void);
bool getTypeOfActivity(void);
bool getTypeOfHeartLevel(void);
bool getAnalysis(char * HeartRateClassification,char *ActiviyClassification,bool *Abnormal);
void includeInBuffer(myBuffer *data ,uint32_t valueToInclude);
uint32_t getLastIncludedInBuffer(myBuffer *data);
void includeInBuffer_(myBuffer_ *data ,bool valueToInclude);
bool isAbnormal(myBuffer_ *data);
#ifdef __cplusplus
}
#endif

#endif /*INTERFACE_ANALYSIS_PINCINATO_H*/
