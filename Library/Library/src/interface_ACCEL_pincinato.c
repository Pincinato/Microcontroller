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
//#include "lcd_pincinato.h"

void initACCELInterface(void){
    uint8_t accelSetupData[4]={STATUSREGISTER,0,STATUSREGISTER,1};
    HAL_I2C_Master_Transmit(&ACCELEROMETER, acellSlaveAdress,&accelSetupData[0],2,10);
    HAL_I2C_Master_Transmit(&ACCELEROMETER, acellSlaveAdress,&accelSetupData[2],2,10);
    clearData();
    //MX_TIM3_Init();
}

void startACCELInterface(void){
    HAL_TIM_Base_Start_IT(&TIMER_ACCEL);
}

void stopACCELInterface(void){
    HAL_TIM_Base_Stop_IT(&TIMER_ACCEL);
}

float getACCELX(void){
    return AccelProcess.Accelerometer_X;
}

bool getACCELCategory(double* valueDestination){

    double PI = 3.14159;
    double gravity = 9806.65;
    *valueDestination = (gravity*((double)AccelProcess.Temp_Displacement))/(2*PI);
    return true;
}
void clearData(void){

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

void interruptTimerACCELCallback(void){
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
                        }
                        if ((((uint8_t)bufRX[2]>>7) &0x01)==0){
                                value = valueToG(((uint8_t)bufRX[2]<<2)>>2);
                                AccelProcess.Accelerometer_Z=filterAdd(&AccelProcess.Z,value);
                        }
                        AccelProcess.Temp_Displacement=filterAdd(&AccelProcess.SquareSumX2Y2Z2,sqrt(AccelProcess.Accelerometer_Z*AccelProcess.Accelerometer_Z+AccelProcess.Accelerometer_Y*AccelProcess.Accelerometer_Y+AccelProcess.Accelerometer_X*AccelProcess.Accelerometer_X));
                }
        }
}
/****** END OF FILE ******/
