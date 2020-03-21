#include "led.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "spi.h"	
#include "LCDFunctions.h"
#include "stm32f10x.h"
#include "MenuFramework.h"



int main()
	{
		//Set your Menu Content titles here//
		defaultvals.menu1 = "  DUMMY MENU 1" ;
		defaultvals.menu2 = "  DUMMY MENU 2" ;
		defaultvals.menu3 = "  DUMMY MENU 3" ;
		defaultvals.menu4 = "  SYSTEM SPECIFICATIONS" ;
		defaultvals.menu5 = "  EXIT";
	
		
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
				Displaymenu(); //MainMenu Navigation//
														
				while (rem == 1) //Menu Selected//
					{
						subwindowframe(defaultvals.menu1); //Window frame for submenu 

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
						subwindowframe(defaultvals.menu2);

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
						subwindowframe(defaultvals.menu3);
	
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
							subwindowframe(defaultvals.menu4);
							POINT_COLOR= defaultvals.textcolor1;  
							LCD_Fill(0,30,239,150, defaultvals.bg); //defaultvals.bg);
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
							subwindowframe(defaultvals.menu5);
							POINT_COLOR=defaultvals.hovercolor;
							LCD_ShowString(1,33,320,16,16, ">> YES"); ///content///
							LCD_ShowString(1,50,320,16,16, ">> NO");  ///content///
							
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


	
	
