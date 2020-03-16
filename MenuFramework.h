#ifndef MenuFramework_H
#define MenuFramework_H

#include "stm32f10x.h"

extern uint8_t rem; //=0; 
extern uint8_t MIN; // = 1; //Don't Change
extern uint8_t MAX; // = 5; //Max Number of Menus
extern uint8_t move; // = 1; // Dont Change


void menumain(void);
void menuchoose(uint8_t choose);
void subwindowframe(uint8_t *title);
void Displaymenu (void);









#endif
