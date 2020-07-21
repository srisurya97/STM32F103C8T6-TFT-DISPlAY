#include "stm32f10x.h"
#include "lcd.h"
#include "delay.h"
#include "SDCARD.h"
#include "Fat.h"

////SDC/SDHC Ver2+
///Volatage 2.7---3.6

sdcardinfo sdcard1info;

uint64_t CID1, CID2;
uint64_t CSD1, CSD2;


void SDCARD_CS (uint8_t sdcs)
	{
		if(sdcs==1)
			{
				GPIOB->BSRR = GPIO_BSRR_BS12;
			}
			else
				{
					GPIOB->BRR = GPIO_BRR_BR12;
				}
	}

uint8_t SDCARDWrite(uint8_t sddata)
	{
		while((SPI2->SR & SPI_SR_TXE)==0);
		SPI2->DR = sddata;
		while((SPI2->SR & SPI_SR_RXNE)==1);
		while((SPI2->SR & SPI_SR_BSY)==1);
		return  SPI2->DR;
	}


void SPI2init (void)
	{
		RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; //Enabling SPI2 Clock
		RCC->APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPBEN; //Enabling ALTFUNC Clock

		//B13-SCK,B15-MOSI 
		GPIOB->CRH |= GPIO_CRH_MODE13 | GPIO_CRH_MODE15 | GPIO_CRH_CNF13_1 | GPIO_CRH_CNF15_1; //OUTPUT 50MHZ //ALT OUPUT PUSHPULL
		GPIOB->CRH &= ~(GPIO_CRH_CNF13_0 | GPIO_CRH_CNF15_0);

		//B14-MISO
		GPIOB->CRH &= ~ (GPIO_CRH_MODE14 | GPIO_CRH_CNF14_0); //INPUT MODE
		GPIOB->CRH |= GPIO_CRH_CNF14_1; //INPUT PUSHPULL

		//B12-NSS/CS
		GPIOB->CRH |= GPIO_CRH_MODE12; //OUTPUT 50MHZ
		GPIOB->CRH &= ~GPIO_CRH_CNF12; //GENERAL PURPOSE OUTPUT

		SPI2->CR2 |= SPI_CR2_TXEIE | SPI_CR2_RXNEIE;
		SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR  | SPI_CR1_SPE;

		SDCARD_CS(1);
	}
	

