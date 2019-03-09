#include "lcd_pincinato.h"

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
