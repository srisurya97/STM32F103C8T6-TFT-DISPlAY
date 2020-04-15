#include "led.h"
#include "delay.h"
#include "lcd.h"
#include "spi.h"	
#include "LCDFunctions.h"
#include "stm32f10x.h"
#include "MenuFramework.h"



int main()
	{
		SPI1_Init();
		navsupport3key();
		delay_init();
		LED_Init();			     
		LCD_Init();
		defaultvalsinit();
		splashdisplay();	
		LCD_Clear(BLACK);
		while(1)
			{
				Displaymenu(); //MainMenu & selection & Navigation//
													
	////////////////				//Menu Selected//        //////////////////
				while (rem == 1)      ///when menu1 selected 
					{
						subwindowframe(defaultvals.menu1); //Window frame for submenu 

						
							//////////////Content Here///////////////
						//showimage(240, 320);
						
						while( rem == 1)
							{
								backtomenu();		
							}	
					}
				
	///////////////////////////////    /////////////////// ////////////////////				
				while (rem == 2)		///when menu2 selected
					{
						subwindowframe(defaultvals.menu2);

						
							//////////////Content Here///////////////

						
						while( rem == 2)
							{
								backtomenu();
							}	
					}

	///////////////////////    //////////////////////////////// //////////////////////				
				while (rem == 3)			///when menu3 selected
					{
						subwindowframe(defaultvals.menu3);
	
							//////////////Content Here///////////////

							
						while( rem == 3)
							{
							  backtomenu();
							}	
						}
						
	/////////////// ///////////////////////////    //////////////////////////			
				while(rem == 4)			///when menu4 selected
						{
							subwindowframe(defaultvals.menu4);
							POINT_COLOR= defaultvals.textcolor1;  
							LCD_Fill(0,30,239,150, defaultvals.bg); //defaultvals.bg);
							LCD_ShowString(defaultvals.submenupaddingv,line3,320,16,16, "CORE      ->ARM CORTEX M3");
							LCD_ShowString(defaultvals.submenupaddingv,line4,320,16,16, "MCU       ->STM32F103C8T6");
							LCD_ShowString(defaultvals.submenupaddingv,line5,320,16,16, "RAM/ROM   ->20K/128K");
							LCD_ShowString(defaultvals.submenupaddingv,line6,320,16,16, "SYSTEM CLK->72 MHz");
							LCD_ShowString(defaultvals.submenupaddingv,line7,320,16,16, "DISPLAY   ->2.8 TFT SPI LCD");
							LCD_ShowString(defaultvals.submenupaddingv,line8,320,16,16, "RESOLUTION->240*320");
							LCD_ShowString(defaultvals.submenupaddingv,line9,320,16,16, "LCD 1602A ->SIMPLE DEBUGGER");
										
							while( rem == 4)
								{
									backtomenu();
								}
						}				
					
	///////////////////////   //////////////////   ////////////////				
				while(rem == 5)				///when menu5 selected
						{ 
							subwindowframe(defaultvals.menu5);
							POINT_COLOR=defaultvals.hovercolor;
							LCD_ShowString(1,33,320,16,16, ">> YES"); ///content///
							LCD_ShowString(1,50,320,16,16, ">> NO");  ///content///
							
							while( rem == 5)
								{
									backtomenu();
								}	
						}
						
	/////////////////// ////////////////// //////////////////////////					
				while(rem == 6)				///when menu5 selected
						{ 
							subwindowframe("Photos");
							
							showimage(20,20);
						  
							
							while( rem == 6)
								{
									backtomenu();
								}	
						}
		
				
						
	/////////////////////  ////////////////////   //////////////////					

			
			LCD_Clear(BLACK);
		}
	
	
	}


	
	
