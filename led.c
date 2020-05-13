#include "led.h"
#include "spi.h"
#include "stm32f10x.h"

void LED_Init(void)
	{ 	
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;									  ///Enabling Clock for PORTA
		
		//LED PA-3  
		// General Purpose Output Pushpull
		//GPIOA->CRL &= ~GPIO_CRL_CNF3;													///Set to General pushpull
		//GPIOA->CRL |= GPIO_CRL_MODE3;     								    ///Set to Output 50Mhz Speed 
	
		//LED PB7
		GPIOB->CRL &= ~GPIO_CRL_CNF7;													///Set to General pushpull
		GPIOB->CRL |= GPIO_CRL_MODE7;     								    ///Set to Output 50Mhz Speed 
		GPIO_SetBit(GPIOB,GPIO_BSRR_BS7);
		
		//GPIO_SetBit(GPIOA,GPIO_BSRR_BS3);
 
	}
 
