#include "stm32f10x.h"
#include "Fat.h"
#include "SDCARD.h"
#include "lcd.h"
#include "delay.h"
#include "MenuFramework.h"


BootSectorFatCommon FatCommon;
BootSectorFat12_16 Fat16;	
BootSectorFat32 Fat32;
READMBR MBRStruct;

uint64_t LBSBaseAddress;
uint64_t RootDirAddress;

uint16_t SizeCount = 0;

uint8_t SDMBRRead (void)
{
	uint32_t tempdata=0;
	uint8_t returndata=0;
	uint16_t retry=0;	
	
	SDCARD_CS(0);
	
	if(SDreaddata2b(0x00,SINGLEBLOCK) == READY)
		{	
			while(returndata != 0xFE && retry <= 2500 )
				{
					for(uint8_t i = 0 ;i<50 ; i++);
					returndata = SDCARDWrite(0xFF);
					retry ++;			
				}
			if(retry >=2500)
				{
					retry = 0;
					LCD_Clear(BLACK);
					LCD_ShowString(2,120,240,16,16,(uint8_t *)"Something Wrong With SDCard!");
					return 0;
				}
				else
					{	
						for(uint16_t Z=0;Z<446;Z++) //Boot Code
							{
								SDCARDWrite(0xFF);
							}
						MBRStruct.BootableFlag = SDCARDWrite(0xFF);		//Bootable flag
						
						//MBRStruct.CHSAddressStart = SDCARDWrite(0xFF);		//Starting CHS Address
						//tempdata = SDCARDWrite(0xFF);
						//MBRStruct.CHSAddressStart |= tempdata<<8;		
						//tempdata = SDCARDWrite(0xFF);
						//MBRStruct.CHSAddressStart |= tempdata<<16;		 
						SDCARDWrite(0xFF);
						SDCARDWrite(0xFF);
						SDCARDWrite(0xFF);		
							
						MBRStruct.PartitionType = SDCARDWrite(0xFF);		//Partition Type
					
						//MBRStruct.CHSAddressEnding = SDCARDWrite(0xFF);		//Ending CHS Address
						//tempdata = SDCARDWrite(0xFF);
						//MBRStruct.CHSAddressEnding |= tempdata<<8;
						//tempdata = SDCARDWrite(0xFF);
						//MBRStruct.CHSAddressEnding |= tempdata<<16;		
						SDCARDWrite(0xFF);
						SDCARDWrite(0xFF);							
						SDCARDWrite(0xFF);
						
						MBRStruct.LBAStart = SDCARDWrite(0xFF);		//LBA Address
						tempdata = SDCARDWrite(0xFF);
						MBRStruct.LBAStart |= tempdata<<8;
						tempdata = SDCARDWrite(0xFF);
						MBRStruct.LBAStart |= tempdata<<16;
						tempdata = SDCARDWrite(0xFF);
						MBRStruct.LBAStart |= tempdata<<24;
						
						MBRStruct.Size = SDCARDWrite(0xFF); //Size in Sector
						tempdata=SDCARDWrite(0xFF);
						MBRStruct.Size |= tempdata<<8;
						tempdata=SDCARDWrite(0xFF);
						MBRStruct.Size |= tempdata<<16;
						tempdata=SDCARDWrite(0xFF);
						MBRStruct.Size |= tempdata<<24;
						
						for(uint8_t p=0;p<48;p++)
							{
								SDCARDWrite(0xFF);
							}			
							
						MBRStruct.Signature = SDCARDWrite(0xFF); //signature
						tempdata = SDCARDWrite(0xFF);
						MBRStruct.Signature |= 0xFF00 & (tempdata<<8);
							
						for(uint8_t p=0;p<48;p++)
							{
								SDCARDWrite(0xFF);
							}				
						
						if(MBRStruct.PartitionType == FAT_16){ LBSBaseAddress = MBRStruct.LBAStart*512;}
						else if(MBRStruct.PartitionType == FAT_32_LBA || MBRStruct.PartitionType == FAT_32_CHS ){LBSBaseAddress = MBRStruct.LBAStart;}		
						else { }
					}
		}					
	SDCARD_CS(1);
	return 1;	
	
}


