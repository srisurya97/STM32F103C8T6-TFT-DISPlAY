#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//Mini STM32
//SPI	   
//@ALIENTEK
//www.openedv.com
//2010/6/13 
//V1.0

////////////////////////////////////////////////////////////////////////////////// 	  
										  
void SPI2_Init(void);			 
void SPI2_SetSpeed(uint8_t SpeedSet);   
uint8_t SPI2_ReadWriteByte(uint8_t TxData);
uint8_t SPI_WriteByte(SPI_TypeDef* SPIx,uint8_t Byte);
void SPI1_Init(void);
 		 
#endif

