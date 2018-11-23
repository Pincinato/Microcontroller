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
#include "interface_ACCEL_pincinato.h"
#include "string.h"
#include "lcd_pincinato.h"

void initACCELInterface(void){
    uint8_t accelSetupData[4]={STATUSREGISTER,0,STATUSREGISTER,1};
    HAL_I2C_Master_Transmit(&ACCELEROMETER, acellSlaveAdress,&accelSetupData[0],2,10);
    HAL_I2C_Master_Transmit(&ACCELEROMETER, acellSlaveAdress,&accelSetupData[2],2,10);
    clearData();
    //MX_TIM3_Init();
}

void startACCELInterface(void){
    HAL_TIM_Base_Start_IT(&TIMER_ACCEL);
    AccelProcess.calibrationY=0;
    while(!calibrate()){
        readAccel();
    }
}

void stopACCELInterface(void){
    HAL_TIM_Base_Stop_IT(&TIMER_ACCEL);
}

bool calibrate(void){
    bool ACK=false;
    if(count_interrupt>=10){
        AccelProcess.calibrationY=AccelProcess.Accelerometer_Y;
        clearIntegration();
        ACK=true;
    }
    return ACK;
}
float getACCELX(void){
    return AccelProcess.Accelerometer_X;
}

bool getDistance(double* valueDestination){
    bool ACK=false;
		int timeAnalisys=3;//seconds;
    if(count_interrupt>=timeAnalisys){
        *valueDestination=AccelProcess.DistanceY.sum;
        clearIntegration();
        ACK=true;
    }
    return ACK;
}

bool getACCELCategory(accel_Category* valueDestination){
    bool ACK=false;
    double distance;
    int low =3;
    int medium=6;
    if(getDistance(&distance)){
        if(distance<=low){*valueDestination=Category_LOW; }
        else if(distance<=medium){*valueDestination=Category_MEDIUM;}
        else if(distance>=medium){*valueDestination=Category_HIGH;}
        ACK=true;
    }
    return ACK;
}

void clearFilter(void){

    readValid=false;
    AccelProcess.Accelerometer_X=0;
    AccelProcess.Accelerometer_Y=0;
    AccelProcess.Accelerometer_Z=0;
    AccelProcess.X.sum=0;
    AccelProcess.Y.sum=0;
    AccelProcess.Z.sum=0;
    AccelProcess.X.index=0;
    AccelProcess.Y.index=0;
    AccelProcess.Z.index=0;
    for(uint8_t i=0;i<filterItemCount;i++){
       AccelProcess.X.data[i]=0;
       AccelProcess.Y.data[i]=0;
       AccelProcess.Z.data[i]=0;
    }
}


void clearIntegration(void){

    count_interrupt=0;
    AccelProcess.VelocityY.index=0;
    AccelProcess.VelocityY.n_1=0;
    AccelProcess.VelocityY.sum=0;
    AccelProcess.DistanceY.index=0;
    AccelProcess.DistanceY.n_1=0;
    AccelProcess.DistanceY.sum=0;
    for(uint8_t i=0;i<integrationLength;i++){
       AccelProcess.VelocityY.data[i]=0;
       AccelProcess.DistanceY.data[i]=0;
    }
}

void clearData(void){

    clearIntegration();
    clearFilter();
}

float valueToG(uint8_t value){
        float returnValue;
        returnValue= ((float)value)/21.33F;
        if(value>31){
                value= (((~value)+1) & 0x1f) ;
                returnValue= (-(float)value)/21.33F;
        }
        return returnValue;
}

float filterAdd(FilterData *filter, float value){

        filter->sum -= filter->data[filter->index]; // throw away last value in ring
        filter->sum += value;                   // add new value to the sum of all values
        filter->data[filter->index++] = value;    // store current value and increment list index
        if (filter->index >= filterItemCount) { // wrap around after last index
           filter->index = 0;
        }
  return filter->sum / filterItemCount;	  // return average value
}

float integrate(IntegrationData *integral, float n){

  float t=0.1;
  float integration= integral->n_1*t + ((n-integral->n_1)*t)/2 ;
  integral->sum += integration;
  integral->data[integral->index++] = integration;    // store current value and increment list index
  if (integral->index >= integrationLength) { // wrap around after last index
    integral->index = 0;
  }
  integral->n_1=n;
  return integral->sum;	  // return average value
}

float integrateDistance(IntegrationData *integral, float n){

  float t=0.1;
  float integration= integral->n_1*t + ((n-integral->n_1)*t)/2 ;
  if(integration<0){
      integration=integration*(-1);
  }
  integral->sum += integration;
  integral->data[integral->index++] = integration;    // store current value and increment list index
  if (integral->index >= integrationLength) { // wrap around after last index
    integral->index = 0;
    __HAL_GPIO_EXTI_GENERATE_SWIT(GPIO_PIN_0); //Signal to indicates end of a buffer
  }
  integral->n_1=n;
  return integral->sum;	  // return average value
}

bool readAccel(void){
	
	bool ret = readValid;
	if(readValid==true){
        float value;
        uint8_t bufRX[4]="";
        HAL_StatusTypeDef i2cstatus =HAL_I2C_Master_Transmit(&ACCELEROMETER, acellSlaveAdress,(uint8_t *)accelXoutRegister,1,10);
       	if( i2cstatus	 == HAL_OK){
                i2cstatus =HAL_I2C_Master_Receive(&ACCELEROMETER, acellSlaveAdress,(uint8_t *)&bufRX[0],3,100);
                if( i2cstatus	 == HAL_OK){
                        if ((((uint8_t)bufRX[0]>>7) &0x01)==0){
                                value = valueToG(((uint8_t)bufRX[0]<<2)>>2);
                                AccelProcess.Accelerometer_X=filterAdd(&AccelProcess.X,value);
                        }
                        if ((((uint8_t)bufRX[1]>>7) &0x01)==0){
                                value = valueToG(((uint8_t)bufRX[1]<<2)>>2);
                                AccelProcess.Accelerometer_Y=filterAdd(&AccelProcess.Y,value);
                                AccelProcess.Accelerometer_Y-=AccelProcess.calibrationY;
                        }
                        if ((((uint8_t)bufRX[2]>>7) &0x01)==0){
                                value = valueToG(((uint8_t)bufRX[2]<<2)>>2);
                                AccelProcess.Accelerometer_Z=filterAdd(&AccelProcess.Z,value);
                        }
                        integrateDistance(&AccelProcess.DistanceY,integrate(&AccelProcess.VelocityY,AccelProcess.Accelerometer_Y));
                }
        }
	readValid=false;
	}
 	return ret;
}


void interruptTimerACCELCallback(void){
        readValid=true;
				TIMER_ACCEL.Instance->SR = (TIMER_ACCEL.Instance->SR & 0xfe) ;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

    if(GPIO_Pin==GPIO_PIN_0){
        count_interrupt++;
    }

}
/****** END OF FILE ******/
