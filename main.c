#include "led.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "spi.h"	
#include "LCDFunctions.h"
#include "stm32f10x.h"

//#include "usart.h"	 
//#include "24cxx.h"
//#include "flash.h"
//#include "touch.h"
//#include "key.h"

uint8_t state=0;
void beepms(uint16_t va);
void xianshi(void);  
void refshow(void);

/*
void Load_Drow_Dialog(void)
	{
		LCD_Clear(WHITE);   
		POINT_COLOR=BLUE; 
		LCD_ShowString(lcddev.width-24, 0, 200, 16, 16, "RST");
  	POINT_COLOR=RED;
	}
	
void xianshi()
	{
		BACK_COLOR=WHITE;
		POINT_COLOR=RED;   
		showhanzi32(0,0,0);
		showhanzi32(40,0,1);
		showhanzi32(80,0,2);
		showhanzi16(0,35,0);
		showhanzi16(20,35,1);
		showhanzi16(40,35,2);
		showhanzi16(60,35,3);
		showhanzi16(80,35,4);
		showhanzi16(100,35,5); 
		LCD_ShowString(0,55,200,16,16, "2.8 TFT SPI 240*320");
	}

void showqq()
	{
		uint16_t x,y; 
		x=0;
		y=75;
		while(y<lcddev.height-39)
			{
				x=0;
				while(x<lcddev.width-39)
					{
						showimage(x,y);	
						x+=40;
					}
				y+=40;
			}	  
	}

void refshow(void)	
	{
		switch(state)
			{
				case 0:
					LCD_Clear(WHITE);
				  xianshi();
				 	showqq();
					break;
				case 1:
					LCD_Clear(BLACK);	
					break;
				case 2:
					LCD_Clear(RED);
					break;
				case 3:
					LCD_Clear(GREEN);
					break;
				case 4:
					LCD_Clear(BLUE);
					break;
			}	
	}


void jiance(uint8_t key) 
	{
		if(key==1)	
			{
				LCD_Clear(WHITE);
				TP_Adjust(); 
				TP_Save_Adjdata();	 
				Load_Drow_Dialog();
			}
		
		if(key==2)
			{
				state++;
				if(state==5)
					{
						state=0; 
					}
				refshow();
			}

	} 

void beepms(uint16_t va)
	{
		beep=1;
		delay_ms(va);
    beep=0;
	}
	
 */
 
int i=2; //Default Screen - Home
int MIN = 1; //Don't Change
int MAX = 7; //Number of Menus + 1
int Current = 2;
 

