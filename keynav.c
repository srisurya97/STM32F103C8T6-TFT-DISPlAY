#include "stm32f10x.h"
#include "MenuFramework.h"
#include "keynav.h"
#include "lcd.h"
#include "delay.h"

//uint8_t nooption;
static uint8_t selectvals=0;
uint8_t MIN = 1;  //Don't Change
uint8_t MAX = 4;  //Max Number of Menus

void interruptsettings(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;  //setting up alternate function for interrupts
	EXTI->RTSR |= (EXTI_RTSR_TR0 | EXTI_RTSR_TR1 | EXTI_RTSR_TR2); //rising edge trigger
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
		interruptsettings();
		interruptenable();
}

void choosenext (void)
{
	 delay_ms(250);
	 if((!(EXTI->PR & EXTI_PR_PR0))&&(rem == 0)) //Choose Next
			{
				move ++;
				if(move > MAX) 
					{
						move = MAX;
					}
				menuchoose(move);
				
			}
			else if ((!(EXTI->PR & EXTI_PR_PR0))&&((rem == 1 )||(rem == 2 )||(rem == 3 )||(rem == 4 )||(rem == 5 )||(rem == 6 )))
			{
				selectvals=0;
				if(selectvals==0)
				{
					selectXmark(selectvals);
				}
			}
	else { }		
}

void selectingmenu(void)
{
	delay_ms(250);	
	  if ((!(EXTI->PR & EXTI_PR_PR0))&&(rem == 0)) //Selecting the Submenu
			{
				rem= move;
			}	
		
			else if ((!(EXTI->PR & EXTI_PR_PR0))&&((rem == 1 )||(rem == 2 )||(rem == 3 )||(rem == 4 )||(rem == 5 )||(rem == 6 )))
				{
					if(selectvals==1)
					{
						rem=100;
						selectvals=0;
					}
				}
			
		else { }	
}

void chooseprevious(void)
{
	delay_ms(250);
	if((!(EXTI->PR & EXTI_PR_PR0))&&(rem == 0)) //Choose Previous
	   {
			 move = move - 1;
			 if(move < MIN) 
				 {
					 move = 1;
				 }
			 menuchoose(move);		//Chooses SubMenu	 
		 }	
		 
		else if ((!(EXTI->PR & EXTI_PR_PR0))&&((rem == 1 )||(rem == 2 )||(rem == 3 )||(rem == 4 )||(rem == 5 )||(rem == 6 )))
			{	
				selectvals=1;
				if(selectvals==1)
					{
						selectXmark(selectvals);
					}
			}
   else { }		
}

void EXTI0_IRQHandler(void)
 {
		EXTI->PR |= EXTI_PR_PR0;
		choosenext();
 }

 void EXTI1_IRQHandler(void)
 {
	 EXTI->PR |= EXTI_PR_PR1;
	 selectingmenu();
 }
 
 void EXTI2_IRQHandler(void)
 {
	EXTI->PR |= EXTI_PR_PR2;
	chooseprevious();
 }






