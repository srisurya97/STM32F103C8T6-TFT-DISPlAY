#include "led.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "spi.h"	
#include "LCDFunctions.h"
#include "stm32f10x.h"
#include "MenuFramework.h"


int main(void)
	{
		
		SPI1_Init();
		//PIN A0 for Nav and A1 for Back and A2 for Select //PORT A PINS 2, 1 and 0 set to input pushpull
		//RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
		//RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
		GPIOA->CRL &= ~(GPIO_CRL_MODE0| GPIO_CRL_MODE1 | GPIO_CRL_MODE2);
		GPIOA->CRL |= (GPIO_CRL_CNF0_1|GPIO_CRL_CNF1_1 | GPIO_CRL_CNF2_1);
		GPIOA->CRL &= ~(GPIO_CRL_CNF0_0|GPIO_CRL_CNF1_0 |GPIO_CRL_CNF2_0);	

		delay_init();
		LED_Init();			     
		LCD_Init();
		
		while(1)
			{
								///////////////////////////MainMenu Navigation////////////////////////////////

				Displaymenu();
													
					///////////////////////////Menu Selected////////////////////////////////
					while (rem == 1)
						{
							subwindowframe("DUMMY MENU 1"); //Window frame for submenu 

							//////////////Content Here///////////////
						
							while( rem == 1)
								{
									if(GPIOA -> IDR & GPIO_IDR_IDR2)  //Back to Mainmenu
										{
											rem = 0;
										}		
								}	
						}
						
					while (rem == 2)
						{
							subwindowframe("DUMMY MENU 2");

							//////////////Content Here///////////////

							
							while( rem == 2)
								{
									if(GPIOA -> IDR & GPIO_IDR_IDR2)  //Back to Mainmenu
										{
											rem = 0;
										}
									
								}	
						}

					while (rem == 3)
						{
							subwindowframe("DUMMY MENU 3");
	
							//////////////Content Here///////////////

							
							while( rem == 3)
								{
									if(GPIOA -> IDR & GPIO_IDR_IDR2)  //Back to Mainmenu
										{
											rem = 0;
										}
								}	
						}
						
				
				while(rem == 4)
						{
							subwindowframe("SYSTEM SPECIFICATIONS");
							POINT_COLOR=WHITE;
							LCD_ShowString(1,33,320,16,16, "CORE       -> ARM CORTEX M3");
							LCD_ShowString(1,50,320,16,16, "MCU        -> STM32F103C8T6");
							LCD_ShowString(1,66,320,16,16, "RAM/ROM    -> 20K/128K");
							LCD_ShowString(1,82,320,16,16, "SYSTEM CLK -> 72 MHz");
							LCD_ShowString(1,98,320,16,16, "DISPLAY    -> 2.8 TFT SPI LCD");
							LCD_ShowString(1,114,320,16,16,"RESOLUTION -> 240*320");
							LCD_ShowString(1,130,320,16,16,"LCD 1602A  -> SIMPLE DEBUGGER");
										
							while( rem == 4)
								{
									if(GPIOA -> IDR & GPIO_IDR_IDR2)  //Back to Mainmenu
										{
											rem = 0;
										}
								}
						}				
					
					
				while(rem == 5)
						{ 
							subwindowframe("EXIT");
							POINT_COLOR=BLUE;
							LCD_ShowString(1,33,320,16,16, "-> YES");
							LCD_ShowString(1,50,320,16,16, "-> NO");
							
							while( rem == 5)
								{
									if(GPIOA -> IDR & GPIO_IDR_IDR2)  //Back to Mainmenu
										{
											rem = 0;
										}
								}	
						}
						

			
			LCD_Clear(BLACK);
		}
	
	
	}


	
	
