#include "interface_ANALYSIS_pincinato.h"

Table *localTableAnalysis=NULL;

void initANALYSISInterface(Table *newTable){
	
		localTableAnalysis=newTable;
		clearANALYSISData();
}

void clearANALYSISData(void){    
	
		AnalysisProcess.Activity.index=0;
    for (uint8_t i=0;i<BUFFERSIZEANALYSIS;++i){
        AnalysisProcess.Activity.buf[i]=0;
    }
    AnalysisProcess.Activity.actual=false;
    AnalysisProcess.HRLevel.actual=false;
    AnalysisProcess.HRLevel.index=0;
    for (uint8_t i=0;i<BUFFERSIZEANALYSIS;++i){
        AnalysisProcess.HRLevel.buf[i]=0;
    }
    AnalysisProcess.Abnormalities.index=0;
    for (uint8_t i=0;i<BUFFERSIZEABNORMALITIES;++i){
        AnalysisProcess.Abnormalities.buf[i]=false;
    }
}

void startANALYSISInterface(void){

    startACCELInterface();
    startECGInterface();
		clearANALYSISData();
}

void stopANALYSISInterface(void){
    stopACCELInterface();
    stopECGInterface();
}

bool getTypeOfActivity(void){

    bool ACK=false;
    double distance;
    readAccel();
    if(getDistance(&distance)){
        if((uint32_t)distance <= MEDIUMDISTANCE ){ includeInBuffer(&AnalysisProcess.Activity,LOWACTIVITY);}
        else if (((uint32_t)distance <= LONGDISTANCE) && ((uint32_t)distance> MEDIUMDISTANCE)){includeInBuffer(&AnalysisProcess.Activity,MEDIUMACTIVITY);}
        else if((uint32_t)distance> LONGDISTANCE){ includeInBuffer(&AnalysisProcess.Activity,HIGHACTIVITY);}
        ACK=true;
  }
   return ACK;
}

bool getTypeOfHeartLevel(void){

    bool ACK=false;
    float HeartRate;
    if(updateHeartRate(&HeartRate)){
        if((uint32_t)HeartRate<= localTableAnalysis->Low_HeartRate){ includeInBuffer(&AnalysisProcess.HRLevel,LOWHEARTRATE);}
        else if (((uint32_t)HeartRate<= localTableAnalysis->High_HeartRate) && ((uint32_t)HeartRate>localTableAnalysis->Low_HeartRate)){includeInBuffer(&AnalysisProcess.HRLevel,MEDIUMHEARTRATE);}
        else if((uint32_t)HeartRate>localTableAnalysis->Normal_HeartRate){ includeInBuffer(&AnalysisProcess.HRLevel,HIGHHEARTRATE);}
        ACK=true;
    }
    return ACK;
}

bool getAnalysis(char * HeartRateClassification,char *ActiviyClassification,bool *Abnormal){

    bool ACK= false;
    uint32_t LastHR;
    uint32_t LastActivity;
    getTypeOfHeartLevel();
    getTypeOfActivity();
    if( (AnalysisProcess.HRLevel.actual==true) && (AnalysisProcess.Activity.actual==true)){
        LastActivity=getLastIncludedInBuffer(&AnalysisProcess.Activity);
        LastHR=getLastIncludedInBuffer(&AnalysisProcess.HRLevel);
        switch(LastActivity){
            case LOWACTIVITY:
                sprintf(ActiviyClassification, "Light");
            break;
            case MEDIUMACTIVITY:
                sprintf(ActiviyClassification, "Normal");
            break;
            case HIGHACTIVITY:
                sprintf(ActiviyClassification, "Heavy");
            break;
            default: break;
        }
        switch(LastHR){
            case LOWHEARTRATE:
                sprintf(HeartRateClassification, "Low");
            break;
            case MEDIUMHEARTRATE:
                sprintf(HeartRateClassification, "Normal");
            break;
            case HIGHHEARTRATE:
                sprintf(HeartRateClassification, "High");
            break;
            default: break;
        }
        includeInBuffer_(&AnalysisProcess.Abnormalities,(LastActivity==LastHR));
				*Abnormal=isAbnormal(&AnalysisProcess.Abnormalities);
				AnalysisProcess.HRLevel.actual=false;
				AnalysisProcess.Activity.actual=false;
        ACK=true;
    }
    return ACK;
}

void includeInBuffer(myBuffer *data,uint32_t valueToIncude){

    data->buf[data->index]=valueToIncude;
    data->index++;
    if(data->index>=BUFFERSIZEANALYSIS){
        data->index=0;
    }
    data->actual=true;

}


void includeInBuffer_(myBuffer_ *data ,bool valueToInclude){

    data->buf[data->index]=!valueToInclude;
    data->index++;
    if(data->index>=BUFFERSIZEABNORMALITIES){
        data->index=0;
    }
}


uint32_t getLastIncludedInBuffer(myBuffer *data){

    if(data->index>0){
        return data->buf[data->index-1];
    }
    else{
        return data->buf[BUFFERSIZEANALYSIS-1];
    }


}

bool isAbnormal(myBuffer_ *data){

    bool ACK=true;
    for(uint8_t i=0;i<BUFFERSIZEABNORMALITIES;++i){
        ACK= ACK & data->buf[i];
    }
    return ACK;
}

/****** END OF FILE ******/
