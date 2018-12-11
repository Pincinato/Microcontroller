/******************************************************************************
* @file     filter_math_pincinato.h
* @brief    This file handles filter and DSP operations
* @author   Thiago Henrique Pincinato
* @version  V1.0
* @date     10.12.2012
* @hardware no specific hardware
******************************************************************************/
#ifndef FILTER_MATH_PINCINATO_H
#define FILTER_MATH_PINCINATO_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/*
 * @brief Struct for Biquads filter
 */
typedef struct BiquadsFilter_{
   float a0,a1,a2;
   float b1,b2;
   uint32_t n_1;
   uint32_t n_2;
   float dataN_1;
   float dataN_2;
   int index;
   int sizeBuffer;
   float *data;
} BiquadsFilter;

 /*
  * @brief Struct for Average filter
  */
typedef struct AverageFilter_{
   float sum;
   int index;
   int sizeBuffer;
   float * data;
 } AverageFilter;

 /*
  * @brief Struct for Integartion
  */

typedef struct _IntegratalData_{
   int index;
   float n_1;
   float sum;
   float t;
   int sizeBuffer;
   float *data;
 } IntegrationData;

 
 /*!
 * \brief normalize     Normalize a arry of float value
 * \param vec           float * to an array, which contains the value to be normalized
 *                      and after function call, will have the normalized array
 * \param size          int that indicates the size of the array
 */
void normalize(float *vec,int size);

/*!
 * \brief initBiquadsFilter         init filter parameters and pointer to respective data
 * \param filter                    BiquadsFilter pointer to the filter that will be initialized
 * \param d_data                    float * to the buffer that contains the filter values
 * \param sizeBuffer                size of the d_data
 * \param Acoef                     A Coefficient of the filter
 * \param Bcoef                     B Coefficient of the filter
 */
void initBiquadsFilter(BiquadsFilter * filter,float *d_data,int sizeBuffer,float Acoef[],float Bcoef[]);

/*!
 * \brief filterBiquadsCompute      Include the value in the filter and compute the filter operation
 * \param filter                    Biquads filter pointer in which the operation will take place.
 * \param value                     uin32_t value to be add in the filter
 * \return                          float with the last result of the filter
 */
float filterBiquadsCompute(BiquadsFilter *filter, uint32_t value);

/*!
 * \brief clearBiquadsFilter       Set all entry to zero
 * \param filter                   Biquads filter that will be cleared
 */
void clearBiquadsFilter(BiquadsFilter * filter);

/*!
 * \brief initAverageFilter         init filter parameters and pointer to respective data
 * \param filter                    pointer to the filter that will be initialized
 * \param d_data                    float * to the buffer that contains the filter values
 * \param sizeBuffer                size of the d_data
 */
void initAverageFilter(AverageFilter * filter,float *d_data,int sizeBuffer);

/*!
 * \brief clearAverageFilter        Set all entry to init value
 * \param filter                    filter to be cleared
 * \param initalValue               float value that will be used to initialize
 */
void clearAverageFilter (AverageFilter *filter,float initalValue);

/*!
 * \brief filterAverageCompute      include the value in the filter and compute the average
 * \param filter                    filter to be computed
 * \param value                     float value to be add in the filter
 * \return                          float with last value of the filter
 */
float filterAverageCompute(AverageFilter *filter, float value);

/*!
 * \brief integrationRelativeDistance   Integrates values, regardingless the signal.No negative values in the integration.
 * \param integral                      Integration data to be computed
 * \param n                             float value to be add in the integration
 * \return                              float value of the last relative integration
 */
float integrationRelativeDistance(IntegrationData *integral, float n);

/*!
 * \brief integrationCompute            include the value in the integration and compute the integration
 * \param integral                      IntegartionData to be computed
 * \param n                             float value to be add in the integration
 * \return                              float value of the last integration
 */
float integrationCompute(IntegrationData *integral, float n);

/*!
 * \brief initIntegration               initializes the IntegrationData parameters and pointer to respective data
 * \param integration                   IntegrationData pointer to the filter that will be initialized
 * \param d_data                        float * to the buffer that contains the filter values
 * \param sizeBuffer                    size of the d_data
 * \param time                          sets time of integration
 */
void initIntegration(IntegrationData * integration,float *d_data,int sizeBuffer,float time);

/*!
 * \brief clearIntegration          Set all entry to zero
 * \param integration               IntegrationData that will be cleared
 */
void clearIntegration(IntegrationData * integration);

#ifdef __cplusplus
}
#endif

#endif /*FILTER_MATH_PINCINATO_H*/
