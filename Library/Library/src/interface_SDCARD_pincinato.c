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
#include "interface_SDCARD_pincinato.h"


//Table *localTableAnalysis=NULL;

bool initSDCARDInterface(){

    configPins();
    startUpSDCard();
    return initSDCard();

}

void configPins(void){

		GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = GPIO_PIN_14; //MISO
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void startUpSDCard(){

    uint8_t Command[7];
    Command[0]=0xFF;
    Command[1]= 0xFF;
    HAL_GPIO_WritePin(SPI2_CS_GPIO_Port,SPI2_CS_Pin,GPIO_PIN_SET);
    for(int i=0;i<100;++i){
        HAL_SPI_Transmit(&SDCARDSPI,&Command[0],1,100);
     }
}


bool initSDCard(void){

    bool ACK=false;
    uint8_t ACKCommand[12]="";
    uint8_t Command[7];
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
    Command[0]=0xFF;
    Command[1]= 0xFF;
    for(int i=0;i<10;++i){
        HAL_SPI_Transmit(&SDCARDSPI,&Command[0],1,100);
        HAL_SPI_Receive(&SDCARDSPI,&ACKCommand[i],1,1000);
     }
    if(checkIfACK(ACKCommand,10,0x01)){
        for(int i=0;i<3;++i){
            Command[0]= CMD1;
            Command[1]=0x00;
            Command[2]=0x00;
            Command[3]=0x00;
            Command[4]=0x00;
            Command[5]=0x01;
            HAL_SPI_Transmit(&hspi2,&Command[0],6,100);
            for(int j=0;j<2000;++j){
                Command[0]= 0xff;
                HAL_SPI_Transmit(&SDCARDSPI,&Command[0],1,100);
                HAL_SPI_Receive(&SDCARDSPI,&ACKCommand[0],1,100);
                if(ACKCommand[0]==0x00){
                    j=5000;
                    i=5;
                    ACK=true;
                    break;
                }
            }
        }
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
    for(int i=0;i>size;++i){
        if(buf[i]==msgACK){
            ACK=true;
            return ACK;
        }
    }
    return ACK;
}


bool write3Bytes(uint8_t first, uint8_t second, uint8_t third){
    bool ACK=false;
    return ACK;

}

bool read3Bytes(uint8_t *first, uint8_t *second, uint8_t *third){
    bool ACK=false;
    return ACK;

}
/****** END OF FILE ******/