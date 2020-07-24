#include "stm32f10x.h"
#include "MenuFramework.h"
#include "keynav.h"
#include "lcd.h"
#include "delay.h"
#include "ADC.h"
#include "SDCARD.h"
#include "Fat.h"

static uint8_t selectvals=2;

void interruptsettings(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;  //setting up alternate function for interrupts
	EXTI->RTSR |= (EXTI_RTSR_TR0 | EXTI_RTSR_TR1 | EXTI_RTSR_TR2); //rising edge trigger
	//AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PA; //setting up interrupt for lineport A0
	//AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PA; //setting up interrupt for lineport A1
  AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PA;	//setting up interrupt for lineport A2	
}

void interruptenable(void)
{
	EXTI->IMR |= (EXTI_IMR_MR0 | EXTI_IMR_MR1 | EXTI_IMR_MR2); //enabling the interrupt for the line 0,1,2
	//NVIC_EnableIRQ(EXTI0_IRQn);
	//NVIC_EnableIRQ(EXTI1_IRQn);
  NVIC_EnableIRQ(EXTI2_IRQn);
}

void navsupport3key(void)
{
	//PIN A0 for Nav and A1 for Back and A2 for Select //PORT A PINS 2, 1 and 0 set to input pushpull
		//RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
		//RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
		GPIOA->CRL &= ~(GPIO_CRL_MODE0| GPIO_CRL_MODE1 | GPIO_CRL_MODE2);
		GPIOA->CRL |= (GPIO_CRL_CNF0_1|GPIO_CRL_CNF1_1 | GPIO_CRL_CNF2_1);
		GPIOA->CRL &= ~(GPIO_CRL_CNF0_0|GPIO_CRL_CNF1_0 |GPIO_CRL_CNF2_0);
	
		//interrupt related
		interruptsettings();
		interruptenable();
}

void choosenext (void)
{		
	delay_ms(100);
	 if(rem == 0) //Choose Next
			{
				uint8_t MAX = 4;  //Max Number of Menus
				move ++;
				if(move > MAX) 
					{
						move = MAX;
					}
				menuchoose(move);
				
			}
			else if (rem == 1 )
				{
					uint8_t MAX = 6;  //Max Number of Menus
					selectvals++;
					if(selectvals>MAX){selectvals=MAX;}
					if(selectvals==1 || selectvals==2)
						{
							selectXmark(selectvals);
							ONOFFSwitch(100,line3,0);
							ONOFFSwitch(100,line6,0);
							ONOFFSwitch(100,line9,0);
							ONOFFSwitch(100,line12,0);
						}	
						else
							{
								menuchoose(selectvals+4);
							}
				}
				else if (((rem == 2 )||(rem == 5 )||(rem == 6 )))
					{
						selectvals=2;
						selectXmark(selectvals);
					}
					else if (rem == 3 ){
						uint8_t MAX = SizeCount + 1;  //Max Number of Menus							
						selectvals++;
						if(selectvals > MAX)selectvals= MAX;
						selectXmark(selectvals);
						displaydirnav(selectvals);
					}
					else if(rem == 4 )
						{
							uint8_t MAX = 4;  //Max Number of Menus							
							selectvals++;
							if(selectvals>MAX)selectvals=MAX;
							selectXmark(selectvals);
							menuchoose(selectvals + 10);
							
						}
						else
							{

							}		
}

