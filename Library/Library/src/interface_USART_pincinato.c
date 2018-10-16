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
#include "interface_USART_pincinato.h"
#include "string.h"

#define ERRORR_NO_OPTION 10 // option choose not avalaible
#define ERRORR_BUFFER 20 //buffer does not contain a valid data
#define ERRORR_TABLE 30 // table is NULL
#define OPTION_HIGH "Hig"
#define OPTION_NORMAL "Nor"
#define OPTION_LOW "Low"
#define OPTION_ACTUAL_CONFIGURATION "Act"
#define OPTION_INFO_ANOMALY "Anm"

Table *localTable=NULL;
UART_HandleTypeDef *localUart=NULL;
char msgToSend[30]="";
char msgReceived[10]="";
bool receiverStart=false;
short receiverCount=0;
	
bool initInterface(Table *A, UART_HandleTypeDef *huart){

	setUart(huart);
	setTable(A);
	return (checkTable() && checkUart());
}


void setUart(UART_HandleTypeDef *huart){
				
	localUart=huart;
}

void setTable(Table * A){
				
	localTable=A;
}

void check_RX(void){
	
  HAL_StatusTypeDef RxResult;
	char rxBuf[3]="";
	RxResult=HAL_UART_Receive(&huart2,(uint8_t*) rxBuf,1,100);
  if (!RxResult){
      rxBuf[1]=0x00;
			if(rxBuf[0]=='?'){receiverStart=true;}
			if(receiverStart && (rxBuf[0]!='?')){		
				if(rxBuf[0]=='!'){
					receiverStart=false;
					msgReceived[receiverCount]=0;
					selectOption(msgReceived);
					clearMsg(msgReceived);
					receiverCount=0;
				}
				else {
					msgReceived[receiverCount]=rxBuf[0];
					receiverCount++;
					if(receiverCount>9){
						receiverCount=0;
					  clearMsg(msgReceived);
						receiverStart=false;
					}
				}
		}
	}
}

void selectOption(const char * buf){
	
	bool ack = true;
	char opt[4]="";
	uint32_t intValue=0;
	if(checkTable()){
		if(!strcmp(buf,OPTION_ACTUAL_CONFIGURATION)){
				sendActualConfig();
			}
		else if (!strcmp(buf,OPTION_INFO_ANOMALY)){
				sendHeartExercise("1234","4321");//only a test
			}
		else {
			if(getOptionAndValue(buf,opt,&intValue)){
				if(!strcmp(opt,OPTION_HIGH)){
					setHigh(intValue);
				}
				else if(!strcmp(opt,OPTION_NORMAL)){
					setNormal(intValue);
				}
				else if(!strcmp(opt,OPTION_LOW)){
					setLow(intValue);
				}
				else {
					ack=false;
				}
				if(ack){ ACK();}
				else{	NACK(ERRORR_NO_OPTION);}
			}
			else{NACK(ERRORR_BUFFER);}
		}
	}
	else{NACK(ERRORR_TABLE);}
}

void setHigh(uint32_t value){

		localTable->High_HeartRate=value;
		localTable->HasBeenEdited=true;
}
void setNormal(uint32_t value){
	
		localTable->Normal_HeartRate=value;
		localTable->HasBeenEdited=true;
}

void setLow(uint32_t value){
	
		localTable->Low_HeartRate=value;
		localTable->HasBeenEdited=true;
}

void sendActualConfig(void) {
	
	
	char Low[4]="";
	char Normal[4]="";
	char High[4]="";
	char Edited[2]="";
	clearMsg(msgToSend);
	if(checkUart()){
		if(localTable->HasBeenEdited){Edited[0]=0x31;}
		else{Edited[0]=0x30;}
		sprintf(Low, "%d", localTable->Low_HeartRate);
		sprintf(Normal, "%d", localTable->Normal_HeartRate);
		sprintf(High, "%d", localTable->High_HeartRate);
		strcat(msgToSend,"Low");
		strcat(msgToSend,Low);
		strcat(msgToSend,"Normal");
		strcat(msgToSend,Normal);
		strcat(msgToSend,"High");
		strcat(msgToSend,High);
		strcat(msgToSend,"Ed");	
		strcat(msgToSend,Edited);
		strcat(msgToSend,"! ");			
		HAL_UART_Transmit_DMA(localUart,(uint8_t*) msgToSend,strlen((const char *) &msgToSend));
	}
}


void sendHeartExercise(const char * heartRate, const char * typeExercise){

	clearMsg(msgToSend);
	if(checkUart()){
		strcat(msgToSend,"HR ");
		strcat(msgToSend,heartRate);
		strcat(msgToSend,"TE ");
		strcat(msgToSend,typeExercise);
		strcat(msgToSend,"!\n");
		HAL_UART_Transmit_DMA(localUart,(uint8_t*) msgToSend,strlen((const char *) &msgToSend));
	}

}

void ACK(void){
	
	clearMsg(msgToSend);
	if(checkUart()){
		strcat(msgToSend,"ACK");
		HAL_UART_Transmit_DMA(localUart,(uint8_t*) msgToSend,strlen((const char *) &msgToSend));
	}
	
}

void NACK(uint32_t error){
	
	clearMsg(msgToSend);
	if(checkUart()){
		sprintf(msgToSend, "%d", error);
		HAL_UART_Transmit_DMA(localUart,(uint8_t*) msgToSend,strlen((const char *) &msgToSend));
	}
}

bool getOptionAndValue(const char * buf, char * const opt, uint32_t * intValue){
 
	bool ack=true;
	char value[4]="";
	int numericalValue=0;
	if(strlen(buf)>3){
		for(int i=0; i<3;i++){
			opt[i]=buf[i];
		}
		if (strlen(buf)>=5){
			for(int i=0; i<2;i++){
				value[i]=buf[3+i];
			}
		}
		if (strlen(buf)>=6){
			value[2]=buf[5];
		}
		for(int i=0; i<strlen(value); i++){
			numericalValue = numericalValue * 10 + ( value[i] - '0' );
		}
	(*intValue)=numericalValue;
	}
	else {ack=false;}
	return ack;
}

bool checkUart(void){
		
	if(localUart==NULL){ return false;}
	return true;

}

bool checkTable(void){

	if(localTable==NULL){ return false;}
	return true;
}

void clearMsg(char *toClear){

	for(int i= 0;i< strlen(toClear);i++)
	{
		toClear[i]=0;
	}
}

/****** END OF FILE ******/
