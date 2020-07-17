#include "led.h"
#include "delay.h"
#include "lcd.h"
#include "spi.h"	
#include "LCDFunctions.h"
#include "stm32f10x.h"
#include "MenuFramework.h"
#include "keynav.h"
#include "ADC.h"
#include "images.h"
#include "PWM.h"
#include "SDCARD.h"

int main()
	{
		//uint32_t m=0;
		delayinit();
		delay_ms(100); //for proper lcd initialization
		SPI1_Init();
		LED_Init();			     
		LCD_Init();
		//if(SDCardBegin() == 1){
		//sdcard1info.mount = 1;}
		//PWMinit();
		//LCD_Set_Rotation(0);
		defaultvalsinit();
		splashdisplay();
		navsupport3key();
		while(1)
			{
				if (rem == 100)
					{
						LCD_Clear(BLACK);
						//defaultvalsinit();
						Displaymenu(); //MainMenu & selection & Navigation//
						rem = 0;
					}
				
				while(rem == 0)
					{
						checkkeys();
					// can be used for live tiles in home menu
					//livetiles();
					
					}
								
	////////////////				//Menu Selected//        //////////////////
				if(rem == 1)
						{		
							subwindowframe(defaultvals.menu1);
							POINT_COLOR=defaultvals.textcolor;
							LCD_ShowString(5,line3,240,16,16,(uint8_t *)"Mode 1 A5        Single Channel");
							LCD_ShowString(5,line4,239,16,16,(uint8_t *)"Uses: VoltMeter ");
							ONOFFSwitch(100,line3,0);
							POINT_COLOR=defaultvals.textcolor;
							LCD_ShowString(5,line6,240,16,16,(uint8_t *)"Mode 2 A5|A6     Dual Channel");
							LCD_ShowString(5,line7,239,16,16,(uint8_t *)"Uses: TransistorBias");
							ONOFFSwitch(100,line6,0);
							POINT_COLOR=defaultvals.textcolor;
							LCD_ShowString(5,line9,240,16,16,(uint8_t *)"Mode 3 A5|A6     Dual Channel");
							LCD_ShowString(5,line10,200,16,16,(uint8_t *)"Uses: Analog");
							ONOFFSwitch(100,line9,0);
							POINT_COLOR=defaultvals.textcolor;
							LCD_ShowString(5,line12,240,16,16,(uint8_t *)"Mode 4 A5        Single Channel");
							LCD_ShowString(5,line13,200,16,16,(uint8_t *)"Uses: Analog");
							ONOFFSwitch(100,line12,0);
							
							
							LCD_Fill(60,line15,157,line16,WHITE);
							POINT_COLOR=BLACK;
							LCD_ShowString(61,line15,159,16,12,(uint8_t *)"Turn ON to Start");	
							
							
							while( rem == 1)
								{
									checkkeys();
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
									checkkeys();
									//////////////////////////////////////////////
								}  
						}				
					
	///////////////////////   //////////////////   ////////////////				
				
				if (rem == 3)			///when menu3 selected
					{
						subwindowframe(defaultvals.menu3);
						if(sdcard1info.mount == 1)
						{
							SDlocateDir();
						}
						else{
							LCD_ShowString(50,150,240,16,16,"SDCard Not Mounted.");
						}
							//////////////Content Here///////////////
						
				    while( rem == 3)
							{ 
								checkkeys();
								//////////////////////////////////
							}	
						}
						
	/////////////// ///////////////////////////    //////////////////////////			
					if (rem == 4)      ///when menu1 selected 
					{
						subwindowframe(defaultvals.menu4); //Window frame for submenu 
						POINT_COLOR= defaultvals.textcolor;//defaultvals.textcolor1;  
						LCD_ShowString(defaultvals.submenupaddingv,line3,320,16,16, (uint8_t *)"CORE      ->ARM CORTEX M3");
						LCD_ShowString(defaultvals.submenupaddingv,line4,320,16,16, (uint8_t *)"MCU       ->STM32F103C8T6");
						LCD_ShowString(defaultvals.submenupaddingv,line5,320,16,16, (uint8_t *)"RAM/ROM   ->20K/128K");
						LCD_ShowString(defaultvals.submenupaddingv,line6,320,16,16, (uint8_t *)"SYSTEM CLK->72 MHz");
						LCD_ShowString(defaultvals.submenupaddingv,line7,320,16,16, (uint8_t *)"ADC RESOLUTION->12 bit");
						LCD_ShowString(defaultvals.submenupaddingv,line8,320,16,16, (uint8_t *)"DISPLAY   ->2.8 TFT SPI LCD");	
						LCD_ShowString(defaultvals.submenupaddingv,line9,320,16,16, (uint8_t *)"DISPLAY RESOLUTION->240*320");	
						LCD_ShowString(defaultvals.submenupaddingv,line10,320,16,16,(uint8_t *)"CORE TEMPERATURE:");	
						LCD_ShowString(defaultvals.submenupaddingv,line11,320,16,16, (uint8_t *)"SYSTEM UPTIME:");	
						LCD_ShowString(defaultvals.submenupaddingv,line12,320,16,16, (uint8_t *)"SCREEN ROTATION");
						LCD_ShowString(defaultvals.submenupaddingv,line13,320,16,16, (uint8_t *)"SDCard");
						
						if(lcddev.height==240 || lcddev.width==320)
							{
								ONOFFSwitch(140,line12,1);
							}
							else
								{
									ONOFFSwitch(140,line12,0);
								}
						
						if( sdcard1info.mount == 0 )
								{
									ONOFFSwitch(140,line13,0);
								}
								else 
									{
										ONOFFSwitch(140,line13,1);
									}				
								
							//////////////Content Here///////////////
						
						while( rem == 4)
							{
								checkkeys();
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
								checkkeys();
								////////////////////////////////
							}	
					}

						
	/////////////////// ////////////////// //////////////////////////					
				if(rem == 6)				///when menu5 selected
						{ 
							subwindowframe(defaultvals.menu6);

							

	
							while( rem == 6)
								{
									checkkeys();
									////////////////////////////////////
								}	
						}
						
		}
	
	
	}


	
	