void SDFATGetInfo(void)
{
	uint16_t tempcode=0;
	uint8_t returndata=0,i=0;
	
	SDMBRRead();
	
	SDCARD_CS(0);
	for( ;i<16; i++){SDCARDWrite(0xFF);}
	if(SDreaddata2b((uint8_t *)LBSBaseAddress,SINGLEBLOCK) == READY)
		{
			while(returndata != 0xEB){
					  returndata = SDCARDWrite(0xFF);	}
				
			FatCommon.BS_jmpBoot[0]=returndata;	
			for(i=1;i<3;i++){
					FatCommon.BS_jmpBoot[i]=SDCARDWrite(0xFF); }
			for(i=0;i<8;i++){
					FatCommon.BS_OEMName[i]=SDCARDWrite(0xFF); }
			
			/////////////////	
			FatCommon.BPB_BytsPerSec = SDCARDWrite(0xFF);
			tempcode =SDCARDWrite(0xFF);		
			FatCommon.BPB_BytsPerSec |= 0xFF00 & (tempcode<<8);	
			////////////////
			FatCommon.BPB_SecPerClus=SDCARDWrite(0xFF);	
			///////////////
			FatCommon.BPB_RsvdSecCnt = SDCARDWrite(0xFF);
			tempcode = SDCARDWrite(0xFF);			
			FatCommon.BPB_RsvdSecCnt |= 0xFF00 & (tempcode<<8);
			////////////////
			FatCommon.BPB_NumFATs=SDCARDWrite(0xFF);	
			////////////////	
			FatCommon.BPB_RootEntCnt = SDCARDWrite(0xFF);
			tempcode = SDCARDWrite(0xFF);			
			FatCommon.BPB_RootEntCnt |= 0xFF00 & (tempcode<<8);
			//////////////////////////
			FatCommon.BPB_TotSec16 = SDCARDWrite(0xFF);
			tempcode = SDCARDWrite(0xFF);			
			FatCommon.BPB_TotSec16 |= 0xFF00 & (tempcode<<8);
			//////////////////////////////
			FatCommon.BPB_Media=SDCARDWrite(0xFF);
			////////////////////////////////	
			FatCommon.BPB_FATSz16 = SDCARDWrite(0xFF);
			tempcode =SDCARDWrite(0xFF);			
			FatCommon.BPB_FATSz16 |= 0xFF00 & (tempcode<<8);
			//////////////////////////////
			//FatCommon.BPB_SecPerTrk = SDCARDWrite(0xFF);
			//tempcode =SDCARDWrite(0xFF);		
			//FatCommon.BPB_SecPerTrk |= 0xFF00 & (tempcode<<8);
			SDCARDWrite(0xFF);
			SDCARDWrite(0xFF);
			///////////////////////////////
			//FatCommon.BPB_NumHeads = SDCARDWrite(0xFF);
			//tempcode =SDCARDWrite(0xFF);		
			//FatCommon.BPB_NumHeads |= 0xFF00 & (tempcode<<8);	
			SDCARDWrite(0xFF);
			SDCARDWrite(0xFF);
			//////////////////////////////
			FatCommon.BPB_HiddSec = SDCARDWrite(0xFF);
			tempcode = SDCARDWrite(0xFF);		
			FatCommon.BPB_HiddSec |= 0x0000FF00 & (tempcode<<8);	
			tempcode = SDCARDWrite(0xFF);		
			FatCommon.BPB_HiddSec |= 0x00FF0000 & (tempcode<<16);	
			tempcode = SDCARDWrite(0xFF);		
			FatCommon.BPB_HiddSec |= 0xFF000000 & (tempcode<<24);						
				
			FatCommon.BPB_TotSec32 = SDCARDWrite(0xFF);
			tempcode = SDCARDWrite(0xFF);		
			FatCommon.BPB_TotSec32 |= 0x0000FF00 & (tempcode<<8);	
			tempcode = SDCARDWrite(0xFF);		
			FatCommon.BPB_TotSec32 |= 0x00FF0000 & (tempcode<<16);	
			tempcode = SDCARDWrite(0xFF);		
			FatCommon.BPB_TotSec32 |= 0xFF000000 & (tempcode<<24);						
			
			if( MBRStruct.PartitionType == FAT_16 && FatCommon.BPB_TotSec16 == 0){
				 FatCommon.BPB_TotSec16 = FatCommon.BPB_TotSec32; }
			
			if(MBRStruct.PartitionType == FAT_16)  ////Fat16 Specific Struct
				{
					Fat16.BS_DrvNum=SDCARDWrite(0xFF);
					//Fat16.BS_Reserved1=SDCARDWrite(0xFF);
					SDCARDWrite(0xFF);
					Fat16.BS_BootSig=SDCARDWrite(0xFF);	
					
					Fat16.BS_VolID = SDCARDWrite(0xFF);
					tempcode = SDCARDWrite(0xFF);		
					Fat16.BS_VolID |= 0x0000FF00 & (tempcode<<8);	
					tempcode = SDCARDWrite(0xFF);		
					Fat16.BS_VolID |= 0x00FF0000 & (tempcode<<16);	
					tempcode = SDCARDWrite(0xFF);		
					Fat16.BS_VolID |= 0xFF000000 & (tempcode<<24);									
					
					for(i=0;i<11;i++){
							Fat16.BS_VolLab[i]=SDCARDWrite(0xFF);	}
					for(i=0;i<8;i++){
							Fat16.BS_FilSysType[i]=SDCARDWrite(0xFF);	}
	
					for(uint16_t p = 0; p<470;p++){
							SDCARDWrite(0xFF);}	
				}
				else if(MBRStruct.PartitionType == FAT_32_CHS || MBRStruct.PartitionType == FAT_32_LBA)
					{
						Fat32.BPB_FATSz32 = SDCARDWrite(0xFF);
						tempcode = SDCARDWrite(0xFF);		
						Fat32.BPB_FATSz32 |= 0x0000FF00 & (tempcode<<8);	
						tempcode = SDCARDWrite(0xFF);		
						Fat32.BPB_FATSz32 |= 0x00FF0000 & (tempcode<<16);	
						tempcode = SDCARDWrite(0xFF);		
						Fat32.BPB_FATSz32 |= 0xFF000000 & (tempcode<<24);						
						
						Fat32.BPB_ExtFlags = SDCARDWrite(0xFF);
						tempcode = SDCARDWrite(0xFF);		
						Fat32.BPB_ExtFlags |= 0x0000FF00 & (tempcode<<8);	
						
						Fat32.BPB_FSVer = SDCARDWrite(0xFF);
						tempcode = SDCARDWrite(0xFF);		
						Fat32.BPB_FSVer |= 0x0000FF00 & (tempcode<<8);	
						
						Fat32.BPB_RootClus = SDCARDWrite(0xFF);
						tempcode = SDCARDWrite(0xFF);		
						Fat32.BPB_RootClus |= 0x0000FF00 & (tempcode<<8);	
						tempcode = SDCARDWrite(0xFF);		
						Fat32.BPB_RootClus |= 0x00FF0000 & (tempcode<<16);	
						tempcode = SDCARDWrite(0xFF);		
						Fat32.BPB_RootClus |= 0xFF000000 & (tempcode<<24);						
						
						Fat32.BPB_FSInfo = SDCARDWrite(0xFF);
						tempcode = SDCARDWrite(0xFF);		
						Fat32.BPB_FSInfo |= 0x0000FF00 & (tempcode<<8);	
						
						Fat32.BPB_BkBootSec = SDCARDWrite(0xFF);
						tempcode = SDCARDWrite(0xFF);		
						Fat32.BPB_BkBootSec |= 0x0000FF00 & (tempcode<<8);
							
						for(uint8_t i = 0;i<12;i++){
								SDCARDWrite(0xFF); }
						
						Fat32.BS_DrvNum = SDCARDWrite(0xFF);	
						//Fat32.BS_Reserved1 = SDCARDWrite(0xFF);
						SDCARDWrite(0xFF);	
						Fat32.BS_BootSig = SDCARDWrite(0xFF);
						
						Fat32.BS_VolID = SDCARDWrite(0xFF);
						tempcode = SDCARDWrite(0xFF);		
						Fat32.BS_VolID |= 0x0000FF00 & (tempcode<<8);	
						tempcode = SDCARDWrite(0xFF);		
						Fat32.BS_VolID |= 0x00FF0000 & (tempcode<<16);	
						tempcode = SDCARDWrite(0xFF);		
						Fat32.BS_VolID |= 0xFF000000 & (tempcode<<24);									
							
						for(i=0;i<11;i++){
								Fat32.BS_VolLab[i]=SDCARDWrite(0xFF);	}
						for(i=0;i<8;i++){
								Fat32.BS_FilSysType[i]=SDCARDWrite(0xFF);	}
	
						for(uint16_t p = 0; p<470;p++){
								SDCARDWrite(0xFF);}	
				}
		}
	SDCARD_CS(1);
	
	if(MBRStruct.PartitionType == FAT_16){
			RootDirAddress= LBSBaseAddress+((FatCommon.BPB_BytsPerSec*FatCommon.BPB_RsvdSecCnt)+(FatCommon.BPB_NumFATs*FatCommon.BPB_FATSz16*FatCommon.BPB_BytsPerSec)); }
			else if(MBRStruct.PartitionType == FAT_32_CHS || MBRStruct.PartitionType == FAT_32_LBA){
					RootDirAddress= LBSBaseAddress+ (Fat32.BPB_FATSz32*FatCommon.BPB_NumFATs)+FatCommon.BPB_RsvdSecCnt ;
					}else{ LCD_ShowString(2,2,240,16,16,(uint8_t *)"Unsupported FileSystem."); }	

}


