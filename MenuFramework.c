#include <stdio.h>
#include "stm32f10x.h"
#include "lcd.h"
#include "MenuFramework.h"
#include "delay.h"
#include "keynav.h"

uint8_t rem =100; 
uint8_t MIN = 1;  //Don't Change
uint8_t MAX = 6;  //Max Number of Menus
uint8_t move = 1; // Dont Change

setdisplay defaultvals;

void defaultvalsinit(void)
{
	//Set your Menu Content titles here//
	defaultvals.Hometitle = "HOME";
	defaultvals.menu1 = "Dummy Menu 1";
	defaultvals.menu2 = "Dummy Menu 2";
	defaultvals.menu3 = "Touch Calibration";
	defaultvals.menu4 = "System Specifications";
	defaultvals.menu5 = "Exit";
	defaultvals.menu6 = "Photos";
	defaultvals.ver = "Ver 0.3";
	//Set Text & Background colors
	defaultvals.Hometitlebg = MetalicGrey;
	defaultvals.textcolor = WHITE;
	defaultvals.hovercolor = BLACK;
	defaultvals.textcolor1 = BLACK;
	defaultvals.bg = WHITE;
	defaultvals.titlebg = MetalicGrey;
	defaultvals.submenupaddingv = 5;
}

void splashloadanimation(void)
{
	//163
	static int load = 54;
	LCD_ShowString(1, line20, 320, 16, 12, "Loading [                   ]" );

	int abc = load + 21;
	//Load Animation//
	for (;load<=abc;load++){
	LCD_ShowString(load, line20, 320, 16, 12, "|" );
	delay_ms(3);
	}
	
}

void splashdisplay(void)
{
	//LCD_Clear(BLACK);
	POINT_COLOR=defaultvals.textcolor;
	LCD_ShowString(1, line1, 320, 16, 16, "Startup processes:" );
	delay_ms(25);
	LCD_ShowString(190, line20, 320, 16, 12, defaultvals.ver);
		splashloadanimation();
	LCD_ShowString(1, line3, 320, 16, 16, "[  ] Spi Init( );" ); 
	delay_ms(25);
	LCD_ShowString(1, line4, 320, 16, 16, "[  ] Spi Started" );
	POINT_COLOR=GREEN;
	LCD_ShowString(1, line4, 320, 16, 16, " OK" );
	POINT_COLOR=WHITE;
	delay_ms(25);
		splashloadanimation();
	LCD_ShowString(1, line5, 320, 16, 16, "[  ] Led Init( );" );
	delay_ms(25);
	delay_ms(25);
	LCD_ShowString(1, line6, 320, 16, 16, "[  ] Led Started" );
	delay_ms(25);
	POINT_COLOR=GREEN;
	LCD_ShowString(1, line6, 320, 16, 16, " OK" );
	POINT_COLOR=WHITE;
		splashloadanimation();
	LCD_ShowString(1, line7, 320, 16, 16, "[  ] Lcd Init( );" );
	delay_ms(25);
	LCD_ShowString(1, line8, 320, 16, 16, "[  ] Lcd Started" );
	delay_ms(25);
	POINT_COLOR=GREEN;
	LCD_ShowString(1, line8, 320, 16, 16, " OK" );
	POINT_COLOR=WHITE;
	splashloadanimation();
	LCD_ShowString(1, line9, 320, 16, 16, "[  ] Defaultvals Init( );" );
	delay_ms(25);	
	LCD_ShowString(1, line10, 320, 16, 16, "[  ] Defaultvals Configured" );
	delay_ms(25);
	POINT_COLOR=GREEN;
	LCD_ShowString(1, line10, 320, 16, 16, " OK" );
	POINT_COLOR=WHITE;
		splashloadanimation();
	LCD_ShowString(1, line11, 320, 16, 16, "[  ] navsupport3key( );" );
	delay_ms(25);	
	LCD_ShowString(1, line12, 320, 16, 16, "[  ] navsupport3key Configured" );
	delay_ms(25);
	POINT_COLOR=GREEN;
	LCD_ShowString(1, line12, 320, 16, 16, " OK" );
	POINT_COLOR=WHITE;
	delay_ms(103);
}	

void menumain() // HOME SCREEN
	{	
		POINT_COLOR= defaultvals.textcolor;
		LCD_Fill(0,0,239,18, defaultvals.Hometitlebg);
		LCD_ShowString(2,line1,320,16,16, defaultvals.Hometitle);
		POINT_COLOR=defaultvals.textcolor1;
		LCD_Fill(3,30,239,120, defaultvals.bg);
		LCD_ShowString(20,line3,320,16,16, defaultvals.menu1);   		 //33
		LCD_ShowString(20,line4,320,16,16, defaultvals.menu2);       //50
		LCD_ShowString(20,line5,320,16,16, defaultvals.menu3);       //66
		LCD_ShowString(20,line6,320,16,16, defaultvals.menu4);       //82
		LCD_ShowString(20,line7,320,16,16, defaultvals.menu5);       //98
		
		LCD_Fill(3, 123, 103, 216, BRRED);
		POINT_COLOR=defaultvals.textcolor;
		LCD_ShowString(5,200,320,16,16, defaultvals.menu6);
	}

void menuchoose(uint8_t choose)
	{
		switch(choose)
		{
			case 1:
				//POINT_COLOR=defaultvals.textcolor;
				//LCD_ShowString(3,98,320,16,16, ">>");
				POINT_COLOR=defaultvals.hovercolor;
				LCD_ShowString(3,line3,320,16,16, ">>");
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(3,line4,320,16,16, ">>");
			
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(5,200,320,16,16, defaultvals.menu6);
			  break;
			
			case 2:
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(3,line3,320,16,16, ">>");
				POINT_COLOR=defaultvals.hovercolor;
				LCD_ShowString(3,line4,320,16,16, ">>");
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(3,line5,320,16,16, ">>");
			  break;
			
			case 3:
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(3,line4,320,16,16, ">>");
		    POINT_COLOR=defaultvals.hovercolor;
				LCD_ShowString(3,line5,320,16,16, ">>");
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(3,line6,320,16,16, ">>");
			  break;

			case 4:
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(3,line5,320,16,16, ">>");
				POINT_COLOR=defaultvals.hovercolor;;
				LCD_ShowString(3,line6,320,16,16, ">>");
				POINT_COLOR=defaultvals.textcolor;
			  LCD_ShowString(3,line7,320,16,16, ">>");
				break;
			
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
		}
		
	}

	void subwindowframe(uint8_t *title)
		{
			LCD_Clear(BLACK);
			POINT_COLOR=defaultvals.textcolor;
			LCD_Fill(0,0,239,20,defaultvals.titlebg);
			//LCD_DrawRectangle(220, 0, 239, 20);
			LCD_ShowString(226,line1+1,20,16,16, "X");
			LCD_ShowString(2,line1,320,16,16, title);
			//POINT_COLOR=DARKBLUE;
			//LCD_ShowString(5,line20,320,16,16,"<<< BACK");
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
	//POINT_COLOR=defaultvals.textcolor;
	//LCD_DrawRectangle(220, 0, 239, 20);	
}	
		
void Displaymenu (void)
		{
			//static uint8_t move = 1; // Dont Change //default select line
			
			////////////MainMenu Navigation////////////
				menumain();         //Display MainMenu
				menuchoose(move);		//Chooses SubMenu		
		}

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
} 






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
