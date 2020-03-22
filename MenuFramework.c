#include<stdio.h>
#include"stm32f10x.h"
#include"lcd.h"
#include"MenuFramework.h"
#include"delay.h"

uint8_t rem=0; 
uint8_t MIN = 1; //Don't Change
uint8_t MAX = 5; //Max Number of Menus
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
	delay_ms(50);
	LCD_ShowString(1, 32, 320, 16, 16, "Spi Init( );" ); 
	delay_ms(50);
	LCD_ShowString(1, 48, 320, 16, 16, "Spi Started" );
	delay_ms(50);
	LCD_ShowString(1, 64, 320, 16, 16, "Led Init( );" );
	delay_ms(50);
	LCD_ShowString(1, 80, 320, 16, 16, "Led Started" );
	delay_ms(50);
	LCD_ShowString(1, 96, 320, 16, 16, "Lcd Init( );" );
	delay_ms(50);
	LCD_ShowString(1, 112, 320, 16, 16, "Lcd Started" );
	delay_ms(50);
	LCD_ShowString(1, 128, 320, 16, 16, "Defaultvals Init( );" );
	delay_ms(50);	
	LCD_ShowString(1, 144, 320, 16, 16, "Defaultvals Configured" );
	delay_ms(50);
	LCD_ShowString(1, 160, 320, 16, 16, "Loading  [                ]" );
	delay_ms(50);
	for (load = 80; load<=201;load++){
	LCD_ShowString(load, 160, 320, 16, 16, "|" );
	delay_ms(10);
	}
	delay_ms(200);
	
}	


void menumain() //setdisplay *defaultvals
	{
		
		POINT_COLOR= defaultvals.textcolor;
		LCD_Fill(0,0,239,20, defaultvals.Hometitlebg);
		LCD_ShowString(2,2,320,16,16, defaultvals.Hometitle);
		POINT_COLOR=defaultvals.textcolor1;
		LCD_Fill(2,30,239,120, defaultvals.bg);
		LCD_ShowString(20,33,320,16,16, defaultvals.menu1);
		LCD_ShowString(20,50,320,16,16, defaultvals.menu2);
		LCD_ShowString(20,66,320,16,16, defaultvals.menu3);
		LCD_ShowString(20,82,320,16,16, defaultvals.menu4);
		LCD_ShowString(20,98,320,16,16, defaultvals.menu5);

	}
	

void menuchoose(uint8_t choose)
	{
		switch(choose)
		{
			case 1:
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(3,98,320,16,16, ">>");
				POINT_COLOR=defaultvals.hovercolor;
				LCD_ShowString(3,33,320,16,16, ">>");
				POINT_COLOR=defaultvals.textcolor;
				LCD_ShowString(3,50,320,16,16, ">>");
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
	
		
	void Displaymenu (void)
		{
			
			////////////MainMenu Navigation////////////
				menumain(); //&defaultvals						 //Display MainMenu
				menuchoose(move);		//AutoChoosing SubMenu
				delay_ms(100);
				while(1)
					{
						menuchoose(move);		//Chooses SubMenu
						if(GPIOA -> IDR & GPIO_IDR_IDR0) //Choose Next
							{
								move ++;
								if(move > MAX) 
									{
										move = MIN; 
									}
								delay_ms(150);
							}
						
						if(GPIOA -> IDR & GPIO_IDR_IDR2) //Choose Previous
							{
								move = move - 1;
								if(move < MIN) 
									{
										move = 1;
									}
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