void SDCardDisplayinfo(void)
	{
		uint16_t j=20;
		uint16_t BLOCK_LEN=0,MULT=0;
		uint32_t capacity=0;
		uint8_t *tempdata;
		
		POINT_COLOR=WHITE;
		///Ver/Capacity//
		sdcard1info.CDSVerType = (0x03 & (uint8_t)(CSD1>>61)) ;
		if(sdcard1info.CDSVerType==0x00){ tempdata = "Ver2StandardCapacity";}
			else if(sdcard1info.CDSVerType==0x02){ tempdata = "Ver2HighCapacity";}
				else { tempdata = "Unknown Ver&Capacity";}
				LCD_ShowString(2,j,180,12,12,tempdata);
				
		j=j+20;
		///Speed///
		sdcard1info.CurrentSpeed = (uint8_t)(0x00000000000000FF & CSD1>>32);		
		if(sdcard1info.CurrentSpeed==0x32){tempdata = "Speed:25MBits/s"; }
			else if(sdcard1info.CurrentSpeed==0x5A) { tempdata = "Speed:50MBits/s";}		
				else { tempdata = "Speed:Unknown";}
				LCD_ShowString(2,j,140,12,12,tempdata);
				
		j=j+20;
		///ClassSupport//
		sdcard1info.ClassSupport = (uint16_t)(CSD1>>20) ;		
		for(uint8_t i=0;i<12;i++)
			{
				if(sdcard1info.ClassSupport & (0b0000000000000001<<i))
						{
							if(i==0){tempdata = "BasicCMD Supported";}
							if(i==2){tempdata = "Block Read";}
							if(i==4){tempdata = "Block Write";}
							if(i==5){tempdata = "Erase";}
							if(i==6){tempdata = "Write Protect";}
							if(i==7){tempdata = "Lock Card";}
							if(i==8){tempdata = "Card SpecificCMD";}
							if(i==9){tempdata = "I/O Mode";}
							if(i==10){tempdata = "Switch";}
							LCD_ShowString(2,j,140,12,12,tempdata);j=j+12;
						}
			}
			
		////Max ReadWWrite Block Len	
		j=j+8;
		//////////////////Card Manfacturer////////////			
		sdcard1info.MID=CID1>>56;			
		if(sdcard1info.MID == 0x01){ sdcard1info.MIDName= "Panasonic";}			
			//else if(sdcard1info.MID == 0x41){sdcard1info.MIDName="Toshiba";}//02
			//	else if(sdcard1info.MID == 0x03){sdcard1info.MIDName= "Sandisk";}			
					else if(sdcard1info.MID == 0x1b){sdcard1info.MIDName="Samsung";}			
						else if(sdcard1info.MID == 0x1d){sdcard1info.MIDName="AData";}			
							else if(sdcard1info.MID == 0x27){sdcard1info.MIDName="Pison";}			
								else if(sdcard1info.MID == 0x28){sdcard1info.MIDName="Lexer";}			
									else if(sdcard1info.MID == 0x31){sdcard1info.MIDName="Silicon Power";}			
										else if(sdcard1info.MID == 0x02 || sdcard1info.MID == 0x41 ){sdcard1info.MIDName="Kingstone";}		//41	
											else if(sdcard1info.MID == 0x74 || sdcard1info.MID == 0x03 || sdcard1info.MID == 0x82){sdcard1info.MIDName="Transcend";}			
												else if(sdcard1info.MID == 0x76){sdcard1info.MIDName="Patroit";}			
													//else if(sdcard1info.MID == 0x82){sdcard1info.MIDName="Sony";}			
														else if(sdcard1info.MID == 0x9C){sdcard1info.MIDName="AngelbirdV60/Hoodman";}			
															else {sdcard1info.MIDName="Unknown";}						
		LCD_ShowString(2,j,200,12,12,"Card Manufacturer:");
		LCD_ShowString(110,j,200,12,12,sdcard1info.MIDName); j=j+12;
			//////////////Block Lenght//////////////		
		j=j+8;
		sdcard1info.MaxReadBlockLen = (uint8_t)(0x000000000000000F & CSD1>>16);
		if(sdcard1info.MaxReadBlockLen==9){BLOCK_LEN=512;}		
			else if(sdcard1info.MaxReadBlockLen==10){BLOCK_LEN=1024;}		
				else if(sdcard1info.MaxReadBlockLen==11){BLOCK_LEN=2048;}		
		
		if(sdcard1info.MaxReadBlockLen==9){tempdata = "MaxR/W BLLen:512Bytes";}
			else if(sdcard1info.MaxReadBlockLen ==10){tempdata = "MaxR/W BLLen:1024Bytes";}
				else if(sdcard1info.MaxReadBlockLen ==11){tempdata = "MaxR/W BLLen:2048Bytes";}
					else {LCD_ShowString(2,j,140,12,12,"MaxReadLen:Unknown");}
					LCD_ShowString(2,j,140,12,12,tempdata);
					
		//////
		if(sdcard1info.CDSVerType==0x00)
			{			
				sdcard1info.C_SIZE_MUL = (uint8_t)(0x0000000000000007 & CSD2>>47);
			}
			else if(sdcard1info.CDSVerType==0x40)
				{
					///Not needed for ver2.0+
				}
		if(sdcard1info.C_SIZE_MUL ==0){MULT=4;}
			else if(sdcard1info.C_SIZE_MUL ==1){MULT=8;}
				else if(sdcard1info.C_SIZE_MUL ==2){MULT=16;}
					else if(sdcard1info.C_SIZE_MUL ==3){MULT=32;}
						else if(sdcard1info.C_SIZE_MUL ==4){MULT=64;}
							else if(sdcard1info.C_SIZE_MUL ==5){MULT=128;}
								else if(sdcard1info.C_SIZE_MUL ==6){MULT=256;}
									else if(sdcard1info.C_SIZE_MUL ==7){MULT=512;}		

		/////////////////////////SDCARD Capacity///////////
		j=j+12;
		if(sdcard1info.CDSVerType==0x00)
			{	
				sdcard1info.C_SIZE = (uint16_t)(((0x00000000000003FF & CSD1)<<2) | (0x0000000000000003 & CSD2>>61));
					
				capacity=(sdcard1info.C_SIZE+1)*MULT*BLOCK_LEN;				
				sdcard1info.CardSize=((capacity/1024)/1024);					
				LCD_ShowxNum(82,j,sdcard1info.CardSize,5,12,1);
				LCD_ShowString(2,j,180,12,12,"Card Capacity:     MB");	
			}
			else if(sdcard1info.CDSVerType==0x02)
			{
				sdcard1info.C_SIZE = (uint32_t)(((0x000000000000003F & CSD1)<<16) | (0x000000000000FFFF & (CSD2>>48)));
				capacity=(sdcard1info.C_SIZE+1)*BLOCK_LEN;				
				sdcard1info.CardSize=(capacity/1024)/1024;					
				LCD_ShowString(2,j,150,12,12,"Card Capacity:  .  GB");	
				LCD_ShowxNum(85,j,sdcard1info.CardSize,2,12,1);
				LCD_ShowxNum(102,j,((capacity*100/1024)/1024)-(sdcard1info.CardSize*100),2,12,1);	
			}	
			////////////////////
			j=j+12;	
			sdcard1info.misalignment = 0x07 & CSD1>>14;	
			if((sdcard1info.misalignment & 0x04) ==0x04)
				{
					//READ_BL_PARTIAL
					LCD_ShowString(2,j,150,12,12,"RD_BL_PR");	
					j=j+12;
				}
			if((sdcard1info.misalignment & 0x02)==0x02)
				{
					//WRITE_BL_MISALIGN
					LCD_ShowString(2,j,150,12,12,"WR_BL_ML");	
					j=j+12;
				}
			if((sdcard1info.misalignment & 0x01)==0x01)
				{
					//READ_BL_MISALIGN
					LCD_ShowString(2,j,150,12,12,"RD_BL_ML");	
					j=j+12;
				}
			
}	
	

