#include "stm32f10x.h"
#include "MenuFramework.h"
#include "keynav.h"
#include "lcd.h"
#include "delay.h"



//uint8_t nooption;

 void EXTI0_IRQHandler(void)
 {
	 if(EXTI->PR & EXTI_PR_PR0)EXTI->PR |= EXTI_PR_PR0;
	 delay_ms(250);
	 if(!(EXTI->PR & EXTI_PR_PR0)&&(rem == 0)) //Choose Next
			{
				move ++;
				if(move > MAX) 
					{
						move = MIN;
					/*	if(rem == 0) {nooption=0;}
						else if (rem == 1){nooption=1;}
							else if(rem == 2){nooption=1;}
								else if(rem == 3){nooption=1;}
									else if(rem == 4){nooption=1;}
										else if(rem == 5){nooption=1;}
											else if(rem == 6){nooption=1;}
										//|| 2 || 3 || 4 || 5 || 6)*/
					}
				menuchoose(move);
				
			}
	
		
	else { }	
		//delay_ms(400);				
		if(EXTI->PR & EXTI_PR_PR0)EXTI->PR |= EXTI_PR_PR0;
	}

 void EXTI1_IRQHandler(void)
 {
	 if(EXTI->PR & EXTI_PR_PR1)EXTI->PR |= EXTI_PR_PR1;
	 delay_ms(250);	
	  if (!(EXTI->PR & EXTI_PR_PR0)&&(rem == 0)) //Selecting the Submenu
			{
				rem= move;
				//if (rem == (1 || 2 || 3 || 4 || 5 || 6)) {nooption=1;}
				//break;
				
			}	
	 
		
		else { }	
   		
		if(EXTI->PR & EXTI_PR_PR1)EXTI->PR |= EXTI_PR_PR1;
	 
 }
 
 void EXTI2_IRQHandler(void)
 {
	if(EXTI->PR & EXTI_PR_PR2)EXTI->PR |= EXTI_PR_PR2;
  delay_ms(350);
	 
	 if((!(EXTI->PR & EXTI_PR_PR0))&&(rem == 0)) //Choose Previous
	   {
			 move = move - 1;
			 if(move < MIN) 
				 {
					 move = 1;
				 }
			 menuchoose(move);		//Chooses SubMenu	 
		 }	
		 
		else if ((!(EXTI->PR & EXTI_PR_PR0))&&(rem != 0 ))       //(1 || 2 || 3 ||4 || 5 || 6)
			{								
				testsuccessfull;
				rem = 100;		
			}

   else { }		
 
  if(EXTI->PR & EXTI_PR_PR2)EXTI->PR |= EXTI_PR_PR2;
 }


void interruptsettings(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;  //setting up alternate function for interrupts
	EXTI->RTSR |= (EXTI_RTSR_TR0 | EXTI_RTSR_TR1 | EXTI_RTSR_TR2); //rising edge trigger
	//EXTI->FTSR |= (EXTI_FTSR_TR0 | EXTI_FTSR_TR1 | EXTI_FTSR_TR2); //Falling edge
	AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PA; //setting up interrupt for lineport A0
	AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PA; //setting up interrupt for lineport A1
  AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PA;	//setting up interrupt for lineport A2	
}

void interruptenable(void)
{
	EXTI->IMR |= (EXTI_IMR_MR0 | EXTI_IMR_MR1 | EXTI_IMR_MR2); //enabling the interrupt for the line 0,1,2
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
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
		POINT_COLOR = RED;
	  LCD_ShowString(5,line20,320,16,16,"Interrupt");   //302
	  
	  interruptsettings();
		
	  
	  LCD_ShowString(5,line20,320,16,16,"         :");   //302
		
	  interruptenable();
	  
	  LCD_ShowString(5,line20,320,16,16,"          Enabled");   //302	
	  //delay_ms(500);
}





