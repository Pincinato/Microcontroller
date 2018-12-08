#ifndef INTERFACE_USART_PINCINATO_H
#define INTERFACE_USART_PINCINATO_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "usart.h"
#include "dma.h"


bool checkUart(void);
bool checkTable(void);
bool initInterface(UART_HandleTypeDef *huart);
void setUart(UART_HandleTypeDef *huart);
void check_RX(void);
void selectOption(const char * buf);
bool getOptionAndValue(const char * buf, char * const opt, uint32_t * intValue);
void setHigh(uint32_t value);
void setNormal(uint32_t value);
void setLow(uint32_t value);
void ACK(void);
void NACK(uint32_t error);
void sendActualConfig(void);
void sendHeartExercise(const char * heartRate, const char * typeExercise);
void clearMsg(char *toClear,int size);



#ifdef __cplusplus
}
#endif

#endif /* INTERFACE_USART_PINCINATO_H */