void SDFatDisplayInfo(void)
{
	uint16_t j=2;
	LCD_ShowString(2,j,240,12,12,(uint8_t *)"OEMName:");
	LCD_ShowString(80,j,240,12,12,FatCommon.BS_OEMName);
	j=j+15;
	LCD_ShowString(2,j,240,12,12,(uint8_t *)"BytesPerSec:");
	LCD_ShowxNum(80,j,FatCommon.BPB_BytsPerSec,3,12,1);
	j=j+15;
	LCD_ShowString(2,j,240,12,12,(uint8_t *)"SecPerClus");
	LCD_ShowxNum(80,j,FatCommon.BPB_SecPerClus,3,12,1);
	j=j+15;
	LCD_ShowString(2,j,240,12,12,(uint8_t *)"MxRootEntCnt:");
	LCD_ShowxNum(80,j,FatCommon.BPB_RootEntCnt,3,12,1);	
	j=j+15;
	LCD_ShowString(2,j,240,12,12,(uint8_t *)"NumFATs:");
	LCD_ShowxNum(80,j,FatCommon.BPB_NumFATs,3,12,1);	
	j=j+15;
	LCD_ShowString(2,j,240,12,12,(uint8_t *)"RsvdSecCnt:");
	LCD_ShowxNum(80,j,FatCommon.BPB_RsvdSecCnt,3,12,1);
	j=j+15;
	if(MBRStruct.PartitionType == FAT_16)
		{	
			LCD_ShowString(2,j,240,12,12,(uint8_t *)"FATSz16:");
			LCD_ShowxNum(80,j,FatCommon.BPB_FATSz16,3,12,1);			
			j=j+15;
			
			LCD_ShowString(2,j,240,12,12,(uint8_t *)"TotalSector16:");
			LCD_ShowxNum(80,j,FatCommon.BPB_TotSec16,10,12,1);
			j=j+15;
			
			LCD_ShowString(2,j,80,12,12,(uint8_t *)"VolLab:");
			LCD_ShowString(80,j,65,12,12,Fat16.BS_VolLab);
			j=j+15;
			LCD_ShowString(2,j,240,12,12,(uint8_t *)"FilSysType:");
			LCD_ShowString(80,j,240,12,12,Fat16.BS_FilSysType);			
			j=j+15;
		}
		else
			{ 
				LCD_ShowString(2,j,240,12,12,(uint8_t *)"FATSz32:");
				LCD_ShowxNum(80,j,Fat32.BPB_FATSz32,3,12,1);
				j=j+15;
				LCD_ShowString(2,j,240,12,12,(uint8_t *)"TotalSector32:");
				LCD_ShowxNum(80,j,FatCommon.BPB_TotSec32,10,12,1);
				j=j+15;
				LCD_ShowString(2,j,240,12,12,(uint8_t *)"RootClus:");
				LCD_ShowxNum(80,j,Fat32.BPB_RootClus,10,12,1);
				j=j+15;
				LCD_ShowString(2,j,80,12,12,(uint8_t *)"VolLab:");
				LCD_ShowString(80,j,65,12,12,Fat32.BS_VolLab);
				j=j+15;
				LCD_ShowString(2,j,240,12,12,(uint8_t *)"FilSysType:");
				LCD_ShowString(80,j,240,12,12,Fat32.BS_FilSysType);			
				j=j+15;
			}
	
	j=j+15;		
	LCD_ShowString(2,j,240,12,12,(uint8_t *)"Bootable FLag:");
	LCD_ShowxNum(120,j,MBRStruct.BootableFlag,8,12,1);
	j=j+15;
	//LCD_ShowString(2,j,240,12,12,(uint8_t *)"CHS Start Sector:");
	//LCD_ShowxNum(120,j,(uint8_t)(0x000000FF & (MBRStruct.CHSAddressStart>>8)),8,12,1);	
	//j=j+15;
	LCD_ShowString(2,j,240,12,12,(uint8_t *)"Partition Type:");
	LCD_ShowxNum(120,j,MBRStruct.PartitionType,8,12,1);			
	j=j+15;
	//LCD_ShowString(2,j,240,12,12,(uint8_t *)"CHS End Sector:");
	//LCD_ShowxNum(120,j,(0x000000FF & (MBRStruct.CHSAddressEnding>>8)),8,12,1);	
	//j=j+15;
	LCD_ShowString(2,j,240,12,12,(uint8_t *)"LBA Addrs:");
	LCD_ShowxNum(80,j,(uint8_t)(MBRStruct.LBAStart>>24),3,12,1);	
	LCD_ShowxNum(110,j,(uint8_t)(MBRStruct.LBAStart>>16),3,12,1);	
	LCD_ShowxNum(140,j,(uint8_t)(MBRStruct.LBAStart>>8),3,12,1);	
	LCD_ShowxNum(170,j,(uint8_t)(MBRStruct.LBAStart),3,12,1);			
	j=j+15;
	LCD_ShowString(2,j,240,12,12,(uint8_t *)"Par1Size(in Sector):");
	LCD_ShowxNum(120,j,MBRStruct.Size,8,12,1);							
	j=j+15;
	//LCD_ShowString(2,j,240,12,12,(uint8_t *)"Signature:");
	//LCD_ShowxNum(120,j,(uint8_t)MBRStruct.Signature ,8,12,1);			
	//LCD_ShowxNum(150,j,(uint8_t)(MBRStruct.Signature>>8),8,12,1);			
	//j=j+15;
	LCD_ShowString(2,j,240,12,12,(uint8_t *)(uint8_t *)"BaseAddress:");
	LCD_ShowxNum(120,j,LBSBaseAddress ,8,12,1);					
	
}