uint8_t SDCMD(uint8_t CMD,uint32_t ARG,uint8_t CRC7)
{	
	SDCARDWrite(64+CMD);
	SDCARDWrite((uint8_t)(ARG>>24));
	SDCARDWrite((uint8_t)(ARG>>16));
	SDCARDWrite((uint8_t)(ARG>>8));
	SDCARDWrite((uint8_t)ARG);
	return SDCARDWrite(CRC7);
}
/*
void SDcheckstatus (void)
	{
		uint8_t returndata=0;
		SDCARD_CS(0); 
		returndata=SDCMD(13,0x00000000,0xFF);
		while(returndata == 0xFF)
		{
			returndata =SDCARDWrite(0xFF);
		}
		sdcard1info.CardStatus = SDCARDWrite(0x00);
		sdcard1info.CardStatus = (0xFF00 & sdcard1info.CardStatus<<8) | returndata; 		
		SDCARD_CS(1); 
	}*/
	

uint8_t SDCorrectWrittenBlocks(void) //Used in for writting data
{
	uint8_t returndata;
	SDCARDWrite(0x77);   /////CMD55
	SDCARDWrite(0x00);
	SDCARDWrite(0x00);
				//SDCARDWrite(0x00);
				//SDCARDWrite(0x00);
	returndata=SDCARDWrite(0xFF); 
	while(SDCARDWrite(0xFF) == 0xFF);
	
	SDCARDWrite(0x56);   //ACMD22
	SDCARDWrite(0x00);
	SDCARDWrite(0x00);
	SDCARDWrite(0x00);
	SDCARDWrite(0x00);
	returndata=SDCARDWrite(0xFF); 
	while(returndata == 0xFF)
			{
				returndata = SDCARDWrite(0xFF);
			}
	LCD_ShowxNum(1,300,returndata,3,12,1);		
	return returndata;
}	

