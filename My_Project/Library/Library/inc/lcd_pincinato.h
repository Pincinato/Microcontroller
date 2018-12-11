#ifndef LCD_PINCINATO_H
#define LCD_PINCINATO_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "lcd_driver.h"
#include "spi.h"

void drawPixel(uint8_t x,uint32_t y);
void drawGraph(uint32_t *y,uint32_t size);
void drawEachPixelGraph(uint32_t y);
void initLCD(void);

#ifdef __cplusplus
}
#endif

#endif /* LCD_PINCINATO_H */
