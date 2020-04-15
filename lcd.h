#ifndef __LCD_H
#define __LCD_H		


#include "stdlib.h"
#include "stm32f10x.h"
#include "spi.h"

// REdefine for LCD controls
//LCD_RST PortB_0
#define SPILCD_RST_SET  GPIO_SetBit(GPIOB, GPIO_BSRR_BS0)     
#define SPILCD_RST_RESET GPIO_ResetBit(GPIOB, GPIO_BRR_BR0)
//LCD_RS/DC PortB_1  
#define SPILCD_RS_SET   GPIO_SetBit(GPIOB,  GPIO_BSRR_BS1)   //Data
#define SPILCD_RS_RESET  GPIO_ResetBit(GPIOB, GPIO_BRR_BR1) //REG
//LCD_CS  PortA_4
#define SPILCD_CS_SET GPIO_SetBit(GPIOA,GPIO_BSRR_BS4 ) 
#define SPILCD_CS_RESET  GPIO_ResetBit(GPIOA, GPIO_BRR_BR4)

  
typedef struct  
{ 					    
	uint16_t width;			
	uint16_t height;		
	uint16_t id;				//LCD ID
	uint8_t	wramcmd;		//开始写gram指令
	uint8_t  setxcmd;		
	uint8_t  setycmd;			 
}_lcd_dev; 	  

//LCD参数
extern _lcd_dev lcddev;	
extern uint16_t  POINT_COLOR;
extern uint16_t  BACK_COLOR; 

//////////////////////////////////////////////////////////////////////////////////	 
//-----------------LCD端口定义---------------- 
//#define	LCD_REST BIT_ADDR(GPIOB_ODR_Addr,n) //LCD REST    		 PB1 	    

typedef struct
{
	uint16_t LCD_REG;
	uint16_t LCD_RAM;
} LCD_TypeDef;
//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A10作为数据命令区分线 
//注意设置时STM32内部会右移一位对其! 111110=0X3E			    
#define LCD_BASE        ((uint32_t)(0x60000000 | 0x0007FFFE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)
//////////////////////////////////////////////////////////////////////////////////

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 	 0x001F  
#define BRED             0XF81F
#define GRED	 					 0XFFE0
#define GBLUE						 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 		 0XBC40 
#define BRRED 			 	   0XFC07 
#define GRAY  			 		 0X8430 

#define TEAL						 0x225
//GUI颜色

#define DARKBLUE      	 0X01CF	
#define LIGHTBLUE      	 0X7D7C	  
#define GRAYBLUE       	 0X5458 
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F 
//#define LIGHTGRAY        0XEF5B 
#define LGRAY 			 		 0XC618 

#define LGRAYBLUE        0XA651 
#define LBBLUE           0X2B12 
	    															  
void LCD_Init(void);			
void LCD_DisplayOn(void);	
void LCD_DisplayOff(void);													
void LCD_Clear(uint16_t Color);	 												
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);							
void LCD_DrawPoint(uint16_t x,uint16_t y);										
void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color);			
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r);								
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);	
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);		
void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);			
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);  			
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);	
	  
void showimage(uint16_t x,uint16_t y); 
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
void LCD_WriteRAM_Prepare(void);

void LCD_WR_DATA8(uint8_t da);  //   
 
void showhanzi16(unsigned int x,unsigned int y,unsigned char index);
void showhanzi32(unsigned int x,unsigned int y,unsigned char index);						  		 
#endif  
	 
	 



