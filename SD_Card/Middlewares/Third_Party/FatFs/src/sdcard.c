/******************************************************************************
* @file     sdcard.c
* @brief    This file will handle the connected sd card
* @author   BFH-TI
* @version  V1.1
* @date     21.5.2012
* @hardware STM32-P103 & MCBSTM32C & STM3210C         
******************************************************************************/

/* Defines ------------------------------------------------------------------*/
//#define SPI_CS_LOW()        GPIO_ResetBits(GPIOB, GPIO_Pin_0)
//#define SPI_CS_HIGH()       GPIO_SetBits(GPIOB, GPIO_Pin_0);

/* Includes -----------------------------------------------------------------*/
#include "Platform.h"
#include "spi.h"
#include "stm32f4xx.h"
#include <stdio.h>
#include "ff.h"
#include "diskio.h"

/* Variables ----------------------------------------------------------------*/
volatile int ret;
FATFS fs = {0};
FIL file;
unsigned int written=0;
int size = 0;

/*******************************************************************************
* Function Name  : SD_Init
* Description    : Initialisation vor SD Card
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SD_Init(void)
{
	/* variables */
	HAL_SPI_Init(&hspi2);	  

	// Enable SPI
	SPI_Cmd(SD_SPI, ENABLE);
}

/*******************************************************************************
* Function Name  : SD_Start
* Description    : Start a Writing Session 
* Input          : File Name, Flags
* Output         : None
* Return         : "1" for Success, "0" for Failure
*******************************************************************************/
int SD_Start(char file_name[], unsigned char Flags)
{
	/* Start Card Service */
	disk_timerproc();
	ret = disk_initialize(fs.drive);
	ret = f_mount(0,&fs);
	ret = f_open(&file, (const char*) file_name, Flags);
 	if( ret == 0 )
	{
		return 1;
	}
	return 0;
}

/*******************************************************************************
* Function Name  : SD_Write
* Description    : Write a String to the SD Card 
* Input          : String, length of String
* Output         : None
* Return         : None
*******************************************************************************/
void SD_Write(char file_data[], int length)
{
	/* write to SD card */
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	ret = f_write(&file,( const BYTE*) file_data, length, &written);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

/*******************************************************************************
* Function Name  : SD_Close
* Description    : Close a Writing Session 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SD_Close(void)
{
	/* close card and umount */
	f_close(&file); // close the file
	ret = f_mount(0,NULL);	// unmount
}

/*******************************************************************************
* Function Name  : SD_Read
* Description    : Read from SD 
* Input          : * String, length
* Output         : None
* Return         : None
*******************************************************************************/
void SD_Read(char *file_data, int length)
{
	unsigned int number;
	f_read(&file,file_data,length,&number);

}

/*******************************************************************************
* Function Name  : timer_proc
* Description    : timer for the SPI 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
volatile void timer_proc(void)
{
	/* call timerproc for timesteps in SD card handeling */
	//GPIO_SetBits(GPIOC, GPIO_Pin_4);
	disk_timerproc();
	//GPIO_ResetBits(GPIOC, GPIO_Pin_4);
}
