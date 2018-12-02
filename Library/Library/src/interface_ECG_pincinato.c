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
#include "interface_ECG_pincinato.h"
#include "interface_ADC_pincinato.h"

void initECGInterface(void){
    initAverageFilter(&ECGprocess.HR,&ECGprocess.HRbuf[0],lowFilterItemCount);
    clearAverageFilter(&ECGprocess.HR,60);
    ECGprocess.HR_threshold=0.75;
    initADCInterface();

}
void startECGInterface(void){
    startADCInterface();

}
void stopECGInterface(void){
    stopADCInterface();
}

float getHeartRate(){
	
	static float ret;
	int heat_beats=0;
	int fs=200;
	int last_detected=-40;
  float localThreshold=ECGprocess.HR_threshold;
	removeADCUnstableValues();
	normalizeADCData();	
	//Detect heart bit
	while((heat_beats<3) & (localThreshold> (float)0.55)){ //Adaptive threshold at lower 0.55
		for(int i=0;i<getADCDataSize();i++){ //interval in which the max heart freq is contained -> avoid count of two peaks too close.
				if((getADCData()[i]>localThreshold)& (i> last_detected+39)){
					 last_detected=i;
					 heat_beats++;
			}
		}
		if(heat_beats<3){
			localThreshold=localThreshold - (float)0.05; //decrement of 0.05
		}
	}
	//Compute Heart rate
	ret=getADCDataSize();
	ret=ret/fs; // sec
	ret= (float)heat_beats/ret; //bps
	ret =ret*60;// bpm
	return ret; //qty of bit / time of measure *60 - bpm

}

bool updateHeartRate(float* destinationValue){
	bool ack=false;
	if (getReadyToUpdate()){
    *destinationValue=filterAverageCompute(&ECGprocess.HR,getHeartRate());
		setReadyToUpdate(false);
		ack=true;		
		startADCInterface();
	}
	return ack;
}

/****** END OF FILE ******/
