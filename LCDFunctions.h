#ifndef LCDFunctions_H
#define LCDFunctions_H

#include <stdio.h>
#include "stm32f10x.h"

#define LCDD0PIN 12
#define LCDD0PORT GPIOB
#define LCDD1PIN 13
#define LCDD1PORT GPIOB
#define LCDD2PIN 14
#define LCDD2PORT GPIOB
#define LCDD3PIN 15
#define LCDD3PORT GPIOB
#define LCDD4PIN 8 
#define LCDD4PORT GPIOA
#define LCDD5PIN 9
#define LCDD5PORT GPIOA
#define LCDD6PIN 10
#define LCDD6PORT GPIOA
#define LCDD7PIN 11
#define LCDD7PORT GPIOA
#define LCDRSPIN 10
#define LCDRSPORT GPIOB
#define LCDRWPIN 11
#define LCDRWPORT GPIOB
#define LCDEnablePIN 8
#define LCDEnablePORT GPIOB
#define DelayBeforeEnable 100 //40000 //100ns
#define DelayAfterEnable 300//80000 //300ns    200ns


//Pinnumber address in mode config port
uint16_t pinpos[16] = {
	(0x00),
	(0x04),
	(0x08),
	(0x0C),
	(0x10),
	(0x14),
	(0x18),
	(0x1C),
	(0x00),
	(0x04),
	(0x08),
	(0x0C),
	(0x10),
	(0x14),
	(0x18),
	(0x1C),
};

void NotExactDelay(int delay)
{
	volatile int i;
	for(i=0; i < delay; i++);
}

void SetPinandPortForOutputPushPull(GPIO_TypeDef *port, int pinnumber)
	{
		//Enabling Port Clocks
		if(port == GPIOA)
			RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
		if(port == GPIOB)
			RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
		if(port == GPIOC)
			RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
		
		//output mode and pushpull
		if(pinnumber>8)
			{
				port->CRH |= (1<<pinpos[pinnumber] | 1<<(pinpos[pinnumber]+1));
				port->CRH &= ~(1<<(pinpos[pinnumber]+2) | 1<<(pinpos[pinnumber]+3));
			}
			else
				{
					port->CRH |= (1<<pinpos[pinnumber] | 1<<(pinpos[pinnumber]+1));
					port->CRH &= ~(1<<(pinpos[pinnumber]+2) | 1<<(pinpos[pinnumber]+3));
				}
	} 
	
void InitializingPortsForLCD()
	{
		SetPinandPortForOutputPushPull(LCDD0PORT, LCDD0PIN);
		SetPinandPortForOutputPushPull(LCDD1PORT, LCDD1PIN);
		SetPinandPortForOutputPushPull(LCDD2PORT, LCDD2PIN);
		SetPinandPortForOutputPushPull(LCDD3PORT, LCDD3PIN);
		SetPinandPortForOutputPushPull(LCDD4PORT, LCDD4PIN);
		SetPinandPortForOutputPushPull(LCDD5PORT, LCDD5PIN);
		SetPinandPortForOutputPushPull(LCDD6PORT, LCDD6PIN);
		SetPinandPortForOutputPushPull(LCDD7PORT, LCDD7PIN);
		SetPinandPortForOutputPushPull(LCDRWPORT, LCDRWPIN);
		SetPinandPortForOutputPushPull(LCDRSPORT, LCDRSPIN);
		SetPinandPortForOutputPushPull(LCDEnablePORT, LCDEnablePIN);
		
	}	
	
void SendBitToPortandPin(GPIO_TypeDef *port, int pinnumber, uint8_t bitstate)
	{
		if(bitstate)
			{
				port->BSRR = 1<<pinnumber;
			}
			else
				{
					port->BRR = 1<<pinnumber;
				}
	}
	
void LCDEnable()
	{
		NotExactDelay(DelayBeforeEnable);
		SendBitToPortandPin(LCDEnablePORT, LCDEnablePIN, 1);
	}	

