/***************************************************************************************************
  * @file
  * @author     ?? original author has to be given as well !!	
  * @version    V0.4
	* @date       30.09.2018
	* @date       20.04.2018
  * @brief      Display setup and communication for LCD on mbed16-application-shield
	* @brief      V0.4;dnd1;add;drawLine() function
  * @hardware   NucleoF446RE with mbed-016.1-application-shield
  **************************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2018 BFH-TI </center></h2>
  *
  */

#include "filter_math_pincinato.h"


void initAverageFilter(AverageFilter * filter,float *d_data,int sizeBuffer){

    filter->data=d_data;
    filter->sizeBuffer=sizeBuffer;
}

void initIntegration(Integration * integration,float *d_data,int sizeBuffer,float time){

    integration->data=d_data;
    integration->sizeBuffer=sizeBuffer;
    integration->t=time;
}

void initBiquadsFilter(BiquadsFilter * filter,float *d_data,int sizeBuffer,float Acoef[],float Bcoef[]){

    filter->data=d_data;
    filter->sizeBuffer=sizeBuffer;
    filter->a0=Acoef[0];
    filter->a1=Acoef[1];
    filter->a2=Acoef[2];
    filter->b1=Bcoef[0];
    filter->b2=Bcoef[1];

}

void clearBiquadsFilter(BiquadsFilter * filter){
		
		filter->index=0;
		filter->n_1=0;
		filter->n_2=0;
		filter->dataN_1=0;
		filter->dataN_2=0;
    for(uint32_t i = 0 ; i<  filter->sizeBuffer; i++){
			filter->data[i]=0;
    }
}

void clearAverageFilter (AverageFilter *filter,float initalValue){
	
          filter->index=0;
          filter->sum=filter->sizeBuffer*initalValue;
          for(uint32_t i = 0 ; i< filter->sizeBuffer; i++){
                filter->data[i]=initalValue;
					}
}

void clearIntegration(Integration * integration){

    integration->index=0;
    integration->n_1=0;
    integration->sum=0;
    integration->t=0;
      for(uint32_t i = 0 ; i< integration->sizeBuffer; i++){
          integration->data[i]=0;
      }
}

float filterAverageCompute(AverageFilter *filter, float value){

        filter->sum -= filter->data[filter->index]; // throw away last value in ring
        filter->sum += value;                   // add new value to the sum of all values
        filter->data[filter->index++] = value;    // store current value and increment list index
        if (filter->index >= filter->sizeBuffer) { // wrap around after last index
           filter->index = 0;
        }
  return filter->sum /filter->sizeBuffer;	  // return average value
}

float filterBiquadsCompute(BiquadsFilter *filter, uint32_t value){

    filter->data[filter->index] = (filter->a0*value) + (filter->a1*filter->n_1) + (filter->a2*filter->n_2) - (filter->b1*filter->dataN_1) - (filter->b2*filter->dataN_2);
    filter->n_2 = filter->n_1;
    filter->n_1 = value;
    filter->dataN_2 = filter->dataN_1;
    filter->dataN_1 = filter->data[filter->index];
    ++filter->index;
    if(filter->index>=filter->sizeBuffer){
        filter->index=0;
    }
    return filter->dataN_1;
}

float integrationCompute(Integration *integral, float n){

  float integration= integral->n_1*integral->t + ((n-integral->n_1)*integral->t)/2 ;
  integral->sum += integration;
  integral->data[integral->index++] = integration;    // store current value and increment list index
  if (integral->index >= integral->sizeBuffer) { // wrap around after last index
    integral->index = 0;
  }
  integral->n_1=n;
  return integral->sum;	  // return average value
}


void normalize(float *vec,int size){

    float max=-9999;
	for(int i=0;i<size;i++){ // find max
		if(vec[i]>max){ max=vec[i];}	
	}
	for(int i=0;i<size;i++){ // normalize
			vec[i]=vec[i]/max;
	}
	
}


/****** END OF FILE ******/
