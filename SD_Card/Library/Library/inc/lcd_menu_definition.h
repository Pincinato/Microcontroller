#ifndef LCD_MENU_DEFINITION_H
#define LCD_MENU_DEFINITION_H

#include "lcd_menu.h"

/* Main Menu */
MenuEntry first = {.description = "LED Control", .type = MENU_EVENT, .event = (menu_event) 1};
MenuEntry second = {.description = "Status of LEDs", .type = MENU_EVENT, .event = (menu_event) 2};
//MenuEntry third = {.description = "HR + Exercise", .type = MENU_EVENT, .event = (menu_event) 3};
//MenuEntry fouth = {.description = "USB", .type = MENU_EVENT, .event = (menu_event) 4};
MenuEntry *mainList[] = {&first, &second};//,&third,&fouth};
Menu mainMenu = {"Main Menu", 2, mainList};

#endif // LCD_MENU_DEFINITION_H