void SDDisplayonLCD(uint8_t *address, uint8_t M_BL)
{
	uint16_t u=0,v=0;
	uint8_t returndata=0;
	SDCARD_CS(0);
	if( SDreaddata2b(address,M_BL) == READY)
		{
			while(SDCARDWrite(0xFF) == 0xFE);
			returndata=SDCARDWrite(0xFF);							
			LCD_Clear(BLACK);			
			for(v=2;v<305;)
				{
					for(u= 2;u<228; )
						{
							if(returndata==0x00){ 
								 returndata=SDCARDWrite(0xFF); }
								else
									{
											LCD_ShowChar(u,v,returndata,12,1);
											returndata=SDCARDWrite(0xFF);						
											u += 7;
									}
						}	
					if(u>228){u=0;}
					v=v+13;
				}
		}
	if(M_BL == MULBLOCK ){	
			returndata=SDCMD(12,0x00000000,0xFF);
			while(returndata != 0x00){returndata = SDCARDWrite(0xFF);}			
		}
	
	SDCARD_CS(1);
}

void SDDisplayRaw(uint8_t *address,uint8_t M_BL)
{
	uint16_t u=0,v=0;
	uint8_t returndata=0;
	SDCARD_CS(0);

	if(SDreaddata2b(address,M_BL) == READY)
		{
			while(SDCARDWrite(0xFF) == 0xFE);				
			LCD_Clear(BLACK);			
			for(v=2;v<305;)
				{
					for(u= 2;u<228; )
						{
							returndata=SDCARDWrite(0xFF);	
							//if(returndata == 0x00 || returndata == 0xFF ||returndata == 0xFE){returndata=SDCARDWrite(0xFF);}
							LCD_ShowxNum(u,v,returndata,3,12,1);
							u=u+24;
						}
					if(u>228){u=0;}
					v=v+13;
					if(M_BL == MULBLOCK){
							if(v>=305){
								delay_ms(3000); v=2; u=2; LCD_Clear(BLACK); }
						}
				}
		}
	
	if(M_BL == MULBLOCK)
		{	
			returndata=SDCMD(12,0x00000000,0xFF);
			while(returndata!=0x00){returndata=SDCARDWrite(0xFF);}			
			for(uint16_t i = 0 ; i< 512; i++){SDCARDWrite(0xFF);}	
		}	
		
	SDCARD_CS(1);
}


