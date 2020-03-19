#include<stdio.h>
#include"stm32f10x.h"
#include"lcd.h"
#include"MenuFramework.h"
#include"delay.h"

uint8_t rem=0; 
uint8_t MIN = 1; //Don't Change
uint8_t MAX = 5; //Max Number of Menus
uint8_t move = 1; // Dont Change


void menumain(void)
	{
		POINT_COLOR=WHITE;
		LCD_Fill(0,0,239,20, TEAL);
		//LCD_DrawRectangle(1, 1, 170, 20);
		LCD_ShowString(95,2,320,16,16,  "H O M E");
		POINT_COLOR=WHITE;
		LCD_ShowString(1,33,320,16,16, ">> DUMMY MENU 1          <<");
		LCD_ShowString(1,50,320,16,16, ">> DUMMY MENU 2          <<");
		LCD_ShowString(1,66,320,16,16, ">> DUMMY MENU 3          <<");
		LCD_ShowString(1,82,320,16,16, ">> SYSTEM SPECIFICATIONS <<");
		LCD_ShowString(1,98,320,16,16, ">> EXIT                  <<");
							
		//POINT_COLOR=WHITE;
}
	

void menuchoose(uint8_t choose)
	{
	
		switch(choose)
		{
			case 1:
				POINT_COLOR=WHITE;
				LCD_ShowString(1,98,320,16,16, ">> EXIT                  <<");
				POINT_COLOR=BLUE;
				LCD_ShowString(1,33,320,16,16, ">> DUMMY MENU 1          <<");
				POINT_COLOR=WHITE;
				LCD_ShowString(1,50,320,16,16, ">> DUMMY MENU 2          <<");
			  break;
			
			case 2:
				POINT_COLOR=WHITE;
				LCD_ShowString(1,33,320,16,16, ">> DUMMY MENU 1          <<");
				POINT_COLOR=BLUE;
				LCD_ShowString(1,50,320,16,16, ">> DUMMY MENU 2          <<");
				POINT_COLOR=WHITE;
				LCD_ShowString(1,66,320,16,16, ">> DUMMY MENU 3          <<");
				//POINT_COLOR=WHITE;
			  break;
			
			case 3:
				POINT_COLOR=WHITE;
				LCD_ShowString(1,50,320,16,16, ">> DUMMY MENU 2          <<");
		    POINT_COLOR=BLUE;
				LCD_ShowString(1,66,320,16,16, ">> DUMMY MENU 3          <<");
				POINT_COLOR=WHITE;
				LCD_ShowString(1,82,320,16,16, ">> SYSTEM SPECIFICATIONS <<");
			  break;

			case 4:
				POINT_COLOR=WHITE;
				LCD_ShowString(1,66,320,16,16, ">> DUMMY MENU 3          <<");
				POINT_COLOR=BLUE;
				LCD_ShowString(1,82,320,16,16, ">> SYSTEM SPECIFICATIONS <<");
				POINT_COLOR=WHITE;
			  LCD_ShowString(1,98,320,16,16, ">> EXIT  								 <<");
				break;
			
			case 5:
				POINT_COLOR=WHITE;
				LCD_ShowString(1,82,320,16,16, ">> SYSTEM SPECIFICATIONS <<");
				POINT_COLOR=BLUE;
				LCD_ShowString(1,98,320,16,16, ">> EXIT  								 <<");
			  POINT_COLOR=WHITE;
				LCD_ShowString(1,33,320,16,16, ">> DUMMY MENU 1          <<");
			  break;
			
		}
		
	}
	
	
	void subwindowframe(uint8_t *title)
		{
			LCD_Clear(BLACK);
			POINT_COLOR=WHITE;
			LCD_Fill(0,0,239,20,TEAL);
			//LCD_DrawRectangle(1, 1, 170, 20);
			LCD_ShowString(2,2,320,16,16, title);
							
			POINT_COLOR=COBALT;
			LCD_ShowString(5,300,320,16,16,"<<< BACK");
			POINT_COLOR=WHITE;
			
		}
	
		
	void Displaymenu (void)
		{
				///////////////////////////MainMenu Navigation////////////////////////////////
				menumain();						 //Display MainMenu
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
	
	
	
	
	
	