void LCDSetToWrite()
	{
		SendBitToPortandPin(LCDRWPORT, LCDRWPIN, 0);
	}

void LCDSetToRead()
	{
		SendBitToPortandPin(LCDRWPORT, LCDRWPIN, 1);
	}

void LCDCommandMode()
	{
		SendBitToPortandPin(LCDRSPORT ,LCDRSPIN, 0);
		//NotExactDelay(10);
	}

void LCDCharacterMode()
	{
		SendBitToPortandPin(LCDRSPORT ,LCDRSPIN, 1);
	}


	
void LCDByte(char character)
	{
		NotExactDelay(240);
		SendBitToPortandPin(LCDD0PORT, LCDD0PIN, character & 0b00000001);
		SendBitToPortandPin(LCDD1PORT, LCDD1PIN, character & 0b00000010);
		SendBitToPortandPin(LCDD2PORT, LCDD2PIN, character & 0b00000100);
		SendBitToPortandPin(LCDD3PORT, LCDD3PIN, character & 0b00001000);
		SendBitToPortandPin(LCDD4PORT, LCDD4PIN, character & 0b00010000);
		SendBitToPortandPin(LCDD5PORT, LCDD5PIN, character & 0b00100000);
		SendBitToPortandPin(LCDD6PORT, LCDD6PIN, character & 0b01000000);
		SendBitToPortandPin(LCDD7PORT, LCDD7PIN, character & 0b10000000);
		//NotExactDelay(DelayAfterEnable);
		NotExactDelay(60); //60ns
		SendBitToPortandPin(LCDEnablePORT, LCDEnablePIN,0);
		NotExactDelay(10);
	}	
	
void LCDSendCharacter(char character)
	{
		LCDSetToWrite();
		LCDCharacterMode();
		LCDEnable();
		LCDByte(character);
		NotExactDelay(190); //200ns
	}


void LCDSendCommand(char command)
	{
		LCDSetToWrite();
		LCDCommandMode();
		LCDEnable();
		LCDByte(command);
		NotExactDelay(190); //200ns
	}
void LCDClear()
	{
		LCDSendCommand(0b00000001); //Clear Display
		NotExactDelay(8500);
	}

void LCDBegin()
	{	
		InitializingPortsForLCD();
		LCDSendCommand(0x0C); //0b00111000 Display ON, Cursor OFF
		LCDSendCommand(0x38); //0b00001110 8-bit Active
		LCDSendCommand(0x06);  //Shift Cursor Right 
		LCDSendCommand(0x80);  //Cursor To 1St Line
		LCDClear();
	}	
	
	

	
void LCDSendString(char *stringofcharacters)
{
	while(*stringofcharacters)
	{
		LCDSendCharacter(*stringofcharacters++);
	}
}	
	
void LCDSendInteger(int Integer ,uint8_t MaxLengthOfString)
{
		char StringNumber[MaxLengthOfString];
		sprintf(StringNumber, "%d", Integer);
		LCDSendString(StringNumber);
}

void LCDSendFloat(float floatnumber ,uint8_t MaxLengthOfString)
{
		char StringNumber[MaxLengthOfString];
		sprintf(StringNumber, "%f", floatnumber);
		LCDSendString(StringNumber);
}

//    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
//   64  65  66  67  68  69  70  71  72  73  74  75  76  77  78  79

void LCDCursorPositionBitCheck()
	{
		//int i=0;
		for(int Y=0;Y<=1;Y++)
		{
			for(int X=0;X<=16;X++)
			{
							LCDSendCommand(0x80 + X + (Y*64));
							LCDSendCharacter('X');	
							NotExactDelay(3000000);
							LCDClear();
			}
		}
	}

void LCDSetCursorPosition(int X, int Y)
	{
		if(X <= 16 && Y <= 1)
		{
			LCDSendCommand(0x80 + X + (Y*64));
		}
	}
	
	

#endif

