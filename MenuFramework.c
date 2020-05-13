#include <stdio.h>
#include "stm32f10x.h"
#include "lcd.h"
#include "MenuFramework.h"
#include "delay.h"
#include "keynav.h"

uint8_t rem =100; 
//uint8_t MIN = 1;  //Don't Change
//uint8_t MAX = 4;  //Max Number of Menus
uint8_t move = 0; // Dont Change

setdisplay defaultvals;
tiledefine tile1define;
tiledefine tile2define;
tiledefine tile3define;
tiledefine tile4define;

void defaultvalsinit(void)
{
	//Set your Menu Content titles and more//
	defaultvals.Hometitle = "H O M E";
	defaultvals.menu1 = "VoltMeter";
	defaultvals.menu2 = "About System";
	defaultvals.menu3 = "Dummy Menu 3";
	defaultvals.menu4 = "Dummy Menu 4";
	defaultvals.menu5 = "Dummy Menu 5";
	defaultvals.menu6 = "Dummy Menu 6";
	defaultvals.ver = "Ver 0.6";
	//Set Text & Background colors
	defaultvals.Hometitlebg = BLACK;//MetalicGrey;
	defaultvals.textcolor = WHITE;
	defaultvals.textcolor1 = BLACK;
	defaultvals.hovercolor = BLACK;
	defaultvals.tilescolor = LightRed;//0x42AF;//BLUE;
	defaultvals.bg = WHITE;
	defaultvals.titlebg = defaultvals.tilescolor;//MetalicGrey;
	defaultvals.submenupaddingv = 5;
	
	tile1define.tileX= 5;
	tile1define.tileY= line3;
	tile1define.tileExpand= 110;//110
	tile1define.tileColor= LightRed;
	tile1define.tileName = defaultvals.menu1;
	
	tile2define.tileX= tile1define.tileX+tile1define.tileExpand+ 5 ;//122;
	tile2define.tileY= tile1define.tileY;//line3;
	tile2define.tileExpand= tile1define.tileExpand;//110;
	tile2define.tileColor= LightRed;
	tile2define.tileName = defaultvals.menu2;
	
	tile3define.tileX= tile1define.tileX;//5;
	tile3define.tileY= tile1define.tileY+(tile1define.tileExpand/2)+5;//line10+4;
	tile3define.tileExpand= tile1define.tileExpand;//110;
	tile3define.tileColor= LightRed;
	tile3define.tileName = defaultvals.menu3;
	
	tile4define.tileX= tile3define.tileX+tile3define.tileExpand+ 5;//122;
	tile4define.tileY= tile3define.tileY;//line10+4;
	tile4define.tileExpand= tile1define.tileExpand;//110;
	tile4define.tileColor= LightRed;
	tile4define.tileName = defaultvals.menu4;
}

void splashloadanimation(void)
{
	//163
	static int load = 54;
	LCD_ShowString(1, line20, 320, 16, 12, "Loading [                   ]");

	int abc = load + 21;
	//Load Animation//
	for (;load<=abc;load++){
	LCD_ShowString(load, line20, 320, 16, 12, "|" );
	delay_ms(2);
	}
	
}

