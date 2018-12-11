/******************************************************************************
* @file     sdcard.h
* @brief    This file will handle the connected sd card
* @author   BFH-TI
* @version  V1.1
* @date     7.5.2012
* @hardware MCBSTM32C & STM32-P103         
******************************************************************************/

/* Function prototypes ------------------------------------------------------*/
void SD_Init(void);
int SD_Start(char file_name[], unsigned char Flags);
void SD_Write(char file_data[],int length);
void SD_Close(void);
void SD_Read(char *file_data, int length);
volatile void timer_proc(void);
