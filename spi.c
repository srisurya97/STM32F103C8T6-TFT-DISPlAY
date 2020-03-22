#include "spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
//#include "stm32f10x_rcc.h"
//#include "stm32f10x_usart.h"
//#include "usart.h"

                         /////////////////Initilizing ports for SPI with TFT/////////////////
void SPI1_Init(void)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		SPI_InitTypeDef  SPI_InitStructure;
  
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;										   //Enabling Clock for PORTA
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;									  	 //Enabling Clock for PORTB
		RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;										   //Enabling Clock for AFIO
		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;										   //Enabling Clock for SPI1
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 ;	 //Enabling Pin_5 for sck | pin7 for MOSI
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  				 //Output mode Alternate PushPull  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				 //Speed 50MHz
		GPIO_Init(GPIOA, &GPIO_InitStructure);									 //Configuring with above settings
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  							 //Enabling Pin_4 PortA
   	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 			 //Speed 50MHz
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 				 //Output mode General PushPull  	
	  GPIO_Init(GPIOA, &GPIO_InitStructure);									 //Configuring with above settings
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);											 //Configure PA4 for LCD_CS. Defined in lcd.h for LCD_CS 

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  							 //Enabling Pin_1  PortB
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 			 //Speed 50MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 				 //Output mode General PushPull  		
    GPIO_Init(GPIOB, &GPIO_InitStructure); 									 //Configuring with above settings
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);											 //Configure PA1 for LCD_DC. Defined in lcd.h for LCD_DC

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;							   //Enabling Pin_0  PortB
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 			 //Speed 50MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 				 //Output mode General PushPull  		
    GPIO_Init(GPIOB, &GPIO_InitStructure);									 //Configuring with above settings
		GPIO_ResetBits(GPIOB, GPIO_Pin_0);											 //Configure PB0 for LCD_RST. Defined in lcd.h for LCD_RST


													/////////////////Initilizing ports the SPI1 for Display/////////////////
 
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI_Direction_1Line_Tx
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;												//SPI Master Mode
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;										//SPI Data SIze 8Bit
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;													//选择了串行时钟的稳态:时钟悬空高
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;												//数据捕获于第二个时钟沿
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;														//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
		SPI_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_2;		//SPI setting for 36MHz 
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;									//Setting for firstBit as MSB
		SPI_InitStructure.SPI_CRCPolynomial = 7;														//CRC
		SPI_Init(SPI1, &SPI_InitStructure);  																//Configuring with above settings
 
		SPI_Cmd(SPI1, ENABLE); 																							//SPI1 Enabled
	
		/*	SPI_BaudRatePrescaler_2      (SPI 36M@sys 72M)
				SPI_BaudRatePrescaler_8      (SPI 9M@sys 72M)
				SPI_BaudRatePrescaler_16     (SPI 4.5M@sys 72M)
				SPI_BaudRatePrescaler_256    (SPI 281.25K@sys 72M)  */
	
	}
	
	
	uint8_t SPI_WriteByte(SPI_TypeDef* SPIx,uint8_t Byte)
	{
		while((SPIx->SR&SPI_I2S_FLAG_TXE)==RESET);		
		SPIx->DR=Byte; 
		while((SPIx->SR&SPI_I2S_FLAG_RXNE)==RESET);
		return SPIx->DR;          	   
	}

										/////////////////Initilizing ports the SPI2 for Touch/////////////////
/*
void SPI2_Init(void)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;
		//RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
		RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能 	
 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15上拉

		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//定义波特率预分频的值:波特率预分频值为16
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
		SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
		SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
		SPI_Cmd(SPI2, ENABLE); //使能SPI外设
	
		SPI2_ReadWriteByte(0xff);//启动传输	 

	}   
	//SPI 速度设置函数
	//SpeedSet:
	//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
	//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
	//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
	//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)
  

                	
void SPI2_SetSpeed(uint8_t SpeedSet)
	{
		SPI2->CR1&=0XFFC7; 
		SPI2->CR1|=SpeedSet;
		SPI_Cmd(SPI2,ENABLE); 
	} 

	//SPIx 读写一个字节
	//TxData:要写入的字节
	//返回值:读取到的字节
uint8_t SPI2_ReadWriteByte(uint8_t TxData)
	{
		uint8_t retry=0;				 	
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)			//检查指定的SPI标志位设置与否:发送缓存空标志位
			{
				retry++;
				if(retry>200)return 0;
			}			  
		SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
		retry=0;

		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
			{
				retry++;
				if(retry>200)return 0;
			}	  						    
		return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据					    
	}          */
	





