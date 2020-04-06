#include<stdio.h>
#include"stm32f10x.h"
#include"lcd.h"
#include"MenuFramework.h"
#include"delay.h"

uint8_t rem=0; 
uint8_t MIN = 1; //Don't Change
uint8_t MAX = 6; //Max Number of Menus
uint8_t move = 1; // Dont Change


		
setdisplay defaultvals;

void defaultvalsinit(void)
{
	//Set your Menu Content titles here//
	defaultvals.Hometitle = "HOME";
	defaultvals.menu1 = "DUMMY MENU 1";
	defaultvals.menu2 = "DUMMY MENU 2";
	defaultvals.menu3 = "DUMMY MENU 3";
	defaultvals.menu4 = "SYSTEM SPECIFICATIONS";
	defaultvals.menu5 = "EXIT";
	defaultvals.menu6 = "Photos";
	defaultvals.ver = "Ver 0.1";
	//Set Text & Background colors
	defaultvals.Hometitlebg = TEAL;
	defaultvals.textcolor = WHITE;
	defaultvals.hovercolor = DARKBLUE;//COBALT;//GRAY; 
	defaultvals.textcolor1 = BLACK;
	defaultvals.bg = WHITE;
	defaultvals.titlebg = TEAL;
	defaultvals.submenupaddingv = 5;
}

void splashdisplay(void)
{
	int load;
	LCD_Clear(BLACK);
	POINT_COLOR=defaultvals.textcolor;
	LCD_ShowString(1, 1, 320, 16, 16, "Startup processes:" );
	delay_ms(25);
	LCD_ShowString(1, 32, 320, 16, 16, "[  ] Spi Init( );" ); 
	delay_ms(25);
	
	LCD_ShowString(1, 48, 320, 16, 16, "[  ] Spi Started" );
	POINT_COLOR=GREEN;
	LCD_ShowString(1, 48, 320, 16, 16, " OK" );
	POINT_COLOR=WHITE;
	delay_ms(25);
	
	LCD_ShowString(1, 64, 320, 16, 16, "[  ] Led Init( );" );
	delay_ms(25);
	delay_ms(25);
	
	LCD_ShowString(1, 80, 320, 16, 16, "[  ] Led Started" );
	delay_ms(25);
	POINT_COLOR=GREEN;
	LCD_ShowString(1, 80, 320, 16, 16, " OK" );
	POINT_COLOR=WHITE;
	
	LCD_ShowString(1, 96, 320, 16, 16, "[  ] Lcd Init( );" );
	delay_ms(25);
	
	LCD_ShowString(1, 112, 320, 16, 16, "[  ] Lcd Started" );
	delay_ms(25);
	POINT_COLOR=GREEN;
	LCD_ShowString(1, 112, 320, 16, 16, " OK" );
	POINT_COLOR=WHITE;
	
	LCD_ShowString(1, 128, 320, 16, 16, "[  ] Defaultvals Init( );" );
	delay_ms(25);	
	LCD_ShowString(1, 144, 320, 16, 16, "[  ] Defaultvals Configured" );
	delay_ms(25);
	POINT_COLOR=GREEN;
	LCD_ShowString(1, 144, 320, 16, 16, " OK" );
	POINT_COLOR=WHITE;
	LCD_ShowString(1, 160, 320, 16, 16, "Loading  [                ]" );
	delay_ms(25);
	LCD_ShowString(190, 305, 320, 16, 12, defaultvals.ver);
	delay_ms(25);
	
	for (load = 80; load<=201;load++){
	LCD_ShowString(load, 160, 320, 16, 16, "|" );
	delay_ms(3);
	}
	delay_ms(103);
	
}	


void menumain() // HOME SCREEN
	{
		
		//int firstpos = 33;		
		POINT_COLOR= defaultvals.textcolor;
		LCD_Fill(0,0,239,18, defaultvals.Hometitlebg);
		LCD_ShowString(2,2,320,16,16, defaultvals.Hometitle);
		POINT_COLOR=defaultvals.textcolor1;
		LCD_Fill(3,30,239,120, defaultvals.bg);
		LCD_ShowString(20,33,320,16,16, defaultvals.menu1);   //33
		LCD_ShowString(20,49,320,16,16, defaultvals.menu2);       //50
		LCD_ShowString(20,65,320,16,16, defaultvals.menu3);       //66
		LCD_ShowString(20,81,320,16,16, defaultvals.menu4);       //82
		LCD_ShowString(20,97,320,16,16, defaultvals.menu5);       //98
		
		
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
				LCD_ShowString(3,33,320,16,16, ">>");
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(3,50,320,16,16, ">>");
			
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(5,200,320,16,16, defaultvals.menu6);
			  break;
			
			case 2:
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(3,33,320,16,16, ">>");
				POINT_COLOR=defaultvals.hovercolor;
				LCD_ShowString(3,50,320,16,16, ">>");
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(3,66,320,16,16, ">>");
			  break;
			
			case 3:
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(3,50,320,16,16, ">>");
		    POINT_COLOR=defaultvals.hovercolor;
				LCD_ShowString(3,66,320,16,16, ">>");
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(3,82,320,16,16, ">>");
			  break;

			case 4:
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(3,66,320,16,16, ">>");
				POINT_COLOR=defaultvals.hovercolor;;
				LCD_ShowString(3,82,320,16,16, ">>");
				POINT_COLOR=defaultvals.textcolor;
			  LCD_ShowString(3,98,320,16,16, ">>");
				break;
			
			case 5:
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(3,82,320,16,16, ">>");
				POINT_COLOR=defaultvals.hovercolor;;
				LCD_ShowString(3,98,320,16,16, ">>");
			  POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(3,33,320,16,16, ">>");
			  
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(5,200,320,16,16, defaultvals.menu6);
				break;
			
			case 6:
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(3,98,320,16,16, ">>");
			
				POINT_COLOR=BLACK;
				LCD_ShowString(5,200,320,16,16, defaultvals.menu6);
				break;
		
			
		}
		
	}
	
	
	void subwindowframe(uint8_t *title)
		{
			LCD_Clear(BLACK);
			POINT_COLOR=defaultvals.textcolor;
			LCD_Fill(0,0,239,20,defaultvals.titlebg);
			//LCD_DrawRectangle(1, 1, 170, 20);
			LCD_ShowString(2,2,320,16,16, title);
							
			POINT_COLOR=DARKBLUE;
			LCD_ShowString(5,300,320,16,16,"<<< BACK");
			
		}
	
/* void livetiles(void)
{						
						static int m = 3;
						LCD_Fill(m,123,m,123,BRED);
						showimage(m,123);
						if(m<53){	m= m+2;}
						else {m=3;}
}       */
		
	void Displaymenu (void)
		{
			
			////////////MainMenu Navigation////////////
				menumain(); //&defaultvals						 //Display MainMenu
				menuchoose(move);		//Chooses SubMenu
				while(1)
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
			
		}
	
	
void backtomenu(void)
	{
		if(GPIOA -> IDR & GPIO_IDR_IDR2)  //Back to Mainmenu
										{
											rem = 0;
										}
		
	}