DirStruct SDRecordEntries (void)
{	
	DirStruct TempFile;
	uint64_t tempcode;
	
	for(uint8_t i = 0;i<8;i++)
			{
				TempFile.DOSFilename[i]  = SDCARDWrite(0xFF);
			}
			
	for(uint8_t i = 0 ;i<3 ; i++)
			{
				TempFile.DOSFileExtension[i]  = SDCARDWrite(0xFF);
			}
			
	TempFile.FileAttributes = SDCARDWrite(0xFF);
	//TempFile.NTCaseInfo = SDCARDWrite(0xFF);
	SDCARDWrite(0xFF);			
	TempFile.CreateTime_ms = SDCARDWrite(0xFF);
				
	TempFile.CreateTime_HMS = SDCARDWrite(0xFF);	
	tempcode = SDCARDWrite(0xFF);	
	TempFile.CreateTime_HMS |= 0x0000FF00 & (tempcode << 8);
			
	TempFile.CreateDate = SDCARDWrite(0xFF);
	tempcode = SDCARDWrite(0xFF);
	TempFile.CreateDate |= 0x0000FF00 & (tempcode << 8);
				
	TempFile.LastAccessDate = SDCARDWrite(0xFF);	
	tempcode = SDCARDWrite(0xFF);	
	TempFile.LastAccessDate |= 0x0000FF00 & (tempcode << 8);
				
	TempFile.StartClus = SDCARDWrite(0xFF);
	tempcode = SDCARDWrite(0xFF);	
	TempFile.StartClus |= 0x0000FF00 & (tempcode << 8);
	if(MBRStruct.PartitionType == FAT_16_LBA || MBRStruct.PartitionType == FAT_16){ TempFile.StartClus = 0; } 
	
	TempFile.LastModTime = SDCARDWrite(0xFF);	
	tempcode = SDCARDWrite(0xFF);	
	TempFile.LastModTime |= 0x0000FF00 & (tempcode << 8);			
			
	TempFile.LastModDate = SDCARDWrite(0xFF);	
	tempcode = SDCARDWrite(0xFF);	
	TempFile.LastModDate |= 0x0000FF00 & (tempcode << 8);			
			
	TempFile.StartClus = TempFile.StartClus<<16; 
	TempFile.StartClus |= SDCARDWrite(0xFF);
	tempcode = SDCARDWrite(0xFF);	
	TempFile.StartClus |= 0x0000FF00 & (tempcode << 8);	
		
	TempFile.FileSize = SDCARDWrite(0xFF);	
	tempcode = SDCARDWrite(0xFF);	
	TempFile.FileSize |= 0x0000FF00 & (tempcode << 8);			
	tempcode = SDCARDWrite(0xFF);	
	TempFile.FileSize |= 0x00FF0000 & (tempcode << 16);			
	tempcode = SDCARDWrite(0xFF);	
	TempFile.FileSize |= 0xFF000000 & (tempcode << 24);					
		
	return TempFile;	
}


