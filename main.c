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
				if(rem == 1)
						{		
							subwindowframe(defaultvals.menu1);
							POINT_COLOR=WHITE;
							LCD_ShowString(5,line3,240,16,16,"Mode 1 A5        Single Channel");
							LCD_ShowString(5,line4,239,16,16,"Uses: VoltMeter ");
							ONOFFSwitch(100,line3,0);
							POINT_COLOR=WHITE;
							LCD_ShowString(5,line6,240,16,16,"Mode 2 A5|A6     Dual Channel");
							LCD_ShowString(5,line7,239,16,16,"Uses: TransistorBias");
							ONOFFSwitch(100,line6,0);
							POINT_COLOR=WHITE;
							LCD_ShowString(5,line9,240,16,16,"Mode 3 A5|A6     Dual Channel");
							LCD_ShowString(5,line10,200,16,16,"Uses: Analog");
							ONOFFSwitch(100,line9,0);
							POINT_COLOR=WHITE;
							LCD_ShowString(5,line12,240,16,16,"Mode 4 A5        Single Channel");
							LCD_ShowString(5,line13,200,16,16,"Uses: Analog");
							ONOFFSwitch(100,line12,0);
							
							
							LCD_Fill(60,290,157,301,WHITE);
							POINT_COLOR=BLACK;
							LCD_ShowString(61,290,159,16,12,"Turn ON to Start");	
							
							
							while( rem == 1)
								{
									//////////////////////////////////////
								}	
								
							if(rem == 7)
								{
									LCD_Clear(BLACK);
									adcinit();
									ADCConvert();
									while(rem == 7)
										{
								
										}
								}
							if(rem == 8)
								{
									LCD_Clear(BLACK);
									adcinit();
									ADCConvert();
									while(rem == 8)
										{
								
										}
									}
							if(rem == 9)
								{
									LCD_Clear(BLACK);
									adcinit();
									ADCConvert();
									while(rem == 9)
										{
								
										}
								}
							if(rem == 10)
								{
									LCD_Clear(BLACK);
									adcinit();
									ADCConvert();
									while(rem == 10)
										{
								
										}
								}	
						}
				
	///////////////////////    //////////////////////////////// //////////////////////				
				
				if(rem == 2)			///when menu4 selected
						{
							subwindowframe(defaultvals.menu2);
										
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
						POINT_COLOR= defaultvals.textcolor;//defaultvals.textcolor1;  
						LCD_ShowString(defaultvals.submenupaddingv,line3,320,16,16, "CORE      ->ARM CORTEX M3");
						LCD_ShowString(defaultvals.submenupaddingv,line4,320,16,16, "MCU       ->STM32F103C8T6");
						LCD_ShowString(defaultvals.submenupaddingv,line5,320,16,16, "RAM/ROM   ->20K/128K");
						LCD_ShowString(defaultvals.submenupaddingv,line6,320,16,16, "SYSTEM CLK->72 MHz");
						LCD_ShowString(defaultvals.submenupaddingv,line7,320,16,16, "ADC RESOLUTION->12 bit");
						LCD_ShowString(defaultvals.submenupaddingv,line8,320,16,16, "DISPLAY   ->2.8 TFT SPI LCD");	
						LCD_ShowString(defaultvals.submenupaddingv,line9,320,16,16, "DISPLAY RESOLUTION->240*320");	
						
						LCD_ShowString(defaultvals.submenupaddingv,line11,320,16,16, "CORE TEMPERATURE:");	
						LCD_ShowString(defaultvals.submenupaddingv,line12,320,16,16, "SYSTEM UPTIME:");	
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


	
	
