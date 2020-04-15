#include "spi.h"


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
		
		//////sck --> Pin A5 | MOSI  --> Pin A7 |
		GPIOA->CRL |= GPIO_CRL_CNF5_1 | GPIO_CRL_CNF7_1;      ///Set to Alt pushpull  
		GPIOA->CRL &= ~(GPIO_CRL_CNF5_0 | GPIO_CRL_CNF7_0);		///Set to Alt pushpull  
		GPIOA->CRL |= GPIO_CRL_MODE5 | GPIO_CRL_MODE7;        ///Set to Output 50Mhz Speed 
		
		/////LCD_CS  ---> Pin A4
		GPIOA->CRL &= ~GPIO_CRL_CNF4;													///Set to General pushpull
		GPIOA->CRL |= GPIO_CRL_MODE4;     								    ///Set to Output 50Mhz Speed 
		GPIO_ResetBit(GPIOA, GPIO_Pin_4);									    ///Configure PA4 for LCD_CS. Defined in lcd.h for LCD_CS

		/////LCD_DC ---->B1
		GPIOB->CRL &= ~GPIO_CRL_CNF1;													///Set to General pushpull
		GPIOB->CRL |= GPIO_CRL_MODE1;     								    ///Set to Output 50Mhz Speed 
		GPIO_ResetBit(GPIOB, GPIO_Pin_1);											///Configure PA1 for LCD_DC. Defined in lcd.h for LCD_DC

		/////LCD_RST ----->B0
		GPIOB->CRL &= ~GPIO_CRL_CNF0;													///Set to General pushpull
		GPIOB->CRL |= GPIO_CRL_MODE0;     								    ///Set to Output 50Mhz Speed 
		GPIO_ResetBit(GPIOB, GPIO_Pin_0);											///Configure PB0 for LCD_RST. Defined in lcd.h for LCD_RST


													/////////////////Settings SPI1 for Display/////////////////
	
		//Baudrateprescaler/2@36Mhz  //8bit data frame //MSBFIRST //SPI FullDuplex
		//The clock polarity high when idle // 2nd clock transistion is the first data capture edge
		//SPI set to mastermode
		//Software slave management  NSS control with software		
		SPI1->CR1 |= ((uint16_t)0x0307);  //0x0307
		SPI1->CR2 |= ((uint16_t)0x00C0);  //Interrupt flag enable
		SPI1->CR1 |= ((uint16_t)0x0040);  //Enable SPI1 
		
		//SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC
		//	SPI_BaudRatePrescaler_2      (SPI 36M@sys 72M)
		//  SPI_BaudRatePrescaler_8      (SPI 9M@sys 72M)
		//	SPI_BaudRatePrescaler_16     (SPI 4.5M@sys 72M)
		//	SPI_BaudRatePrescaler_256    (SPI 281.25K@sys 72M)  
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
	