void splashdisplay(void)
{
	//LCD_Clear(BLACK);
	POINT_COLOR=defaultvals.textcolor;
	LCD_ShowString(1, line1, 320, 16, 16, "Startup processes:" );
	LCD_ShowString(190, line20, 320, 16, 12, defaultvals.ver);
		splashloadanimation();
	LCD_ShowString(1, line3, 320, 16, 16, "[  ] Spi Init( );" ); 
	LCD_ShowString(1, line4, 320, 16, 16, "[  ] Spi Started" );
	POINT_COLOR=GREEN;
	LCD_ShowString(1, line4, 320, 16, 16, " OK" );
	POINT_COLOR=WHITE;
		splashloadanimation();
	LCD_ShowString(1, line5, 320, 16, 16, "[  ] Led Init( );" );
	LCD_ShowString(1, line6, 320, 16, 16, "[  ] Led Started" );
	POINT_COLOR=GREEN;
	LCD_ShowString(1, line6, 320, 16, 16, " OK" );
	POINT_COLOR=WHITE;
		splashloadanimation();
	LCD_ShowString(1, line7, 320, 16, 16, "[  ] Lcd Init( );" );
	LCD_ShowString(1, line8, 320, 16, 16, "[  ] Lcd Started" );
	POINT_COLOR=GREEN;
	LCD_ShowString(1, line8, 320, 16, 16, " OK" );
	POINT_COLOR=WHITE;
	splashloadanimation();
	LCD_ShowString(1, line9, 320, 16, 16, "[  ] Defaultvals Init( );" );
	LCD_ShowString(1, line10, 320, 16, 16, "[  ] Defaultvals Configured" );
	POINT_COLOR=GREEN;
	LCD_ShowString(1, line10, 320, 16, 16, " OK" );
	POINT_COLOR=WHITE;
		splashloadanimation();
	LCD_ShowString(1, line11, 320, 16, 16, "[  ] navsupport3key( );" );
	LCD_ShowString(1, line12, 320, 16, 16, "[  ] navsupport3key Configured" );
	POINT_COLOR=GREEN;
	LCD_ShowString(1, line12, 320, 16, 16, " OK" );
	POINT_COLOR=WHITE;
	delay_ms(93);
}	

void tiles(uint8_t tileStartX,uint8_t tileStartY,uint8_t tileExpand, uint16_t tileColor,uint8_t *tileName)
{
	LCD_Fill(tileStartX, tileStartY, tileStartX+tileExpand, tileStartY+tileExpand/2, tileColor);
	POINT_COLOR=defaultvals.textcolor;
	LCD_ShowString(tileStartX+4,(tileStartY+(tileExpand/2))-18,tileStartY+tileExpand,tileStartX+tileExpand,16, tileName);
}

void selecttile(uint8_t tileStartX,uint8_t tileStartY,uint8_t tileExpand, uint8_t *selectcolor)
{
	POINT_COLOR = selectcolor;
	LCD_DrawRectangle(tileStartX,tileStartY,tileStartX+tileExpand,tileStartY+tileExpand/2);
	LCD_DrawRectangle(tileStartX+1,tileStartY+1,tileStartX+tileExpand-1,(tileStartY+tileExpand/2)-1);
	LCD_DrawRectangle(tileStartX+2,tileStartY+2,tileStartX+tileExpand-2,(tileStartY+tileExpand/2)-2);
}

void menumain() // HOME SCREEN
	{	
		POINT_COLOR= defaultvals.textcolor;
		LCD_Fill(0,0,239,18, defaultvals.Hometitlebg);
		LCD_ShowString(6,line1,120,16,16,defaultvals.Hometitle); //13  16
		
		tiles(tile1define.tileX,tile1define.tileY,tile1define.tileExpand,tile1define.tileColor,tile1define.tileName);
		tiles(tile2define.tileX,tile2define.tileY,tile2define.tileExpand,tile2define.tileColor,tile2define.tileName);
		tiles(tile3define.tileX,tile3define.tileY,tile3define.tileExpand,tile3define.tileColor,tile3define.tileName);
		tiles(tile4define.tileX,tile4define.tileY,tile4define.tileExpand,tile4define.tileColor,tile4define.tileName);

		
	//	tiles(6,line17+6,110,defaultvals.tilescolor,defaultvals.menu5);
	//	tiles(122,line17+6,110,defaultvals.tilescolor,defaultvals.menu6);
	}

