#ifndef FILTER_MATH_PINCINATO_H
#define FILTER_MATH_PINCINATO_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>


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

typedef struct AverageFilter_{
   float sum;
   int index;
   int sizeBuffer;
   float * data;
 } AverageFilter;

 
typedef struct _IntegratalData_{
   int index;
   float n_1;
   float sum;
   float t;
   int sizeBuffer;
   float *data;
 } IntegrationData;

 
 void normalize(float *vec,int size);
void initBiquadsFilter(BiquadsFilter * filter,float *d_data,int sizeBuffer,float Acoef[],float Bcoef[]);
float filterBiquadsCompute(BiquadsFilter *filter, uint32_t value);
void clearBiquadsFilter(BiquadsFilter * filter);
void initAverageFilter(AverageFilter * filter,float *d_data,int sizeBuffer);
void clearAverageFilter (AverageFilter *filter,float initalValue);
float filterAverageCompute(AverageFilter *filter, float value);
float integrationRelativeDistance(IntegrationData *integral, float n);
float integrationCompute(IntegrationData *integral, float n);
void initIntegration(IntegrationData * integration,float *d_data,int sizeBuffer,float time);
void clearIntegration(IntegrationData * integration);

#ifdef __cplusplus
}
#endif

#endif /*FILTER_MATH_PINCINATO_H*/
