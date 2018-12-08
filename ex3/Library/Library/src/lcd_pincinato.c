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

#define NUMBER_OF_COLUMNS 32

uint32_t posX=0;

void drawPixel(uint8_t x, uint32_t y){

		uint8_t newY = (uint32_t)y/128; //4096 /128 = 32
		newY = (NUMBER_OF_COLUMNS-1)-newY;	
    lcd_setPixel(x, newY, true);
}


void drawEachPixelGraph(uint32_t y){
		
	drawPixel(posX,y);
	lcd_show();
	posX++;
	if(posX ==128){
		posX=0;
		lcd_clear();
	}
}


void drawGraph(uint32_t *y,uint32_t size){
	
}

void initLCD(){
	MX_SPI1_Init();	
	lcd_init ();
	lcd_clear();
}
/****** END OF FILE ******/
