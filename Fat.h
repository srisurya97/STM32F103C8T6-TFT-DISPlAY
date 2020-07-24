#ifndef FAT_H
#define FAT_H

#include"stm32f10x.h"

typedef struct 
	{
		uint8_t BS_jmpBoot[3];
		uint8_t BS_OEMName[8];
		uint16_t BPB_BytsPerSec;
		uint8_t BPB_SecPerClus;
		uint16_t BPB_RsvdSecCnt;
		uint8_t BPB_NumFATs;
		uint16_t BPB_RootEntCnt;
		uint32_t BPB_TotSec16;
		uint8_t BPB_Media;
		uint16_t BPB_FATSz16;
		//uint16_t BPB_SecPerTrk;
		//uint16_t BPB_NumHeads;
		uint32_t BPB_HiddSec;
		uint32_t BPB_TotSec32;
	}BootSectorFatCommon;

typedef struct 
	{	
		uint8_t BS_DrvNum; //Specific for Fat12_16 from here.
		//uint8_t BS_Reserved1;
		uint8_t BS_BootSig;
		uint32_t BS_VolID;
		uint8_t BS_VolLab[11];
		uint8_t BS_FilSysType[8];
		uint16_t RootDirSecs;
		uint32_t FirstDataSec;
		uint16_t FirstSecofClus;
	}BootSectorFat12_16;


typedef struct 
	{
		uint32_t BPB_FATSz32; //Specific for Fat32 from here.
		uint16_t BPB_ExtFlags;
		uint16_t BPB_FSVer;
		uint32_t BPB_RootClus;
		uint16_t BPB_FSInfo;
		uint16_t BPB_BkBootSec;
		uint64_t BPB_Reserved_1;
		uint32_t BPB_Reserved_2;
		uint8_t BS_DrvNum;
		//uint8_t BS_Reserved1;
		uint8_t BS_BootSig;
		uint32_t BS_VolID;
		uint8_t BS_VolLab[11];
		uint8_t BS_FilSysType[8];
	}BootSectorFat32;

typedef struct
	{
		uint8_t BootableFlag;
		//uint32_t CHSAddressStart;
		uint8_t  PartitionType;
		//uint32_t CHSAddressEnding;
		uint32_t LBAStart;
		uint32_t Size;
		uint16_t Signature;
	}READMBR;

typedef struct
	{
		uint8_t DOSFilename[8];
		uint8_t DOSFileExtension[3];
		uint8_t FileAttributes;
		//uint8_t NTCaseInfo;
		uint8_t CreateTime_ms;
		uint16_t CreateTime_HMS;
		uint16_t CreateDate;
		uint16_t LastAccessDate;
		//uint16_t FFStartCLus_High;
		uint16_t LastModTime;
		uint16_t LastModDate;
		//uint16_t FFStartCLus_Low;
		uint32_t FileSize;
		uint16_t PreviousDir;
		uint32_t StartClus;
	}DirStruct;

	
#define FAT_16 0x06
#define	FAT_16_LBA 0x0E	
#define NTFS   0x07
#define FAT_32_CHS 0x0B
#define	FAT_32_LBA 0x0C
	
#define MULBLOCK 0x01
#define SINGLEBLOCK 0x00
#define READY 0x00
	

	
//File Attributes	
#define READONLY 0x01
#define HIDDEN   0x02 
#define SYSTEM   0x04
#define VOLLABEL 0x08
#define DIRECTORY   0x10
#define ARCHIVE  0x20
#define LONGNAME 0x0F

#define ROOTDIR 0x00
#define SUBDIR 0x01
#define PREDIR 0x01
#define CURDIR 0x00
	
extern uint64_t LBSBaseAddress;	
//extern READMBR MBRStruct;
//extern BootSectorFatCommon FatCommon;
//extern BootSectorFat12_16 Fat16;
//extern BootSectorFat32 Fat32;
extern uint16_t SizeCount;
extern DirStruct *FolderPtr;

void SDDisplayonLCD(uint8_t *address,uint8_t M_BL);

void SDDisplayRaw(uint8_t *address,uint8_t M_BL);
uint8_t SDFATGetInfo(void);
void SDFatDisplayInfo(void);
void SDlocateDir (uint8_t);
uint8_t SDMBRRead(void);




#endif
