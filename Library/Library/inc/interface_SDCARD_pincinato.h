/******************************************************************************
* @file     interface_SDCARD_pincinato.h
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
#define TOKEN_ACK (0xFE)        // ACK Token for cmd17 and 24
#define SD_DATA_BEGIN 	(0xFC)

#define SDCARDSPI hspi2

 /*!
 * \brief initSDCARDInterface       Configuration of SPI pins, starut up and init of SDcard
 * \return                          true if the initialize was sucesseful, false otherwise
 */
bool initSDCARDInterface(void);

/*!
 * \brief configPins                Assure that Di is used as pullup
 */
void configPins(void);

/*!
 * \brief startUpSDCard             initial procedure to "start up" Sd.
 */
void startUpSDCard(void);

/*!
 * \brief initSDCard                first two message to indicate a spi mode
 * \return                          true if the initialization was sucesseful, false otherwise
 */
bool initSDCard(void);

/*!
 * \brief clearACKbuffer            assing 0xFF to all values in the buffers
 * \param buf                       buffer to be cleared
 * \param size                      size of the buffer
 */
void clearACKbuffer(uint8_t * buf, uint8_t size);

/*!
 * \brief checkIfACK                look if the buffer contain at leat on msgACK
 * \param buf                       buffer to be checked
 * \param size                      size of the buffer
 * \param msgACK                    msg that is searched in the the buffer
 * \return                          true if the there is, at least, one msgACK, false otherwise
 */
bool checkIfACK(uint8_t * buf, uint8_t size,uint8_t msgACK);

/*!
 * \brief SDCardWriteBytes                write Bytes into SD card
 * \param toWrite                   buffer that contain the values to be written
 * \param size                      amount of data to be written
 * \param startPosition             position to start writing process
 * \return                          true if the writing process was sucesseful, false otherwise
 */
bool SDCardWriteBytes(uint8_t * toWrite, uint16_t size, uint32_t startPosition);

/*!
 * \brief readBytes                 read Bytes from SD card
 * \param toRead                    buffer that will recieve the read Bytes
 * \param size                      amount of data to be read
 * \param startPosition             position to start the reading process
 * \return                          true if the reading process was sucesseful, false otherwise
 */
bool SDCardReadBytes(uint8_t * toRead, uint16_t size, uint32_t startPosition);


bool sendCommand0(void);

bool sendCommand(uint8_t cmd);

#ifdef __cplusplus
}
#endif

#endif /*INTERFACE_SDCARD_PINCINATO_H*/
