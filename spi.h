#ifndef __SPI_H
#define __SPI_H

#include"stm32f10x.h"

////////////////////////////////////////////////////////////////////////////////// 	  
void GPIO_SetBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint8_t SPI_WriteByte(SPI_TypeDef* SPIx,uint8_t Byte);
void SPI1_Init(void);
 		 
#endif

