/******************************************************************************
* @file     interface_ANALYSIS_pincinato.h
* @brief    This file analysis the results of ECG and ACCEL interface
* @author   Thiago Henrique Pincinato
* @version  V1.0
* @date     10.12.2012
* @hardware STM32F446RET6
******************************************************************************/
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


/*!
 * \brief initANALYSISInterface     clear data and set local table
 * \param newTable                  new table that will be assigned to localTable variable
 */
void initANALYSISInterface(Table *newTable);

/*!
 * \brief startANALYSISInterface    clear data and initialize ECG and ACCEL interface
 */
void startANALYSISInterface(void);

/*!
 * \brief stopANALYSISInterface     stop ECG and ACCEL interface
 */
void stopANALYSISInterface(void);

/*!
 * \brief clearANALYSISData         clear all data , including AnalysisProcess
 */
void clearANALYSISData(void);

/*!
 * \brief getTypeOfActivity         analyses the value of accel inteface and add the result in the AnalysisProcess
 * \return                          true if a new value was added, false if not.
 */
bool getTypeOfActivity(void);

/*!
 * \brief getTypeOfHeartLevel       analyses the value of ECG inteface and add the result in the AnalysisProcess
 * \return                          true if a new value was added, false if not.
 */
bool getTypeOfHeartLevel(void);

/*!
 * \brief getAnalysis
 * \param HeartRateClassification   char pointer that recieves the classification of Heart Rate
 * \param ActiviyClassification     char pointer that recieves the classitication of Heart Rate
 * \param Abnormal                  bool pointer that recieves true when a abnormality is observed
 * \return                          true if a new analisys was done, false if not.
 */
bool getAnalysis(char * HeartRateClassification,char *ActiviyClassification,bool *Abnormal);

/*!
 * \brief includeInBuffer           Include value in the myBuffer
 * \param data                      myBuffer in which a newValue will be included
 * \param valueToInclude            values to be included
 */
void includeInBuffer(myBuffer *data ,uint32_t valueToInclude);

/*!
 * \brief getLastIncludedInBuffer   return last value of a myBuffer variable
 * \param data                      myBuffer in which the last value will be returned
 * \return                          last value of myBuffer
 */
uint32_t getLastIncludedInBuffer(myBuffer *data);

/*!
 * \brief includeInBuffer_          Include value in the myBuffer_
 * \param data                      myBuffer_ in which a newValue will be included
 * \param valueToInclude            values to be included
 */
void includeInBuffer_(myBuffer_ *data ,bool valueToInclude);

/*!
 * \brief isAbnormal                check the myBuffer_ to verify with the HR is abnormal related with activity
 * \param data                      myBuffer_ to be analysed
 * \return                          true if all value of the array are abnormal
 */
bool isAbnormal(myBuffer_ *data);
#ifdef __cplusplus
}
#endif

#endif /*INTERFACE_ANALYSIS_PINCINATO_H*/
