#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "sys.h"

#define TP_PRES_DOWN 0x80 
#define TP_CATH_PRES 0x40 
							
void settouchbit(uint8_t n);							

typedef struct
{
	uint8_t (*init)(void);
	uint8_t (*scan)(uint8_t);
	void (*adjust)(void);		
	uint16_t x0;						
	uint16_t y0;
	uint16_t x; 						
	uint16_t y;						   	    
	uint8_t  sta;					
								
////////////////////////触摸屏校准参数/////////////////////////								
	float xfac;					
	float yfac;
	short xoff;
	short yoff;	   

	uint8_t touchtype;
}_m_tp_dev;

extern _m_tp_dev tp_dev;	 


#define PEN  	PAin(8)  	//PC10 INT
#define DOUT 	PBin(15)   	//PC2  MISO
#define TDIN 	PBout(14)  	//PC3  MOSI
#define TCLK 	PBout(12)  	//PC1  SCLK

#define TCS  	PBout(13)  	//PC0  CS  


void TP_Write_Byte(uint8_t num);					
uint16_t TP_Read_AD(uint8_t CMD);					
uint16_t TP_Read_XOY(uint8_t xy);					
uint8_t TP_Read_XY(uint16_t *x,uint16_t *y);
uint8_t TP_Read_XY2(uint16_t *x,uint16_t *y);
void TP_Drow_Touch_Point(uint16_t x,uint16_t y,uint16_t color);
void TP_Draw_Big_Point(uint16_t x,uint16_t y,uint16_t color);	
uint8_t TP_Scan(uint8_t tp);				
//void TP_Save_Adjdata(void);					
//uint8_t TP_Get_Adjdata(void);				
void TP_Adjust(void);							
uint8_t TP_Init(void);						
																 
void TP_Adj_Info_Show(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t x3,uint16_t y3,uint16_t fac);
 		  
//void settouchbit(uint8_t n);

#endif

















