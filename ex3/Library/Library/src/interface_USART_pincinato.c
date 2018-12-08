/***************************************************************************************************
  * @file				interface_USART_pincinto.c
  * @author     Thiago Henrique Pincinato
  * @version    V0.4
	* @date       30.09.2018
	* @date       20.04.2018
  * @brief      Uart reciever via DMA (circular)
  * @hardware   NucleoF446RE with mbed-016.1-application-shield
  **************************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2018 BFH-TI </center></h2>
  *
  */
#include "interface_USART_pincinato.h"
#include "string.h"
#include "usart.h"

#define ERRORR_NO_OPTION 10 // option choose not avalaible
#define OPTION_LEDREDON "Red1" //
#define OPTION_LEDREDOFF "Red0" //
#define OPTION_LEDGREENON "Gre1" //
#define OPTION_LEDGREENOFF "Gre0" //
#define OPTION_LEDBLUEON "Blu1" //
#define OPTION_LEDBLUEOFF "Blu0" //

UART_HandleTypeDef *localUart=NULL;
char msgToSend[30]="";
char msgReceived[10]="";
char lastMsg[10]="";
char bufMsg[8]="";
bool receiverStart=false;
short receiverCount=0;

bool initInterface(UART_HandleTypeDef *huart){
		
  MX_USART2_UART_Init();
  MX_DMA_Init();	
	setUart(huart);
	HAL_UART_Receive_DMA(&huart2,(uint8_t*) bufMsg,6);//circular
	receiverCount=0;
	return checkUart();
}


void setUart(UART_HandleTypeDef *huart){
				
	localUart=huart;
}

void check_RX(void){
	
	bool newMsg=false;
	int index=1;
	if(bufMsg[0]=='?'){//ex ?Act!
		while((bufMsg[index]!='!')&&(index<10)){
			msgReceived[index-1]=bufMsg[index];
			++index;
		}
		for(int i = 0; i<5;i++){
			if(lastMsg[i]!=msgReceived[i]){newMsg=true;}
		}
		if(newMsg==true){
			selectOption(&msgReceived[0]);
			for(int i = 0; i<5;i++){
				lastMsg[i]=msgReceived[i];
			}
		}
	}
	clearMsg(msgReceived,10);
}

void selectOption(const char * buf){
	
	bool ack = true;
	if(!strcmp(buf,OPTION_LEDREDON)){
				HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_RESET);
		}
		else if (!strcmp(buf,OPTION_LEDREDOFF)){
				HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET);
		}
		else if(!strcmp(buf,OPTION_LEDGREENON)){
				HAL_GPIO_WritePin(LedGreen_GPIO_Port,LedGreen_Pin,GPIO_PIN_RESET);
		}
		else if(!strcmp(buf,OPTION_LEDGREENOFF)){
				HAL_GPIO_WritePin(LedGreen_GPIO_Port,LedGreen_Pin,GPIO_PIN_SET);
		}
		else if(!strcmp(buf,OPTION_LEDBLUEON)){
			HAL_GPIO_WritePin(LedBlue_GPIO_Port,LedBlue_Pin,GPIO_PIN_RESET);
		}
		else if(!strcmp(buf,OPTION_LEDBLUEOFF)){
			HAL_GPIO_WritePin(LedBlue_GPIO_Port,LedBlue_Pin,GPIO_PIN_SET);
		}
		else {
					ack=false;
				}
	if(ack){ ACK();}
	else{	NACK(ERRORR_NO_OPTION);}
}


void ACK(void){
	
	clearMsg(msgToSend,30);
	if(checkUart()){
		strcat(msgToSend,"ACK");
		HAL_UART_Transmit_DMA(localUart,(uint8_t*) msgToSend,strlen((const char *) &msgToSend));
	}
	
}

void NACK(uint32_t error){
	
	clearMsg(msgToSend,30);
	if(checkUart()){
		sprintf(msgToSend, "%d", error);
		HAL_UART_Transmit_DMA(localUart,(uint8_t*) msgToSend,strlen((const char *) &msgToSend));
	}
}


bool checkUart(void){
		
	if(localUart==NULL){ return false;}
	return true;

}
void clearMsg(char *toClear,int size){

	for(int i= 0;i< size;i++)
	{
		toClear[i]=0;
	}
}


/****** END OF FILE ******/
