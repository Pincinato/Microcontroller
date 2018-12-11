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
	bool HasBeenEdited;
}Table;

void initTable(Table *A);

#ifdef __cplusplus
}
#endif

#endif /* STRUCTS_PINCINATO_H*/
