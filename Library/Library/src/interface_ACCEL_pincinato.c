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
		initIntegration(&AccelProcess.VelocityY,&AccelProcess.DataVelocityY[0],integrationLength,0.01);//fs 100 hz ->ts =0.01
		initIntegration(&AccelProcess.DistanceY,&AccelProcess.DataDistanceY[0],integrationLength,0.01);// at the end to complete itnegration 10 x ts -> 0.1 s of measurement
		initAverageFilter(&AccelProcess.Y,&AccelProcess.DataY[0],filterItemCount);
    uint8_t accelSetupData[4]={STATUSREGISTER,0,STATUSREGISTER,1};
    HAL_I2C_Master_Transmit(&ACCELEROMETER, acellSlaveAdress,&accelSetupData[0],2,10);
    HAL_I2C_Master_Transmit(&ACCELEROMETER, acellSlaveAdress,&accelSetupData[2],2,10);
    clearData();
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
				count_interrupt=0;
        clearIntegration(&AccelProcess.VelocityY);
				clearIntegration(&AccelProcess.DistanceY);
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
				count_interrupt=0;
        clearIntegration(&AccelProcess.VelocityY);
				clearIntegration(&AccelProcess.DistanceY);
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

void clearData(void){
		count_interrupt=0;
    clearIntegration(&AccelProcess.VelocityY);
    clearIntegration(&AccelProcess.DistanceY);
    clearAverageFilter(&AccelProcess.Y,0);
		readValid=false;
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

bool readAccel(void){
	bool ret = readValid;
	if(readValid==true){
        float value;
        uint8_t bufRX[4]="";
        HAL_StatusTypeDef i2cstatus =HAL_I2C_Master_Transmit(&ACCELEROMETER, acellSlaveAdress,(uint8_t *)accelXoutRegister,1,10);
       	if( i2cstatus	 == HAL_OK){
                i2cstatus =HAL_I2C_Master_Receive(&ACCELEROMETER, acellSlaveAdress,(uint8_t *)&bufRX[0],3,100);
                if( i2cstatus	 == HAL_OK){
                        if ((((uint8_t)bufRX[1]>>7) &0x01)==0){
                                value = valueToG(((uint8_t)bufRX[1]<<2)>>2);
                                AccelProcess.Accelerometer_Y=filterAverageCompute(&AccelProcess.Y,value);
                                AccelProcess.Accelerometer_Y-=AccelProcess.calibrationY;
                        }
                        integrationRelativeDistance(&AccelProcess.DistanceY,integrationCompute(&AccelProcess.VelocityY,AccelProcess.Accelerometer_Y));
												if(AccelProcess.DistanceY.index == 0){
													count_interrupt++;; // counts end of a buffer
												}
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
/*
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

    if(GPIO_Pin==GPIO_PIN_0){
        //count_interrupt++;
    }

}*/
/****** END OF FILE ******/