int main(void)
	{
		//PIN A0 for Nav and A1 for Back and A2 for Select
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
		//RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
		//PORT A PINS 2, 1 and 0 set to input pushpull
		GPIOA->CRL &= ~(GPIO_CRL_MODE0| GPIO_CRL_MODE1 | GPIO_CRL_MODE2);
		GPIOA->CRL |= (GPIO_CRL_CNF0_1|GPIO_CRL_CNF1_1 | GPIO_CRL_CNF2_1);
		GPIOA->CRL &= ~(GPIO_CRL_CNF0_0|GPIO_CRL_CNF1_0 |GPIO_CRL_CNF2_0);	
		//PORT B PINS 6 set to input pushpull
		GPIOB->CRL |= GPIO_CRL_CNF6_1;
		GPIOB->CRL &= GPIO_CRL_CNF6_0;

		//uint16_t i=0;	 
		//uint8_t key=0;	
		delay_init();	    
    LCDBegin();
		LCDSendString("D");
					
		NVIC_Configuration();
				
		LCDSendString("E");
				
		//uart_init(9600);	 
		LED_Init();			     
		//SPI_Flash_Init();
		//KEY_Init();	   	 
		
		LCDSendString("B");
				
		SPI1_Init();
		
		LCDSendString("U");
				
		LCD_Init();	
		//tp_dev.init(); 			
		POINT_COLOR=WHITE; 
		//xianshi();	   
		//showqq();	   
	 
		LCDSendString("GGING");
		delay_ms(1000);
		//LCD_Clear(BLACK);
		
		

		
		
	while(1)
			{
				
					while(i == 2)
						{ 
							POINT_COLOR=RED;
							LCD_DrawRectangle(1, 1, 170, 20);
							LCD_ShowString(2,2,320,16,16,  "HOME");
							POINT_COLOR=WHITE;
							LCD_ShowString(1,33,320,16,16, ">> DUMMY MENU 1          <<");
							LCD_ShowString(1,50,320,16,16, ">> DUMMY MENU 2          <<");
							LCD_ShowString(1,66,320,16,16, ">> DUMMY MENU 3          <<");
							LCD_ShowString(1,82,320,16,16, ">> SYSTEM SPECIFICATIONS <<");
							LCD_ShowString(1,98,320,16,16, ">> EXIT                  <<");
							
							POINT_COLOR=WHITE;
							Current = 2;
						  delay_ms(100);
							while( i == 2)
								{
									if(GPIOA -> IDR & GPIO_IDR_IDR0)
										{
											Current ++;
											delay_ms(300);
										}
										
									if( Current == 3)
											{ 
												//LCD_Clear(BLACK);
												//POINT_COLOR=RED;
												//LCD_DrawRectangle(1, 1, 170, 20);
												//LCD_ShowString(2,2,320,16,16,  "HOME");	
												POINT_COLOR=BLUE;
												LCD_ShowString(1,33,320,16,16, ">> DUMMY MENU 1          <<");
												POINT_COLOR=WHITE;
												LCD_ShowString(1,50,320,16,16, ">> DUMMY MENU 2          <<");
												LCD_ShowString(1,66,320,16,16, ">> DUMMY MENU 3          <<");
												LCD_ShowString(1,82,320,16,16, ">> SYSTEM SPECIFICATIONS <<");
												LCD_ShowString(1,98,320,16,16, ">> EXIT                  <<");				
											}
											else if( Current == 4)
												{
													//LCD_Clear(BLACK);
													//POINT_COLOR=RED;
													//LCD_DrawRectangle(1, 1, 170, 20);
													//LCD_ShowString(2,2,320,16,16,  "HOME");
													//POINT_COLOR=WHITE;
													LCD_ShowString(1,33,320,16,16, ">> DUMMY MENU 1          <<");
													POINT_COLOR=BLUE;
													LCD_ShowString(1,50,320,16,16, ">> DUMMY MENU 2          <<");
												  POINT_COLOR=WHITE;
													LCD_ShowString(1,66,320,16,16, ">> DUMMY MENU 3          <<");
													LCD_ShowString(1,82,320,16,16, ">> SYSTEM SPECIFICATIONS <<");
													LCD_ShowString(1,98,320,16,16, ">> EXIT                  <<");
												}
												else if( Current == 5)
													{
														//LCD_Clear(BLACK);
														//POINT_COLOR=RED;
														//LCD_DrawRectangle(1, 1, 170, 20);
														//LCD_ShowString(2,2,320,16,16,  "HOME");
														//POINT_COLOR=WHITE;
														LCD_ShowString(1,33,320,16,16, ">> DUMMY MENU 1          <<");
														LCD_ShowString(1,50,320,16,16, ">> DUMMY MENU 2          <<");
														POINT_COLOR=BLUE;
														LCD_ShowString(1,66,320,16,16, ">> DUMMY MENU 3          <<");
														POINT_COLOR=WHITE;	
														LCD_ShowString(1,82,320,16,16, ">> SYSTEM SPECIFICATIONS <<");
														LCD_ShowString(1,98,320,16,16, ">> EXIT                  <<");
													}
													else if( Current == 6)
														{
															//LCD_Clear(BLACK);
															//POINT_COLOR=RED;
															//LCD_DrawRectangle(1, 1, 170, 20);
															//LCD_ShowString(2,2,320,16,16,  "HOME");
															//POINT_COLOR=WHITE;
															LCD_ShowString(1,33,320,16,16, ">> DUMMY MENU 1          <<");
															LCD_ShowString(1,50,320,16,16, ">> DUMMY MENU 2          <<");
															LCD_ShowString(1,66,320,16,16, ">> DUMMY MENU 3          <<");
															POINT_COLOR=BLUE;
															LCD_ShowString(1,82,320,16,16, ">> SYSTEM SPECIFICATIONS <<");
															POINT_COLOR=WHITE;
															LCD_ShowString(1,98,320,16,16, ">> EXIT                  <<");
														}
														else if(Current == 7)
															{
																//LCD_Clear(BLACK);
																//POINT_COLOR=RED;
																//LCD_DrawRectangle(1, 1, 170, 20);
																//LCD_ShowString(2,2,320,16,16,  "HOME");
																//POINT_COLOR=WHITE;
																LCD_ShowString(1,33,320,16,16, ">> DUMMY MENU 1          <<");
																LCD_ShowString(1,50,320,16,16, ">> DUMMY MENU 2          <<");
																LCD_ShowString(1,66,320,16,16, ">> DUMMY MENU 3          <<");
																LCD_ShowString(1,82,320,16,16, ">> SYSTEM SPECIFICATIONS <<");
																POINT_COLOR=BLUE;
																LCD_ShowString(1,98,320,16,16, ">> EXIT                  <<");
																POINT_COLOR=WHITE;
															}
															else { Current = 3;}
																			
										if(GPIOA -> IDR & GPIO_IDR_IDR2)
												{	
													if(Current > MAX) 
														{
															Current = MAX; 
															i = Current; 
															LCD_Clear(BLACK);
														}
														else {
															i = Current;
															LCD_Clear(BLACK);
																	}
												}
											
		
									if(GPIOA -> IDR & GPIO_IDR_IDR1)
										{
											i = i - 1;
											if(i <= MIN) i = 2;
										}
								}
							}						
					
					while (i == 3)
						{
							POINT_COLOR=RED;
							LCD_DrawRectangle(1, 1, 170, 20);
							LCD_ShowString(2,2,320,16,16, "DUMMY MENU 1");
							POINT_COLOR=WHITE;
							
							POINT_COLOR=GREEN;
							LCD_ShowString(5,300,320,16,16,"<<< BACK");
							POINT_COLOR=WHITE;
							Current = 3;
							delay_ms(100);
							
							while( i == 3)
								{
									if(GPIOA -> IDR & GPIO_IDR_IDR1)
										{
											i = 2;
											LCD_Clear(BLACK);
										}	
								}	
							
						}
						
					while (i == 4)
						{
							POINT_COLOR=RED;
							LCD_DrawRectangle(1, 1, 170, 20);
							LCD_ShowString(2,2,320,16,16, "DUMMY MENU 2");
							POINT_COLOR=WHITE;
							
							POINT_COLOR=GREEN;
							LCD_ShowString(5,300,320,16,16,"<<< BACK");
							POINT_COLOR=WHITE;
							Current = 4;
							delay_ms(100);
							
							while( i == 4)
								{
									if(GPIOA -> IDR & GPIO_IDR_IDR1)
										{
											i = 2;
											LCD_Clear(BLACK);
										}	
								}	
							
						}

					while (i == 5)
						{
							POINT_COLOR=RED;
							LCD_DrawRectangle(1, 1, 170, 20);
							LCD_ShowString(2,2,320,16,16, "DUMMY MENU 3");
							POINT_COLOR=WHITE;
							
							POINT_COLOR=GREEN;
							LCD_ShowString(5,300,320,16,16,"<<< BACK");
							POINT_COLOR=WHITE;
							Current = 5;
							delay_ms(100);
							
							while( i == 5)
								{
									if(GPIOA -> IDR & GPIO_IDR_IDR1)
										{
											i = 2;
											LCD_Clear(BLACK);
										}	
								}	
							
						}
						
				
				while(i == 6)
						{
							POINT_COLOR=RED;
							LCD_DrawRectangle(1, 1, 170, 20);
							LCD_ShowString(2,2,320,16,16,  "SYSTEM SPECIFICATIONS");
							POINT_COLOR=WHITE;
							LCD_ShowString(1,33,320,16,16, "CORE       -> ARM CORTEX M3");
							LCD_ShowString(1,50,320,16,16, "MCU        -> STM32F103C8T6");
							LCD_ShowString(1,66,320,16,16, "RAM/ROM    -> 20K/64K");
							LCD_ShowString(1,82,320,16,16, "SYSTEM CLK -> 72 MHz");
							LCD_ShowString(1,98,320,16,16, "DISPLAY    -> 2.8 TFT SPI LCD");
							LCD_ShowString(1,114,320,16,16,"RESOLUTION -> 240*320");
							LCD_ShowString(1,130,320,16,16,"LCD 1602A  -> SIMPLE DEBUGGER");
				
							POINT_COLOR=GREEN;
							LCD_ShowString(5,300,320,16,16,"<<< BACK");
							POINT_COLOR=WHITE;
							Current = 6;
							delay_ms(100);
							
							while( i == 6)
								{
									if(GPIOA -> IDR & GPIO_IDR_IDR1)
										{
											i = 2;
											LCD_Clear(BLACK);
										}	
								}	
						}				
					
					
				while(i == 7)
						{ 
							POINT_COLOR=RED;
							LCD_DrawRectangle(1, 1, 170, 20);
							LCD_ShowString(2,2,320,16,16,  "SHUT DOWN");
							POINT_COLOR=BLUE;
							LCD_ShowString(1,33,320,16,16, "-> YES");
							LCD_ShowString(1,50,320,16,16, "-> NO");
							POINT_COLOR=GREEN;
							LCD_ShowString(5,300,320,16,16,"<<< BACK");
							POINT_COLOR=WHITE;
							
							Current = 7;
							delay_ms(100);
							while( i == 7)
								{
									if(GPIOA -> IDR & GPIO_IDR_IDR0)
										{
											i = i + 1;
											if(i > MAX) i = MAX;
										}
		
									if(GPIOA -> IDR & GPIO_IDR_IDR1)
										{
											i = 2;
											LCD_Clear(BLACK);
										}
								}	
						}

			

		} 
	 
	/* 
		while(1)
			{ 
				key=KEY_Scan(0);   
				tp_dev.scan(0); 
			  
				if(tp_dev.sta&TP_PRES_DOWN)
					{
						if(state!=1)
							{
								LCD_Clear(WHITE);
							} 		
						while(key==0)
							{
								key=KEY_Scan(0);
								tp_dev.scan(0);	
								if(tp_dev.sta&TP_PRES_DOWN)
									{
										if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)
											{
												if(tp_dev.x>(lcddev.width-24)&&tp_dev.y<16)Load_Drow_Dialog();
												else TP_Draw_Big_Point(tp_dev.x,tp_dev.y,RED);
						 		   		}
									}					
							} 			
						}
					
						
				jiance(key);
        i++;
				if(i==65000)
					{
						i=0;
						LED0=!LED0;
					}
			}	*/
	
	
	
	
	
   
	
	
	}
	
	
	

		
		
			
		


