#ifndef MenuFramework_H
#define MenuFramework_H

#include "stm32f10x.h"

extern uint8_t rem; //=0; 
extern uint8_t MIN; // = 1; //Don't Change
extern uint8_t MAX; // = 5; //Max Number of Menus
extern uint8_t move; // = 1; // Dont Change


//defining deplay settings & more
		
typedef struct
{
	uint16_t Hometitle;
	uint16_t Hometitlebg;
	uint16_t title;
	uint16_t titlebg;
	uint16_t textcolor;
	uint16_t textcolor1;
	uint16_t textcolor2;
	uint16_t hovercolor;
	uint16_t bg;
	uint16_t subtextcolor;
	uint16_t menu1;
	uint16_t menu2;
	uint16_t menu3;
	uint16_t menu4;
	uint16_t menu5;
	
}setdisplay;

extern setdisplay defaultvals;



void subwindowframe(uint8_t *title);
void Displaymenu (void);









#endif