uint8_t SDdataresponsetoken(uint8_t returndata) //Used in for writting data
{
	if((0x0F & returndata)== 0b00000101)
	{
		//LCD_ShowxNum(100,300,returndata,3,16,1);
		LCD_ShowString(180,300,60,12,12,"Done");
		return 0;
		
	}else if((0x0F & returndata)== 0b00001011)
		{
			LCD_ShowString(180,300,60,12,12,"CRC Er");
			SDCorrectWrittenBlocks();
			return 1;
		}else if((0x0F & returndata)== 0b00001101)
			{
				LCD_ShowString(180,300,60,12,12,"Write Er");
				SDCorrectWrittenBlocks();
				return 2;
			}
			else 
				{
					LCD_ShowString(180,300,60,12,12,"Unknown Er");
					SDCorrectWrittenBlocks();
					return 3;
				}
}


uint8_t SDreaddataerror(uint8_t returndata)
{
	if((0xFF & returndata)== 0x00)
		{
			LCD_ShowString(180,300,60,12,12,"Done");
			return NOERROR;
		}
		else if((0xFF & returndata)== 0x01)
			{
				LCD_ShowString(180,300,60,12,12,"Error");
				return 1;
			}
			else if((0xFF & returndata)== 0x02)
				{
					LCD_ShowString(180,300,60,12,12,"CC Error");
					return 2;
				}
				else if((0xFF & returndata)== 0x04)
					{
			  		LCD_ShowString(180,300,60,12,12,"ECC Fail");
				    return 3;
					}
					else if((returndata & 0xFF)== 0x08)
						{
					  	LCD_ShowString(180,300,60,12,12,"OutofRange");
							return 4;
						}
						else
							{
								LCD_ShowString(180,300,60,12,12,"Unknown");
								return 5;
							}
}


uint8_t SDStatusR1(uint8_t returndata)
{
	if((0x00 & returndata)== 0x00)
		{
			return NOERROR;
		}
		else if((0x01 & returndata)== 0x01)
			{
				LCD_ShowString(180,300,60,12,12,"Idle State");
				return CARDIDLE;
			}
			else if((0x02 & returndata)== 0x02)
				{
					LCD_ShowString(180,300,60,12,12,"Erase Reset");
					return 2;
				}
				else if((0x04 & returndata)== 0x04)
					{
						LCD_ShowString(180,300,60,12,12,"Illegal CMD");
						return 3;
					}
					else if((0x08 & returndata)== 0x08)
						{
							LCD_ShowString(180,300,60,12,12,"CRC Error");
							return 4;
						}
						else if((0x10 & returndata)== 0x10)
							{
								LCD_ShowString(180,300,60,12,12,"ErSeq Error");
						  	return 5;
							}
							else if ((0x20 & returndata)== 0x20)
								{
									LCD_ShowString(180,300,60,12,12,"Address Error");
									return 6;
								}
								else if ((0x40 & returndata)== 0x40)
									{
										LCD_ShowString(180,300,60,12,12,"Param Error");
										return 7;
									}
									else 
										{
											LCD_ShowString(180,300,60,12,12,"Multiple Error");
											return 8;
										}
}


uint16_t SDWritedata2b (uint8_t data[],uint8_t *address,uint16_t DataCRC)
{
	uint16_t x = 0;
	uint8_t returndata;
	SDCARD_CS(0);
	returndata = SDCMD(24,(uint32_t)address,0xFF);
	
	while(returndata==0xFF)
		{
			returndata=SDCARDWrite(0xFF);
		}
	if(SDStatusR1(returndata) == NOERROR )
		{
			returndata=SDCARDWrite(0xFE); //Data Start Byte
			for(x = 0 ; data[x] != 0x00; x++){
				returndata=SDCARDWrite(data[x]);
			}
			for(uint16_t y = 0 ; y < 512 - x ; y++){
				returndata=SDCARDWrite(0x00);
			}
			/*
			SDCARDWrite((uint8_t)DataCRC>>8); //CRC
			returndata=SDCARDWrite((uint8_t)DataCRC);	
			*/
			while(returndata == 0xFF)
				{
					returndata = SDCARDWrite(0xFF);
				}
			SDdataresponsetoken(returndata);		
			while(SDCARDWrite(0xFF) != 0xFF);				
		}
		else 
			{
				LCD_ShowString(180,300,60,12,12,"Busy");
			}
	SDCARD_CS(1);
			
		return x;
}



