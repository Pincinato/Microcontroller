/******************************************************************************
* @file     interface_ANALYSIS_pincinato.h
* @brief    This file analysis the results of ECG and ACCEL interface
* @author   Thiago Henrique Pincinato
* @version  V1.0
* @date     10.12.2012
* @hardware STM32F446RET6
******************************************************************************/
#ifndef INTERFACE_SDCARD_PINCINATO_H
#define INTERFACE_SDCARD_PINCINATO_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <spi.h>
#include <gpio.h>
#include <stdbool.h>

#define CMD0	(0x40+0)	// GO_IDLE_STATE
#define CMD1	(0x40+1)	// SEND_OP_COND (MMC)
#define ACMD41	(0xC0+41)	// SEND_OP_COND (SDC)
#define CMD8	(0x40+8)	// SEND_IF_COND
#define CMD9	(0x40+9)	// SEND_CSD
#define CMD10	(0x40+10)	// SEND_CID
#define CMD12	(0x40+12)	// STOP_TRANSMISSION
#define ACMD13	(0xC0+13)	// SD_STATUS (SDC)
#define CMD16	(0x40+16)	// SET_BLOCKLEN
#define CMD17	(0x40+17)	// READ_SINGLE_BLOCK
#define CMD18	(0x40+18)	// READ_MULTIPLE_BLOCK
#define CMD23	(0x40+23)	// SET_BLOCK_COUNT (MMC)
#define ACMD23	(0xC0+23)	// SET_WR_BLK_ERASE_COUNT (SDC)
#define CMD24	(0x40+24)	// WRITE_BLOCK
#define CMD25	(0x40+25)	// WRITE_MULTIPLE_BLOCK
#define CMD55	(0x40+55)	// APP_CMD
#define CMD58	(0x40+58)	// READ_OCR

#define SDCARDSPI hspi2

bool initSDCARDInterface(void);
void configPins(void);
void startUpSDCard(void);
bool initSDCard(void);
void clearACKbuffer(uint8_t * buf, uint8_t size);
bool checkIfACK(uint8_t * buf, uint8_t size,uint8_t msgACK);
bool write3Bytes(uint8_t first, uint8_t second, uint8_t third);
bool read3Bytes(uint8_t *first, uint8_t *second, uint8_t *third);

#ifdef __cplusplus
}
#endif

#endif /*INTERFACE_ANALYSIS_PINCINATO_H*/
