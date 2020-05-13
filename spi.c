#include "spi.h"
#include "stm32f10x.h"

void GPIO_SetBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
   GPIOx->BSRR = GPIO_Pin;
}


void GPIO_ResetBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
   GPIOx->BRR = GPIO_Pin;
}
                         /////////////////ports settings for SPI with TFT & Initialization/////////////////
void SPI1_Init(void)
	{  
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;										///Enabling Clock for PORTA
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;									  ///Enabling Clock for PORTB
		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;										///Enabling Clock for SPI1
		RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
		
		AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;   //Disable jtag for remapping
		
		AFIO->MAPR |= AFIO_MAPR_SPI1_REMAP;
		///Remap---NSS/PA15, SCK/PB3, MISO/PB4, MOSI/PB5
		
		//////sck --> Pin A5 | MOSI  --> Pin A7 |
		//GPIOA->CRL |= GPIO_CRL_CNF5_1 | GPIO_CRL_CNF7_1;      ///Set to Alt pushpull  
		//GPIOA->CRL &= ~(GPIO_CRL_CNF5_0 | GPIO_CRL_CNF7_0);		///Set to Alt pushpull  
		//GPIOA->CRL |= GPIO_CRL_MODE5 | GPIO_CRL_MODE7;        ///Set to Output 50Mhz Speed 
		/////sck --> Pin B3 | MOSI  --> Pin B5 |
		GPIOB->CRL |= GPIO_CRL_CNF3_1 | GPIO_CRL_CNF5_1;      ///Set to Alt pushpull  
		GPIOB->CRL &= ~(GPIO_CRL_CNF3_0 | GPIO_CRL_CNF5_0);		///Set to Alt pushpull  
		GPIOB->CRL |= GPIO_CRL_MODE3 | GPIO_CRL_MODE5;        ///Set to Output 50Mhz Speed 
		
		/////MISO--->A6
		//GPIOA->CRL &= ~GPIO_CRL_CNF6_0; 											///Float input
		//GPIOA->CRL |= GPIO_CRL_CNF6_1;
		//GPIOA->CRL &= ~GPIO_CRL_MODE6;                        ///set to Input mode
		////MISO--->B4
		GPIOB->CRL &= ~GPIO_CRL_CNF4_0; 											///Float input
		GPIOB->CRL |= GPIO_CRL_CNF4_1;
		GPIOB->CRL &= ~GPIO_CRL_MODE4;                        ///set to Input mode		
				
		/////LCD_CS  ---> Pin A4
		//GPIOA->CRL &= ~GPIO_CRL_CNF4;													///Set to General pushpull
		//GPIOA->CRL |= GPIO_CRL_MODE4;     								    ///Set to Output 50Mhz Speed 
		//GPIO_ResetBit(GPIOA, GPIO_BRR_BR4);									    ///Configure PA4 for LCD_CS. Defined in lcd.h for LCD_CS
		/////NSS---->Pin A15
		GPIOA->CRH &= ~GPIO_CRH_CNF15;													///Set to General pushpull
		GPIOA->CRH |= GPIO_CRH_MODE15;     								    ///Set to Output 50Mhz Speed 
		GPIO_ResetBit(GPIOA, GPIO_BRR_BR15);									    ///Configure PA4 for LCD_CS. Defined in lcd.h for LCD_CS

		/////LCD_DC ---->B1
		//GPIOB->CRL &= ~GPIO_CRL_CNF1;													///Set to General pushpull
		//GPIOB->CRL |= GPIO_CRL_MODE1;     								    ///Set to Output 50Mhz Speed 
		//GPIO_ResetBit(GPIOB, GPIO_BRR_BR1);											///Configure PA1 for LCD_DC. Defined in lcd.h for LCD_DC
		////LCD_DC ----->B8
		GPIOB->CRH &= ~GPIO_CRH_CNF8;													///Set to General pushpull
		GPIOB->CRH |= GPIO_CRH_MODE8;     								    ///Set to Output 50Mhz Speed 
		GPIO_ResetBit(GPIOB, GPIO_BRR_BR8);											///Configure PA1 for LCD_DC. Defined in lcd.h for LCD_DC
		

		/////LCD_RST ----->B0
		//GPIOB->CRL &= ~GPIO_CRL_CNF0;													///Set to General pushpull
		//GPIOB->CRL |= GPIO_CRL_MODE0;     								    ///Set to Output 50Mhz Speed 
		//GPIO_ResetBit(GPIOB, GPIO_BRR_BR0);											///Configure PB0 for LCD_RST. Defined in lcd.h for LCD_RST
		////LCD _RST ------>B9
		GPIOB->CRH &= ~GPIO_CRH_CNF9;													///Set to General pushpull
		GPIOB->CRH |= GPIO_CRH_MODE9;     								    ///Set to Output 50Mhz Speed 
		GPIO_ResetBit(GPIOB, GPIO_BRR_BR9);											///Configure PB0 for LCD_RST. Defined in lcd.h for LCD_RST

													/////////////////Settings SPI1 for Display/////////////////
		//Baudrateprescaler-2@36Mhz/8bit data frame/MSBFIRST/SPI FullDuplex/clock idle low/rising edge data capture
		//SPI mastermode/Software slave NSS bit
		
		SPI1->CR1 |= ((uint16_t)0x0304);  //0x0307
		SPI1->CR2 |= ((uint16_t)0x00C0);  //Interrupt flag enable
		SPI1->CR1 |= ((uint16_t)0x0040);  //Enable SPI1 
		
		//	SPI_BaudRatePrescaler_2    (36M@sys 72M)
		//  SPI_BaudRatePrescaler_8    (9M)
		//	SPI_BaudRatePrescaler_16   (4.5M)
		//	SPI_BaudRatePrescaler_256  (281.25K)  
	}
	
	
	uint8_t SPI_WriteByte(SPI_TypeDef* SPIx,uint8_t Byte)
	{
		while((SPIx->SR&SPI_SR_TXE)==RESET);		
		SPIx->DR=Byte; 
		while((SPIx->SR&SPI_SR_RXNE)==RESET);
		return SPIx->DR;          	   
	}

