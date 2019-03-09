/******************************************************************************
* @file     interface_USART_pincinato.h
* @brief    This file handle UART via DMA
* @author   Thiago Henrique Pincinato
* @version  V1.0
* @date     10.12.2012
* @hardware STM32F446RET6
******************************************************************************/
#ifndef INTERFACE_USART_PINCINATO_H
#define INTERFACE_USART_PINCINATO_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "usart.h"
#include "dma.h"
#include "structs_pincinato.h"

 /*!
 * \brief checkUart     Check if a Uart was configured
 * \return              true if it was; otherwise false
 */
bool checkUart(void);

/*!
 * \brief checkTable    Check if a Table was assigned to the local Table
 * \return              true if it was; otherwise false
 */
bool checkTable(void);

/*!
 * \brief initInterface     initialized variables of the interface, UART, DMA
 * \param A                 table that will be assigned to the local table
 * \param huart             Uart that will be assigned to the local uart
 * \return                  true if the interface was well initialized; otherwise false
 */
bool initInterface(Table *A, UART_HandleTypeDef *huart);

/*!
 * \brief setUart           set the local *Uart
 * \param huart             uart to be refenced to the local uart
 */
void setUart(UART_HandleTypeDef *huart);

/*!
 * \brief setTable          set local Table
 * \param A                 new *table to be place in the local table
 */
void setTable(Table *A);

/*!
 * \brief check_RX          check if there is a valid message in the buffer
 */
void check_RX(void);

/*!
 * \brief selectOption      recognize the message sent via UART-dma
 * \param buf               cosnt char buffer that contains the message
 */
void selectOption(const char * buf);

/*!
 * \brief getOptionAndValue     Split the message of Uart in an option and a value
 * \param buf                   const char * buffer that contains the message
 * \param opt                   char * const that recieves the option
 * \param intValue              uint32_t that recieves the value
 * \return                      true is option and value were found. Otherwise, false
 */
bool getOptionAndValue(const char * buf, char * const opt, uint32_t * intValue);

/*!
 * \brief setHigh       set High value of local table
 * \param value         new value of table's High variable
 */
void setHigh(uint32_t value);

/*!
 * \brief setNormal     set Normal value of local table
 * \param value         new value of table's Normal variable
 */
void setNormal(uint32_t value);

/*!
 * \brief setLow        set Low value of local table
 * \param value         new value of table's Low variable
 */
void setLow(uint32_t value);

/*!
 * \brief ACK           send an ACK to the UART via DMA
 */
void ACK(void);

/*!
 * \brief NACK          send an NACK to the UART via DMA
 * \param error         type of NACK to be sent
 */
void NACK(uint32_t error);

/*!
 * \brief sendActualConfig    send actual values of localTable
 */
void sendActualConfig(void);

/*!
 * \brief sendHeartExercise     send the heart rate and exercise via UART
 * \param heartRate             fisrt const char to be sent
 * \param typeExercise          second const char to be sent
 */
void sendHeartExercise(const char * heartRate, const char * typeExercise);

/*!
 * \brief clearMsg      clear buffer
 * \param toClear       buffer to clear
 * \param size          size of the buffer
 */
void clearMsg(char *toClear,int size);



#ifdef __cplusplus
}
#endif

#endif /* INTERFACE_USART_PINCINATO_H */
