#include "interface_SDCARD_pincinato.h"
#include <string.h>

//Table *localTableAnalysis=NULL;

bool initSDCARDInterface(void){

    configPins();
    startUpSDCard();
    return initSDCard();

}

void configPins(void){

    
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15; //MISO and MOSI
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void startUpSDCard(){

    uint8_t Command[7]="";
		clearACKbuffer(&Command[0],6);
    HAL_GPIO_WritePin(SPI2_CS_GPIO_Port,SPI2_CS_Pin,GPIO_PIN_SET);
    for(int i=0;i<80;++i){
        HAL_SPI_Transmit(&SDCARDSPI,&Command[0],1,100);
     }
}


bool initSDCard(void){

	bool ACK=false;
		if(sendCommand0()){
				if(sendCommand(CMD1)){ 
					ACK=true;
				}
    }
    return ACK;
}

bool sendCommand0(void){
	  
		bool ACK=false;
    uint8_t ACKCommand[12]="";
    uint8_t Command[7]="";
		int attempt =0;
    clearACKbuffer(&ACKCommand[0],12);
    HAL_Delay(5);
    HAL_GPIO_WritePin(SPI2_CS_GPIO_Port,SPI2_CS_Pin,GPIO_PIN_RESET);
    HAL_Delay(5);
    Command[0]= CMD0;
    Command[1]=0x00;
    Command[2]=0x00;
    Command[3]=0x00;
    Command[4]=0x00;
    Command[5]=0x95;
    HAL_SPI_Transmit(&SDCARDSPI,&Command[0],6,1000);
    //Wait R1
    ACKCommand[0]=0xFF;
    clearACKbuffer(&Command[0],6);
   while((ACKCommand[0]!=0x01) && (attempt<100)){
        HAL_SPI_TransmitReceive(&SDCARDSPI,&Command[0],&ACKCommand[0],1,1000);
        ++attempt;
     }
	 if(ACKCommand[0]==0x01) {ACK=true;}
	return ACK;
}

bool sendCommand(uint8_t cmd){
		bool ACK=false;
		int attempt=0;
    uint8_t ACKCommand[31]="";
    uint8_t Command[31]="";
		clearACKbuffer(&ACKCommand[0],30);
		clearACKbuffer(&Command[0],30);
		while((!ACK) && (attempt<100)){
			Command[0]= cmd;
			Command[1]=0x00;
			Command[2]=0x00;
			Command[3]=0x00;
			Command[4]=0x00;
			Command[5]=0x01;
			HAL_SPI_TransmitReceive(&SDCARDSPI,&Command[0],&ACKCommand[0],30,100);
			++attempt;
			ACK=checkIfACK(&ACKCommand[0], 30,0x00);
    }
		return ACK;
}

void clearACKbuffer(uint8_t * buf, uint8_t size){
    for (int i=0;i< size;++i){
        buf[i]=0xFF;
    }
}


bool checkIfACK(uint8_t * buf, uint8_t size, uint8_t msgACK){
    bool ACK=false;
    for(int i=0;i<size;++i){
        if(buf[i]==msgACK){
            ACK=true;
            return ACK;
        }
    }
    return ACK;
}


bool SDCardWriteBytes(uint8_t * toWrite, uint16_t size, uint32_t startPosition){
	
    bool ACK=false;
    bool sdCardBusy=false;
    uint16_t dataWritten=0;
    if(size<100){
        uint8_t ACKCommand[11]="";
        uint8_t Command[11]="";
        uint8_t Msg[101]="";
        clearACKbuffer(&Command[0],10);
        clearACKbuffer(&ACKCommand[0],10);
        Msg[0]= TOKEN_ACK;
				//Msg[1]= SD_DATA_BEGIN;
        strcat((char *)Msg,(char *)toWrite);
        Command[0]=CMD24;
        Command[1]=0x00;
        Command[2]=0x00;
        Command[3]=0x00;
        Command[4]=startPosition;
        Command[5]=0x01;
        HAL_SPI_TransmitReceive(&SDCARDSPI,&Command[0],&ACKCommand[0],10,100);
        if(checkIfACK(ACKCommand,10,0x00)){
            HAL_SPI_Transmit(&SDCARDSPI,&Msg[0],size+1,100);
            ACKCommand[0]=0xFF;
            Command[0]=0xFF;
            for(int i=0;i<5000;++i){
                HAL_SPI_TransmitReceive(&SDCARDSPI,&Command[0],&ACKCommand[0],1,100);
                if(ACKCommand[0]==0x00){ //start
                    sdCardBusy=true;
                    ++dataWritten;
                }
                if((ACKCommand[0]==0xFF) && (sdCardBusy)){ //finish
                    break;
                }
            }
            if(dataWritten>=size) {ACK=true;}
        }
    }
    return ACK;

}

bool SDCardReadBytes(uint8_t * toRead, uint16_t size, uint32_t startPosition){

    bool ACK=false;
    bool getValues=false;
    uint8_t ACKCommand[11]="";
    uint8_t Command[11]="";
    uint32_t index=0;
    clearACKbuffer(&Command[0],10);
    clearACKbuffer(&ACKCommand[0],10);
    //Read block cMD17 + adr + Crc OK - Working
    Command[0]=CMD17;
    Command[1]=0x00;
    Command[2]=0x00;
    Command[3]=0x00;
    Command[4]=startPosition;
    Command[5]=0x01;
    HAL_SPI_TransmitReceive(&SDCARDSPI,&Command[0],&ACKCommand[0],10,100);
		if(checkIfACK(&ACKCommand[0],10,0x00)){
			Command[0]=0xFF;
	    for(int i=0;i<1000;++i){
        HAL_SPI_TransmitReceive(&SDCARDSPI,&Command[0],&ACKCommand[0],1,100);
        if(getValues){
            if(index<size){
                toRead[index]=ACKCommand[0];
                index++;
            }
        }
        else if(ACKCommand[0] == TOKEN_ACK){ //|| (ACKCommand[0]==SD_DATA_BEGIN)){
            getValues=true;
        }
			}
			if((index<=size)&&(getValues)){ACK=true;}
		}
    return ACK;
}

/****** END OF FILE ******/