void selectingmenu(void)
{
	delay_ms(100);
	  if (rem == 0)   //Selecting the Submenu 
			{
				//uint8_t MIN = 1;  //Don't Change
				//uint8_t MAX = 4;  //Max Number of Menus
				rem= move;
			}
			else if (rem == 1 )
				{
					if(selectvals==1)
						{
							ADC1->CR2 &= ~ADC_CR2_ADON;
							selectvals=2;
							rem=100;
						}
					if(selectvals==3)
						{
							mode = 0;
							rem = 7;
						}
					if(selectvals==4)
						{
							mode = 1;
							rem = 8;
						}
					if(selectvals==5)
						{
							mode = 2;
							rem = 9;
						}
					if(selectvals==6)
						{
							mode = 2;
							rem = 10;
						}
				}
				else if (((rem == 2 )||(rem == 5 )||(rem == 6 ))) 
					{
						if(selectvals==1)
							{
								selectvals=2;
								rem=100;
							}
					}
					else if (rem == 3 ) 
						{
							if(selectvals==1)
								{
									selectvals=2;
									rem=100;
									free(FolderPtr);
									FolderPtr = NULL;
								}
								else{
										SDlocateDir((selectvals - 2));
										selectvals = 3; 
										LCD_ShowxNum(120,0,selectvals-2,2,16,1);
								}
						}
						else if(rem == 4 )
							{
								if(selectvals==1) {selectvals=2; rem=100;}
								if(selectvals==3) 
									{
										if(lcddev.height==320 && lcddev.width==240)
											{
												LCD_Set_Rotation(1);
												selectvals=2;
												rem =100;
											}
											else
												{
													LCD_Set_Rotation(0);
													selectvals=2;
													rem =100;
												}
									}
								if(selectvals == 4)
									{
										if( sdcard1info.mount == 0 )
											{
												POINT_COLOR = WHITE;
												LCD_ShowString(50,300,220,12,12,"Mounting...");
												if(SDCardBegin() == 1)
													{
														sdcard1info.mount = 1;
														ONOFFSwitch(140,line13,1);
												
													}
											}
											else 
												{
													sdcard1info.mount = 0;
													ONOFFSwitch(140,line13,0);
												}									
										}
								POINT_COLOR = BLACK;
								LCD_ShowString(50,300,220,12,12,"Mounting...");
									
							}			
							else
								{

								}		

}

void chooseprevious(void)
{
	if(rem == 0)
		{
			uint8_t MIN = 1;  //Don't Change
			move = move - 1;
			if(move < MIN) 
				{
					move = 1;
				}
			menuchoose(move);		//Chooses SubMenu	 
		}
		else if (rem == 1 )
			{
				uint8_t MIN = 1;
				selectvals--;
				if(selectvals<MIN){selectvals=MIN;}				
    		if(selectvals==1 || selectvals==2)
					{
						selectXmark(selectvals);
						ONOFFSwitch(100,line3,0);
						ONOFFSwitch(100,line6,0);
						ONOFFSwitch(100,line9,0);
					}
					else
						{
							menuchoose(selectvals+4);
						}				
			}
			else if((rem == 7) || (rem == 8) || (rem == 9) || (rem == 10))
					{
						ADC1->CR2 &= ~ADC_CR2_ADON;
						selectvals=2;
						rem=1;
					}
					else if ((rem == 2 )||(rem == 5 )||(rem == 6 ))
							{
								selectvals=1;
								selectXmark(selectvals);
							}
					else if (rem == 3 ){
						uint8_t MIN = 1;  //Max Number of Menus							
						selectvals--;
						if(selectvals<MIN) selectvals=MIN;
						selectXmark(selectvals);
						displaydirnav(selectvals);
						
						}
						else if(rem == 4 )
						{
							uint8_t MIN = 1;  //Max Number of Menus							
							selectvals--;
							if(selectvals<MIN) selectvals=MIN;
							selectXmark(selectvals);
							menuchoose(selectvals+10);
							
						}
						else 
							{
					
						
							}		
}


void checkkeys(void) //For Polling Use
{
	if(GPIOA -> IDR & GPIO_IDR_IDR1){
		delay_ms(100);
		selectingmenu();
	}
	if(GPIOA -> IDR & GPIO_IDR_IDR0){
		delay_ms(100);
		choosenext();
	}
}

/*void EXTI0_IRQHandler(void)
 {
	 EXTI->PR |= EXTI_PR_PR0;
	 static uint8_t debounce=0;
	 debounce++;
	 if(debounce ==2) { debounce=0;	choosenext();}
 }*/

/* void EXTI1_IRQHandler(void)
 {
	 EXTI->PR |= EXTI_PR_PR1;
	 static uint8_t debounce=0;
	 debounce++;
	 if(debounce ==2)	{ debounce=0; selectingmenu();}
 }*/
 
 void EXTI2_IRQHandler(void)
 {
	EXTI->PR |= EXTI_PR_PR2;
	static uint8_t debounce=0;
	 debounce++;
	 if(debounce ==2)	{debounce=0; chooseprevious();}
 }






