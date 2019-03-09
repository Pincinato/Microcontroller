/******************************************************************************
* @file     lcd_pincinato.h
* @brief    This file support funtions to facilitate the use of lcd_driver and plot of ADC values
* @author   Thiago Henrique Pincinato
* @version  V1.0
* @date     10.12.2012
* @hardware STM32F446RET6
******************************************************************************/
#ifndef LCD_PINCINATO_H
#define LCD_PINCINATO_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "lcd_driver.h"
#include "spi.h"

 /*!
 * \brief drawPixel     draw a pixel in the lcd
 * \param x             x condinate
 * \param y             y cordinate
 */
void drawPixel(uint8_t x,uint32_t y);

/*!
 * \brief drawGraph     draw several point in the LCD
 * \param y             array with point to be drawn
 * \param size          size of array
 */
void drawGraph(uint32_t *y,uint32_t size);

/*!
 * \brief drawEachPixelGraph    draw an unit pixel, rescaling value of 0-3.3V to 0to 127
 * \param y                     value to be drawn
 */
void drawEachPixelGraph(uint32_t y);

/*!
 * \brief initLCD       initializes
 */
void initLCD(void);

#ifdef __cplusplus
}
#endif

#endif /* LCD_PINCINATO_H */