void menuchoose(uint8_t choose)
	{
	 switch(choose)
		{
			case 1:
				selecttile(tile1define.tileX,tile1define.tileY,tile1define.tileExpand,defaultvals.hovercolor);	
			  selecttile(tile2define.tileX,tile2define.tileY,tile2define.tileExpand,tile2define.tileColor);	
				break;
			
			case 2:
				selecttile(tile1define.tileX,tile1define.tileY,tile1define.tileExpand,tile1define.tileColor);
				selecttile(tile2define.tileX,tile2define.tileY,tile2define.tileExpand,defaultvals.hovercolor);
				selecttile(tile3define.tileX,tile3define.tileY,tile3define.tileExpand,tile3define.tileColor);
				break;
			
			case 3:
				selecttile(tile2define.tileX,tile2define.tileY,tile2define.tileExpand,tile2define.tileColor);
			  selecttile(tile3define.tileX,tile3define.tileY,tile3define.tileExpand,defaultvals.hovercolor);
			  selecttile(tile4define.tileX,tile4define.tileY,tile4define.tileExpand,tile4define.tileColor);
				break;

			case 4:
				selecttile(tile3define.tileX,tile3define.tileY,tile3define.tileExpand,tile3define.tileColor);
				selecttile(tile4define.tileX,tile4define.tileY,tile4define.tileExpand,defaultvals.hovercolor);	
				break;
	/*	
			case 5:
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(3,line6,320,16,16, ">>");
				POINT_COLOR=defaultvals.hovercolor;;
				LCD_ShowString(3,line7,320,16,16, ">>");
			  //POINT_COLOR=defaultvals.textcolor;
				//LCD_ShowString(3,line3,320,16,16, ">>");
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(5,200,320,16,16, defaultvals.menu6);
				break;
			
			case 6:
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(3,line7,320,16,16, ">>");
			
				POINT_COLOR=defaultvals.hovercolor;
				LCD_ShowString(5,200,320,16,16, defaultvals.menu6);
				break;
		*/
	 }
	}

void subwindowframe(uint8_t *title)
	{
		LCD_Clear(BLACK);
		POINT_COLOR=defaultvals.textcolor;
		LCD_Fill(0,0,239,20,defaultvals.titlebg);
		LCD_ShowString(226,line1+1,20,16,16, "X");
		LCD_ShowString(defaultvals.submenupaddingv,line1,320,16,16, title);
	}

void selectXmark(uint8_t selectvalue)
{
	if(selectvalue == 1)
	{
	LCD_Fill(219,0,239,20,RED);
	POINT_COLOR=defaultvals.textcolor1;
	}
	else
		{
			LCD_Fill(219,0,239,20,defaultvals.titlebg);
			POINT_COLOR=WHITE;
		}
	LCD_ShowString(226,line1+1,20,16,16, "X");	
	
}	
		
void Displaymenu (void)
		{
			//static uint8_t move = 1; // Dont Change //default select line

			////////////MainMenu Navigation////////////
				menumain();         //Display MainMenu
				menuchoose(move);		//Chooses SubMenu		
		}
/*
void livetiles(void)
{						
		static int m = 3;
		LCD_Fill(3, 123, 103, 216, BRRED);
		if(move==6)POINT_COLOR=BLACK;
		else POINT_COLOR=defaultvals.textcolor; 
		LCD_ShowString(5,200,320,16,16, defaultvals.menu6);
	  showimage(m,123);
	  if(m<60)
		{
			m= m+30;
		}
		else {m=3;}
		delay_ms(5000);
} */






/////////////////////////////////Backup Codes////////////////////////////////////
		/*	  while(1)
					{
						//livetiles();
						
						if(GPIOA -> IDR & GPIO_IDR_IDR0) //Choose Next
							{
								move ++;
								if(move > MAX) 
									{
										move = MIN; 
									}
								menuchoose(move);		//Chooses SubMenu	
								delay_ms(150);
							}
						
						if(GPIOA -> IDR & GPIO_IDR_IDR2) //Choose Previous
							{
								move = move - 1;
								if(move < MIN) 
									{
										move = 1;
									}
								menuchoose(move);		//Chooses SubMenu	
								delay_ms(150);
							}
										
						if(GPIOA -> IDR & GPIO_IDR_IDR1) //Selecting the Submenu
							{	
								rem= move;
								break;
							}	
					}
	
void backtomenu(void)
	{
		if(GPIOA -> IDR & GPIO_IDR_IDR2)  //Back to Mainmenu
										{
											rem = 0;
										}
	}

void buttonnooption(void)
{
	if(nooption == 1) 
							{	
								POINT_COLOR=RED;
								LCD_ShowString(70,line20,320,16,16,"No Option");   //302
								delay_ms(1000);
								POINT_COLOR=BLACK;
								LCD_ShowString(70,line20,320,16,16,"No Option");   //302
							
								uint8_t nooption = 0;
							}
}*/