//#define FileLocation  RootDirAddress + (FatCommon.BPB_RootEntCnt*32 ) + ((File->StartClus - 2) * FatCommon.BPB_SecPerClus * FatCommon.BPB_BytsPerSec)

void OpenAsTXTFile ( DirStruct *File)
{
	SDCARD_CS(0);
	
	uint16_t u=0,v=25;
	uint8_t returndata=0;
	uint32_t Sizecount=0, FileLocation;
	
	if(MBRStruct.PartitionType == FAT_32_LBA || MBRStruct.PartitionType == FAT_32_CHS){FatCommon.BPB_BytsPerSec = 1;}
	FileLocation = RootDirAddress + (FatCommon.BPB_RootEntCnt * 32 ) + ((File->StartClus - 2) * FatCommon.BPB_SecPerClus * FatCommon.BPB_BytsPerSec);
			
	if( SDreaddata2b((uint8_t *)FileLocation,File->FileSize*0.00195) == READY)
		{ 
			while(SDCARDWrite(0xFF) != 0xFE);
			returndata = SDCARDWrite(0xFF);
			subwindowframe("TXT File");
			//LCD_Clear(BLACK);		
			//while(Sizecount < File->FileSize)
			while(returndata != 0x00)
				{
					Sizecount ++;
					if(returndata == 9){returndata = SDCARDWrite(0xFF); u = u + 14; } //Tab Space
					else{
						if(returndata==13) //Checking for Enter
							{
								returndata=SDCARDWrite(0xFF);
								if(returndata==10)
									{
										u=2;
										v=v+13;
										returndata=SDCARDWrite(0xFF);
									}
									else
										{						
											LCD_ShowChar(u,v,13,12,1);
											u=u+7;
											LCD_ShowChar(u,v,returndata,12,1);
											u=u+7;
											returndata=SDCARDWrite(0xFF);				
										}	
								}	
								else
									{
										if(returndata==0x00);
										else
											{
												LCD_ShowChar(u,v,returndata,12,1);
												u=u+7;
											}
											returndata=SDCARDWrite(0xFF);						
									}
						if(u>228){u=0; v=v+13;}
						if( v > 290)
							{	
								if(File->FileSize*0.00195 != SINGLEBLOCK)
									{ 
										delay_ms(5000);
										v = 25;
										//LCD_Clear(BLACK); 
										subwindowframe("TXT File");
									}
									else { break; }
							}
					}
			}
				
		if(File->FileSize*0.00195 != SINGLEBLOCK)
			{			
				SDCMD(12,0x00000000,0xFF);
				while(SDCARDWrite(0xFF) != 0x00);					
			}
		for(uint16_t i = 0; i< 512; i++){SDCARDWrite(0xFF);}
	}
		LCD_ShowString(2,300,240,16,16,"File Size:           Bytes.");
		LCD_ShowxNum(84,300,Sizecount,10,16,1);
	SDCARD_CS(1);
}

