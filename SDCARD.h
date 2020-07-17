#ifndef SDCARD_H
#define SDCARD_H

#include "stm32f10x.h"

typedef struct
{
	uint8_t  CDSVerType;
	uint16_t AccessTime;
	uint16_t CurrentSpeed;
	uint16_t ClassSupport;
	uint8_t  MaxReadBlockLen;
	uint8_t misalignment;
	uint32_t C_SIZE;
	uint16_t Max_Current;
	uint8_t C_SIZE_MUL;
	uint8_t Erasesingleblock;
	uint8_t EraseSectorSize;
	uint16_t CardStatus;
	uint32_t CCS;
	uint32_t CardSize;
	uint8_t MID;
	uint8_t *MIDName;
	uint8_t Debug;
	uint8_t mount;
}sdcardinfo;

extern sdcardinfo sdcard1info;


#define CARDREADY 0x00
#define CARDIDLE 0x01
#define NOERROR 0x00


void SPI2init (void);
uint16_t SDCardBegin (void);
uint8_t SDCMD(uint8_t CMD,uint32_t ARG,uint8_t CRC7);
void SDCardDisplayinfo(void);
void SDCARD_CS(uint8_t sdcs);
void SDcheckstatus (void);
uint8_t SDStatusR1(uint8_t returndata);
uint8_t SDCARDWrite(uint8_t sddata);
uint8_t SDreaddata2b (uint8_t *address,uint8_t sorm);
uint16_t SDWritedata2b (uint8_t data[],uint8_t *address,uint16_t DataCRC);
uint8_t SDStatusR1(uint8_t returndata);
void SDStopReadTransmit (void);







#endif
