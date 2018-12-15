/******************************************************************************
* @file     structs_pincinato.h
* @brief    This file contain a struct Table used by other library
* @author   Thiago Henrique Pincinato
* @version  V1.0
* @date     10.12.2012
* @hardware STM32F446RET6
******************************************************************************/
#ifndef STRUCTS_PINCINATO_H
#define STRUCTS_PINCINATO_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>


//Table in which value of the heart rate are setted up according to the activity level, High - Norral -Low
typedef struct{
	
	uint32_t High_HeartRate; //Minimal rate to hard activity
	uint32_t Normal_HeartRate; // Minimal rate to normal Acitity
	uint32_t Low_HeartRate; // Minimal rate to low acitiviy
  bool sdCardStatus; //
}Table;
 /*!
 * \brief initTable     Initialize table with standart values
 * \param A             table to be initialized
 */
void initTable(Table *A);

/*!
 * \brief loadTableValues   Load values from the SD card and using those values
 *                          set attributes of table A
 * \param A                 table to be set
 * \return                  true if values are loaded
 */
bool loadTableValues(Table *A);

/*!
 * \brief saveTableValues    Save values into the SD card and using table A values
 * \param A                  tables in which values are used
 * \return                   true if values are saved
 */
bool saveTableValues(Table *A);

#ifdef __cplusplus
}
#endif

#endif /* STRUCTS_PINCINATO_H*/
