#include "led.h"
#include "delay.h"
#include "lcd.h"
#include "spi.h"	
#include "LCDFunctions.h"
#include "stm32f10x.h"
#include "MenuFramework.h"
#include "keynav.h"
#include "ADC.h"



int main()
	{
	
		delayinit();
		delay_ms(100); //for proper lcd initialization
		SPI1_Init();
		LED_Init();			     
		LCD_Init();
		defaultvalsinit();
		splashdisplay();
		navsupport3key();
		while(1)
			{
				if (rem == 100)
				{
					LCD_Clear(BLACK);
					Displaymenu(); //MainMenu & selection & Navigation//
					rem = 0;
				}
				
				while(rem == 0)
				{
					// can be used for live tiles in home menu
					//livetiles();
					
				}
								
	////////////////				//Menu Selected//        //////////////////
				if(rem == 1)				///when menu5 selected
						{ 
							subwindowframe(defaultvals.menu1);
							adcinit();
						  
														
							while( rem == 1)
								{
									ADCConvert();
									//////////////////////////////////////
								}	
						}
				
	///////////////////////    //////////////////////////////// //////////////////////				
				
				if(rem == 2)			///when menu4 selected
						{
							subwindowframe(defaultvals.menu2);
							POINT_COLOR= defaultvals.textcolor;//defaultvals.textcolor1;  
							//LCD_Fill(0,30,239,150, defaultvals.bg); //defaultvals.bg);
							LCD_ShowString(defaultvals.submenupaddingv,line3,320,16,16, "CORE      ->ARM CORTEX M3");
							LCD_ShowString(defaultvals.submenupaddingv,line4,320,16,16, "MCU       ->STM32F103C8T6");
							LCD_ShowString(defaultvals.submenupaddingv,line5,320,16,16, "RAM/ROM   ->20K/128K");
							LCD_ShowString(defaultvals.submenupaddingv,line6,320,16,16, "SYSTEM CLK->72 MHz");
							LCD_ShowString(defaultvals.submenupaddingv,line7,320,16,16, "DISPLAY   ->2.8 TFT SPI LCD");
							LCD_ShowString(defaultvals.submenupaddingv,line8,320,16,16, "RESOLUTION->240*320");
							//LCD_ShowString(defaultvals.submenupaddingv,line9,320,16,16, "LCD 1602A ->SIMPLE DEBUGGER");
										
							while( rem == 2)
								{
									//////////////////////////////////////////////
								}  
						}				
					
	///////////////////////   //////////////////   ////////////////				
				
				if (rem == 3)			///when menu3 selected
					{
						subwindowframe(defaultvals.menu3);
						
							//////////////Content Here///////////////
						
				    while( rem == 3)
							{ 
								//////////////////////////////////
							}	
						}
						
	/////////////// ///////////////////////////    //////////////////////////			
					if (rem == 4)      ///when menu1 selected 
					{
						subwindowframe(defaultvals.menu4); //Window frame for submenu 
										
							//////////////Content Here///////////////
						
						while( rem == 4)
							{
									///////////////////////////
							}	
					}
				
	///////////////////////////////    /////////////////// ////////////////////				
				if (rem == 5)		///when menu2 selected
					{
						subwindowframe(defaultvals.menu5);
						
							//////////////Content Here///////////////
						
					 while( rem == 5)
							{
								////////////////////////////////
							}	
					}

						
	/////////////////// ////////////////// //////////////////////////					
				if(rem == 6)				///when menu5 selected
						{ 
							subwindowframe(defaultvals.menu6);

							
							while( rem == 6)
								{
									////////////////////////////////////
								}	
						}
						
		}
	
	
	}


	
	
