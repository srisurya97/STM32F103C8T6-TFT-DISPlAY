#ifndef MenuFramework_H
#define MenuFramework_H

#include "stm32f10x.h"

extern uint8_t rem; 
extern uint8_t move; 
		
typedef struct
{
	uint8_t *Hometitle;
	uint16_t Hometitlebg;
	uint16_t title;
	uint16_t titlebg;
	uint16_t textcolor;
	uint16_t textcolor1;
	uint16_t textcolor2;
	uint8_t *hovercolor;
	uint16_t tilescolor;
	uint16_t bg;
	uint16_t subtextcolor;
	uint8_t *menu1;
	uint8_t *menu2;
	uint8_t *menu3;
	uint8_t *menu4;
	uint8_t *menu5;
	uint8_t *menu6;
	uint16_t menu7;
	uint16_t menu8;
	uint16_t menu9;
	uint8_t *ver;
	uint8_t  submenupaddingv;
	uint8_t  mainmenupaddingv;
}setdisplay;

extern setdisplay defaultvals;

typedef struct 
{
  uint8_t  tileX;
	uint8_t tileY;
	uint8_t tileExpand; 
	uint8_t *tileColor;
	uint8_t *tileName;
}tiledefine;


#define lineht 15.9
#define line1 ((lineht*0)+1)			 //1	
#define line2 ((lineht*1)+1)       //17
#define line3 ((lineht*2)+1)       //33
#define line4 ((lineht*3)+1)       //49
#define line5 ((lineht*4)+1)       //65
#define line6 ((lineht*5)+1)       //81
#define line7 ((lineht*6)+1)       //97
#define line8 ((lineht*7)+1)       //113
#define line9 ((lineht*8)+1)       //129
#define line10 ((lineht*9)+1)      //145
#define line11 ((lineht*10)+1)     //161
#define line12 ((lineht*11)+1)     //177
#define line13 ((lineht*12)+1)     //193
#define line14 ((lineht*13)+1)     //209
#define line15 ((lineht*14)+1)		 //225
#define line16 ((lineht*15)+1)		 //241
#define line17 ((lineht*16)+1)		 //257
#define line18 ((lineht*17)+1)		 //273
#define line19 ((lineht*18)+1)		 //289
#define line20 ((lineht*19)+1)		 //305 use this line for size12font //Max for lineht15.9

void displaydirnav(uint8_t selectvals);
void defaultvalsinit(void);
void splashdisplay(void);
void Displaymenu (void);
void subwindowframe(uint8_t *title);
void menuchoose(uint8_t choose);
void livetiles(void);
void selectXmark(uint8_t selectvals);
void ONOFFSwitch(uint8_t x1,uint8_t y1,uint8_t ONOFF);

#endif
