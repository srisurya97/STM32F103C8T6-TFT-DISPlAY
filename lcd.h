#ifndef __LCD_H
#define __LCD_H		

#include "sys.h" 
#include "stdlib.h"
#include "stm32f10x.h"


// REdefine for LCD controls
//LCD_RST PortB_0
#define SPILCD_RST_SET  GPIO_SetBits(GPIOB, GPIO_Pin_0)     
#define SPILCD_RST_RESET GPIO_ResetBits(GPIOB, GPIO_Pin_0)
//LCD_RS/DC PortB_1  
#define SPILCD_RS_SET  GPIO_SetBits(GPIOB,  GPIO_Pin_1)
#define SPILCD_RS_RESET  GPIO_ResetBits(GPIOB, GPIO_Pin_1)
//LCD_CS  PortA_4
#define SPILCD_CS_SET GPIO_SetBits(GPIOA,GPIO_Pin_4 ) 
#define SPILCD_CS_RESET  GPIO_ResetBits(GPIOA, GPIO_Pin_4)

  
typedef struct  
{ 					    
	uint16_t width;			//LCD 宽度
	uint16_t height;			//LCD 高度
	uint16_t id;				//LCD ID
	uint8_t	wramcmd;		//开始写gram指令
	uint8_t  setxcmd;		//设置x坐标指令
	uint8_t  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	  

//LCD参数
extern _lcd_dev lcddev;	
extern uint16_t  POINT_COLOR;
extern uint16_t  BACK_COLOR; 

//////////////////////////////////////////////////////////////////////////////////	 
//-----------------LCD端口定义---------------- 
#define	LCD_REST PBout(1) //LCD REST    		 PB1 	    

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
#define BROWN 			 		 0XBC40 //棕色
#define BRRED 			 	   0XFC07 //棕红色
#define GRAY  			 		 0X8430 //灰色
#define COBALT					 0xAA00FF	
#define INDIGO					 0xA20025
#define AMBER						 0x008A00
#define TEAL						 0x225
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 		 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
	    															  
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
void LCD_WR_DATA8(uint8_t da);   
 
void showhanzi16(unsigned int x,unsigned int y,unsigned char index);
void showhanzi32(unsigned int x,unsigned int y,unsigned char index);						  		 
#endif  
	 
	 