void DisplayDir (DirStruct *TempFile)
{
	DirStruct *File2;
	uint16_t FileNumber = 40;
	LCD_Clear(BLACK);
	LCD_ShowString(1,1,240,16,16,(uint8_t *)"./"); 
	if(TempFile[0].FileAttributes == DIRECTORY){ LCD_ShowString(20,1,240,16,16,TempFile[0].DOSFilename); }		
	LCD_ShowString(1,20,240,16,16,(uint8_t *)"No  Name    Ext     Size  Clus");
	for(uint8_t i = 1 ; ;i++)
				{
					if(TempFile[i].FileAttributes == 0x00){break;}
					if(TempFile[i].FileAttributes == DIRECTORY){TempFile[i].DOSFileExtension[0] = 'D', 
																											TempFile[i].DOSFileExtension[1] = 'I',
																											TempFile[i].DOSFileExtension[2] = 'R'; }
					File2 = &TempFile[i];
					LCD_ShowxNum(0,FileNumber,i,2,12,1);
					LCD_ShowString(20,FileNumber,45,12,12,File2->DOSFilename); 
					LCD_ShowString(100,FileNumber,23,12,12,File2->DOSFileExtension);
					LCD_ShowxNum(135,FileNumber,File2->FileAttributes,2,12,1);
					LCD_ShowxNum(135,FileNumber,File2->FileSize,10,12,1);
					LCD_ShowxNum(200,FileNumber,File2->StartClus,5,12,1);
					
					FileNumber = FileNumber + 12;
					if(FileNumber >= 300){LCD_ShowString(20,FileNumber,150,12,12,(uint8_t *)"& More File/Folders."); break;}																							
				  //if(FileNumber >= 300){FileNumber = 40; LCD_Clear(BLACK); LCD_ShowString(1,20,240,16,16,(uint8_t *)"No  Name    Ext     Size  Clus");}																									
				}
}


