#ifndef LCD_MENU_DEFINITION_H
#define LCD_MENU_DEFINITION_H

#include "lcd_menu.h"

/* Main Menu */
MenuEntry first = {.description = "Temperature", .type = MENU_EVENT, .event = (menu_event) 1};
MenuEntry second = {.description = "Accelerometer", .type = MENU_EVENT, .event = (menu_event) 2};
MenuEntry *mainList[] = {&first, &second};
Menu mainMenu = {"Main Menu", 2, mainList};

#endif // LCD_MENU_DEFINITION_H