uint8_t SDreaddata2b (uint8_t *address,uint8_t sorm)
{
	uint8_t returndata=0;	
	if(sorm==0){ returndata = SDCMD(17,(uint32_t)address,0xFF);}	//CMD17 0x51  
	else{returndata = SDCMD(18,(uint32_t)address,0xFF);} //CMD 18 0x52
	
	while(returndata == 0xFF)	
		{
			returndata = SDCARDWrite(0xFF);
		}
	if(SDStatusR1(returndata)== NOERROR)
		{
			return NOERROR;
		}
	else { LCD_Clear(BLACK);  return SDreaddataerror(returndata); }
}


void SDCRCONOFF(uint8_t onoff)
{
	uint8_t returndata=0;
	SDCARD_CS(0);
	returndata=SDCMD(59,onoff,0x00); //CMD59
	while(returndata==0xFF)
		{
			returndata=SDCARDWrite(0xFF);
		}
	SDCARD_CS(1);	
}

void SDStopReadTransmit (void)
{
	SDCMD(12,0x00000000,0xFF);
	while(SDCARDWrite(0xFF) != 0x00);			
	for(uint16_t i = 0 ; i< 512; i++){SDCARDWrite(0xFF);}	
}


uint16_t SDCardBegin (void)
	{
		SPI2init();
		
		uint16_t i=0;
		uint8_t returndata=0,retry=0;
		sdcard1info.Debug = 0;
		//SPI2init();
		
		SDCARD_CS(1);
		for(i=0;i<12;i++){ SDCARDWrite(0xFF);	}
			
		SDCARD_CS(0);		
			
		returndata=SDCMD(0,0x00000000,0x95);
		while(returndata != CARDIDLE && retry <= 4 )
			{
				returndata=SDCARDWrite(0xFF);
				retry++;
			}		
		SDCARD_CS(1);	
			
		if(retry == 5)
			{
				POINT_COLOR = WHITE;
				LCD_ShowString(150,300,85,40,12,"SDCard Not Found!");	
				delay_ms(200);
				POINT_COLOR = BLACK;
				LCD_ShowString(150,300,85,40,12,"SDCard Not Found!");	
				POINT_COLOR = WHITE;
			}			
			else
				{
					retry = 0;	
					if(sdcard1info.Debug == 1)
						{		
							LCD_ShowString(200,20,50,16,16,"CMD0");	
						}
					SDCRCONOFF(0x00);		
					//////Next Command//////
					do
						{	
							SDCARD_CS(0);
							returndata=SDCMD(8,0x000001AA,0x87);
							while(returndata==0xFF)
								{
									returndata = SDCARDWrite(0xFF);			
								}	
						}while(returndata != CARDIDLE);
							
					SDCARD_CS(1);	
					if(sdcard1info.Debug == 1)
						{
							LCD_ShowString(200,40,50,16,16,"CMD8");	
						}
					///////Next Command////////					
					do
						{
							SDCARD_CS(0);
							SDCARDWrite(0x77);   /////CMD55
							SDCARDWrite(0x00);
							SDCARDWrite(0x00);
							//SDCARDWrite(0x00);
							//SDCARDWrite(0x00);
							returndata=SDCARDWrite(0xFF);
							while(returndata==0xFF)
								{
									returndata = SDCARDWrite(0xFF);
								}
							SDCARD_CS(1);
							if(sdcard1info.Debug == 1)
								{	
									LCD_ShowString(200,60,50,16,16,"CMD55");		
								}
							/////////Next Command/////
							SDCARD_CS(0); 
							returndata=SDCMD(41,0x40000000,0x77);
							while(returndata==0xFF)
								{
									returndata = SDCARDWrite(0xFF);
								}				
						}while(returndata!= CARDREADY);
						SDCARDWrite(0xFF);
						SDCARD_CS(1);
						if(sdcard1info.Debug == 1)
							{	
								LCD_ShowString(200,80,50,16,16,"AMD41");	
							}
						////////NExt Command//////////			
						SDCARD_CS(0);
						returndata=SDCMD(9,0x00000000,0x00); //CMD9
						while(returndata==0xFF)
							{
								returndata = SDCARDWrite(0xFF);
							}
						while(returndata==0x00){returndata = SDCARDWrite(0xFF);}
						while(returndata==0xFF){returndata = SDCARDWrite(0xFF);}
						while(returndata==0xFE){returndata = SDCARDWrite(0xFF);}			
						CSD1 = returndata;
						for(i=0;i<6;i++)
							{
								CSD1 = CSD1<<8;
								CSD1 |= SDCARDWrite(0xFF);
							}
						CSD1 = CSD1<<8;
						CSD1 |= SDCARDWrite(0xFF);
						CSD2=0;
						for(i=0;i<7;i++)
							{
								CSD2 |= SDCARDWrite(0xFF);
								CSD2=CSD2<<8;
							}
						CSD2 |= SDCARDWrite(0xFF);
						SDCARDWrite(0xFF);	
						SDCARD_CS(1);		
						if(sdcard1info.Debug == 1)
							{		
								LCD_ShowString(200,100,50,16,16,"CMD9");		
							}
						//////NextCMD////////////////
						SDCARD_CS(0);
						returndata=SDCMD(10,0x00000000,0x00);	
						while(returndata==0xFF)
							{
								returndata = SDCARDWrite(0xFF);
							}
						while(returndata==0x00){returndata = SDCARDWrite(0xFF);}
						while(returndata==0xFF){returndata = SDCARDWrite(0xFF);}
						while(returndata==0xFE){returndata = SDCARDWrite(0xFF);}		
						CID1 = returndata;
						for(i=0;i<6;i++)
							{
								CID1 = CID1<<8;
								CID1 |= SDCARDWrite(0xFF);
							}
						CID1 = CID1<<8;
						CID1 |= SDCARDWrite(0xFF);
						for(i=0;i<7;i++)
							{
								CID2 |= SDCARDWrite(0xFF);
								CID2=CID2<<8;
							}	
						CID2 |=  SDCARDWrite(0xFF);
						SDCARDWrite(0xFF);
									
						SDCARD_CS(1);
						if(sdcard1info.Debug == 1)
							{
								LCD_ShowString(200,120,50,16,16,"CMD10");		
							}
						/////CMD58//////////////
						SDCARD_CS(0);
						returndata=SDCMD(58,0x00000000,0x00);	//CMD58
						while(SDCARDWrite(0xFF) == 0xFF);
						while(returndata==0x00)
							{
								returndata = SDCARDWrite(0xFF);
							}
						sdcard1info.CCS = 0x01 & returndata>>6;
						returndata = SDCARDWrite(0xFF);	
						returndata = SDCARDWrite(0xFF);
						returndata = SDCARDWrite(0xFF);
						SDCARDWrite(0xFF);	
						SDCARD_CS(1);
						if(sdcard1info.Debug == 1)
							{
								LCD_ShowString(200,140,50,16,16,"CMD58");	
							}
							//////////Set Block length	
						SDCARD_CS(0);
						returndata=SDCMD(16,0x00000200,0xFF);	//CMD16 set to 512 BL_LEN
						while(SDCARDWrite(0xFF) != 0x00)
						SDCARDWrite(0xFF);
						
						SDCARD_CS(1);

						if(sdcard1info.Debug == 1)
							{
								LCD_ShowString(200,160,50,16,16,"CMD16");	
							}
						if(SDFATGetInfo() == 1){
						return 1;}
						return 0;
				}
}