DirStruct* DirAlloc (void)
{
	SDCARD_CS(0);
	DirStruct *TempFile;
	uint16_t count = 1, i=0;
	SDRecordEntries();	
	TempFile = (DirStruct *)malloc(sizeof(DirStruct)*5);	
	TempFile[0] = SDRecordEntries();				
	SizeCount = 0;	
	SizeCount++;
	for(i = 1 ; ;count++)
				{
					if(count >15){
						SDCARDWrite(0xFF); SDCARDWrite(0xFF); SDCARDWrite(0xFF); SDCARDWrite(0xFF);		count = 0;}
			
					if( i % 5 == 0)
						{
							TempFile = (DirStruct *)realloc(TempFile,sizeof(DirStruct)*(i));
							if(TempFile == NULL){ LCD_Clear(BLACK); LCD_ShowString(50,50,240,16,16,"Allocation Failed."); free(TempFile); break;}
						}
					TempFile[i] = SDRecordEntries();
					if(TempFile[i].FileAttributes == 0x00){ break; }
					else{
								if((TempFile[i].FileAttributes == ARCHIVE || (TempFile[i].FileAttributes == DIRECTORY && TempFile[i].StartClus != 0x00))&& (TempFile[i].DOSFilename[0] >= 0x41 && TempFile[i].DOSFilename[0] <= 0x5B))
									{
										i++;
										SizeCount++;
									}
							} 
				}
			LCD_Clear(BLACK);				
			SDStopReadTransmit();
			SDCARD_CS(1);
			
	return TempFile;			
}


DirStruct* LoadDirEntries (uint8_t *DirectoryAddress)
{
	SDCARD_CS(0);
	if(SDreaddata2b(DirectoryAddress, MULBLOCK) == READY){	
		while(SDCARDWrite(0xFF) != 0xFE);	}
	return DirAlloc();
}


DirStruct* LoadSUBDirEntries (DirStruct *TempFolder,uint8_t DirectoryNumber)
{
	SDCARD_CS(0);
	DirStruct *TempDirectory = &TempFolder[DirectoryNumber];
	uint32_t RootClus=0;
	uint8_t DirName[8];
	for(uint8_t x=0;x<8;x++){	DirName[x] = TempDirectory->DOSFilename[x];}
	
	if(TempDirectory->StartClus == 0)
			{
				RootClus = RootDirAddress; 
			}
			else
				{	
					RootClus = RootDirAddress + (FatCommon.BPB_RootEntCnt*32 ) + ((TempDirectory->StartClus - 2) * FatCommon.BPB_SecPerClus * FatCommon.BPB_BytsPerSec);
				}
	free(TempFolder);		
	TempFolder = NULL;
					 
	if(SDreaddata2b((uint8_t *)RootClus, MULBLOCK) == READY)
		{
			while(SDCARDWrite(0xFF) != 0xFE);						
			TempFolder = DirAlloc();
			for(uint8_t x=0;x<8;x++){	TempFolder[0].DOSFilename[x] = DirName[x];}
		}
		
	SDCARD_CS(1);
	return TempFolder;
}

void OpenEntry (DirStruct *OpenEntryPtr, uint8_t EntryNumber)
{
	if(OpenEntryPtr[EntryNumber].FileAttributes == DIRECTORY){
		DisplayDir(LoadSUBDirEntries(OpenEntryPtr,EntryNumber));}
	else if(OpenEntryPtr[EntryNumber].FileAttributes == ARCHIVE){
		OpenAsTXTFile(&OpenEntryPtr[EntryNumber]);}
}

void writetoexistingfile ( DirStruct * TempFolder, uint8_t *Data)
{
	//uint16_t newsize;
	uint32_t FileLocation;
	if(MBRStruct.PartitionType == FAT_32_LBA || MBRStruct.PartitionType == FAT_32_CHS){FatCommon.BPB_BytsPerSec = 1;}
	FileLocation = RootDirAddress + (FatCommon.BPB_RootEntCnt * 32 ) + ((TempFolder->StartClus - 2) * FatCommon.BPB_SecPerClus * FatCommon.BPB_BytsPerSec);
	SDWritedata2b( Data, (uint8_t *)(FileLocation),0xFFFF);
	
	//SDWritedata2b(newsize,TempFolder->FileSize,0xFF);
}



void SDlocateDir (void)
{
	//uint8_t w = 0;
	DirStruct *FolderPtr;
	
	
	FolderPtr = LoadDirEntries((uint8_t *)RootDirAddress);
	DisplayDir(FolderPtr);		
	
	//writetoexistingfile(&FolderPtr[1],"I AM SRISURYA OF PLANET EARTH. 0123456789");
	
  OpenEntry(FolderPtr,1);
	
	free(FolderPtr);
	FolderPtr = NULL;
	
}
