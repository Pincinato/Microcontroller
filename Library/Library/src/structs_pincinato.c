/***************************************************************************************************
  * @file
  * @author     ?? original author has to be given as well !!	
  * @version    V0.4
	* @date       30.09.2018
	* @date       20.04.2018
	* @date       25.11.2017
  * @date       28.10.2015
  * @brief      Display setup and communication for LCD on mbed16-application-shield
	* @brief      V0.4;dnd1;add;drawLine() function
  * @hardware   NucleoF446RE with mbed-016.1-application-shield
  **************************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2018 BFH-TI </center></h2>
  *
  */
#include "structs_pincinato.h"


void initTable(Table *A){

	A->High_HeartRate=0; // initial High value
	A->Normal_HeartRate=0; // initial normal value
	A->Low_HeartRate=0; // initial low level
	A->HasBeenEdited=false;
}

/****** END OF FILE ******/
