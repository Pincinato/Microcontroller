/***************************************************************************************************
  * @file
  * @author     ?? original author has to be given as well !!	
  * @version    V0.4
	* @date       30.09.2018
	* @date       20.04.2018
	* @date       25.11.2017
  * @date       28.10.2015
  * @brief      Display setup and communication for LCD on mbed16-application-shield
	* @brief      V0.4;dnd1;add;drawLine() function
  * @hardware   NucleoF446RE with mbed-016.1-application-shield
  **************************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2018 BFH-TI </center></h2>
  *
  */
#include "structs_pincinato.h"
#include "interface_SDCARD_pincinato.h"


void initTable(Table *A){

	A->High_HeartRate=100; // initial High value
	A->Normal_HeartRate=80; // initial normal value
	A->Low_HeartRate=60; // initial low level
	A->sdCardStatus=false;
}


bool loadTableValues(Table *A){
	
	bool ACK=true;
	static uint8_t dataRead[4]="";
	if(SDCardReadBytes(&dataRead[0],3, 0x90)){
			if((dataRead[0]>=40) && (dataRead[0]<=100)){
					A->Low_HeartRate= (uint32_t)dataRead[0];
			}
			else {ACK=false;}
			if((dataRead[1]>=50) && (dataRead[1]<=120)){
					A->Normal_HeartRate= (uint32_t)dataRead[1];
			}
			else {ACK=false;}
			if((dataRead[2]>=80) && (dataRead[2]<=250)){
					A->High_HeartRate= (uint32_t)dataRead[2];
			}
			else {ACK=false;}
	}
	else {
		ACK=false;
	}
	return ACK;
}

bool saveTableValues(Table *A){
	uint8_t dataWrite[4]={(uint8_t)A->Low_HeartRate,(uint8_t)A->Normal_HeartRate,(uint8_t)A->High_HeartRate,0x00};
	return SDCardWriteBytes(&dataWrite[0],3,0x90);
		
}

/****** END OF FILE ******/
