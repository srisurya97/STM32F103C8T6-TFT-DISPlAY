#include "touch.h" 
#include "lcd.h"
#include "delay.h"
#include "stdlib.h"
#include "math.h"
#include "stm32f10x.h"
#include "MenuFramework.h"

//#include "flash.h"	  

//#include "stm32f10x_rcc.h"
//#include "stm32f10x_tim.h"
//#include "stm32f10x_spi.h"

/*
void settouchbit(uint8_t n)
{
	if(n==1){GPIOB->BSRR = GPIO_BSRR_BS13;}
  if(n==0){GPIOB->BRR = GPIO_BRR_BR13;}	
	
} */

/* void EXTI9_5_IRQHandler(void)
 {
	 EXTI->PR |= EXTI_PR_PR8;
	 LCD_ShowString(1, line13, 320, 16, 16, "Interrupt occured" );
	 
 } */

_m_tp_dev tp_dev=
{
	TP_Init,
	TP_Scan,
	TP_Adjust,
	0,
	0,
 	0,
	0,
	0,
	0,
	0,
	0,	  	 		
	0,
	0,	  	 		
};	
				

uint8_t CMD_RDX=0XD0;
uint8_t CMD_RDY=0X90;
uint16_t SAVE_ADDR_BASE=0;	 
			    					   

void TP_Write_Byte(uint8_t num)    
{  
	uint8_t count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)TDIN=1;  
		else TDIN=0;   
		num<<=1;    
		TCLK=0; 	 
		TCLK=1;
	}		 			    
}

uint16_t TP_Read_AD(uint8_t CMD)	  
{ 	 
	uint8_t count=0; 	  
	uint16_t Num=0; 
	TCLK=0;	
	TDIN=0; 
	TCS=0; 	
	TP_Write_Byte(CMD);
	delay_us(6);
	TCLK=0; 	     	    
	delay_us(1);    	   
	TCLK=1;		    	    
	TCLK=0; 	     	    
	for(count=0;count<16;count++)
	{ 				  
		Num<<=1; 	 
		TCLK=0;
		TCLK=1;
		if(DOUT)Num++; 		 
	}  	
	Num>>=4; 
	TCS=1;
	return(Num);   
}


#define READ_TIMES 10 	
#define LOST_VAL 2	  	
uint16_t TP_Read_XOY(uint8_t xy)
{
	uint16_t i, j;
	uint16_t buf[READ_TIMES];
	uint16_t sum=0;
	uint16_t temp;
	for(i=0;i<READ_TIMES;i++)buf[i]=TP_Read_AD(xy);		 		    
	for(i=0;i<READ_TIMES-1; i++)
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
}
 

uint8_t TP_Read_XY(uint16_t *x,uint16_t *y)
{
	uint16_t xtemp,ytemp;			 	 		  
	xtemp=TP_Read_XOY(CMD_RDX);
	ytemp=TP_Read_XOY(CMD_RDY);	  												   
	//if(xtemp<100||ytemp<100)return 0;
	*x=xtemp;
	*y=ytemp;
	return 1;
}


#define ERR_RANGE 30 
uint8_t TP_Read_XY2(uint16_t *x,uint16_t *y) 
{
	uint16_t x1,y1;
 	uint16_t x2,y2;
 	uint8_t flag;    
    flag=TP_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-ERR_RANGE内
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
}  

//////////////////////////////////////////////////////////////////////////////////		  

void TP_Drow_Touch_Point(uint16_t x,uint16_t y,uint16_t color)
{
	POINT_COLOR=color;
	LCD_DrawLine(x-12,y,x+13,y);
	LCD_DrawLine(x,y-12,x,y+13);
	LCD_DrawPoint(x+1,y+1);
	LCD_DrawPoint(x-1,y+1);
	LCD_DrawPoint(x+1,y-1);
	LCD_DrawPoint(x-1,y-1);
	Draw_Circle(x,y,6);
}	  


void TP_Draw_Big_Point(uint16_t x,uint16_t y,uint16_t color)
{	    
	POINT_COLOR=color;
	LCD_DrawPoint(x,y);
	LCD_DrawPoint(x+1,y);
	LCD_DrawPoint(x,y+1);
	LCD_DrawPoint(x+1,y+1);	 	  	
}						  

//////////////////////////////////////////////////////////////////////////////////		  

uint8_t TP_Scan(uint8_t tp)
{			   
	if(PEN==0)
	{
		if(tp){TP_Read_XY2(&tp_dev.x,&tp_dev.y);}
		else if(TP_Read_XY2(&tp_dev.x,&tp_dev.y))
		{
	 		tp_dev.x=tp_dev.xfac*tp_dev.x+tp_dev.xoff;
			tp_dev.y=tp_dev.yfac*tp_dev.y+tp_dev.yoff;  
		} 
		if((tp_dev.sta&TP_PRES_DOWN)==0)
		{		 
			tp_dev.sta=TP_PRES_DOWN|TP_CATH_PRES;
			tp_dev.x0=tp_dev.x;
			tp_dev.y0=tp_dev.y;  	   
					
		}			   
	}else
	{
		if(tp_dev.sta&TP_PRES_DOWN)
		{
			tp_dev.sta&=~(1<<7);
		}else
		{
			tp_dev.x0=0;
			tp_dev.y0=0;
			tp_dev.x=0xffff;
			tp_dev.y=0xffff;
		}	    
	}
	return tp_dev.sta&TP_PRES_DOWN;
}	  

//////////////////////////////////////////////////////////////////////////	 
/*
void TP_Save_Adjdata(void)
{
	s32 temp;			 
	//SPI_Flash_Erase_Sector(0);
	
	temp=tp_dev.xfac*100000000;
	//AT24CXX_WriteLenByte(SAVE_ADDR_BASE,temp,4);   
	SPI_Flash_Write((void *)&temp,SAVE_ADDR_BASE,4);
	temp=tp_dev.yfac*100000000;  
	//AT24CXX_WriteLenByte(SAVE_ADDR_BASE+4,temp,4);
	SPI_Flash_Write((void *)&temp,SAVE_ADDR_BASE+4,4);
	
	//AT24CXX_WriteLenByte(SAVE_ADDR_BASE+8,tp_dev.xoff,2);	
	SPI_Flash_Write((void *)&tp_dev.xoff,SAVE_ADDR_BASE+8,2);
	
	//AT24CXX_WriteLenByte(SAVE_ADDR_BASE+10,tp_dev.yoff,2);	
	SPI_Flash_Write((void *)&tp_dev.yoff,SAVE_ADDR_BASE+10,2);

	//AT24CXX_WriteOneByte(SAVE_ADDR_BASE+12,tp_dev.touchtype);	
	SPI_Flash_Write((void *)&tp_dev.touchtype,SAVE_ADDR_BASE+12,1);
	temp=0X0A;
	//AT24CXX_WriteOneByte(SAVE_ADDR_BASE+13,temp); 
	SPI_Flash_Write((void *)&temp,SAVE_ADDR_BASE+13,1);
}


uint8_t TP_Get_Adjdata(void)
{					  
	s32 tempfac;
	tempfac = 0;		
	SPI_Flash_Read((void *)&tempfac,SAVE_ADDR_BASE+13,1);
	if(tempfac==0X0A)
	{    												 
		//tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE,4);		  
		SPI_Flash_Read((void *)&tempfac,SAVE_ADDR_BASE,4);
		tp_dev.xfac=(float)tempfac/100000000;
		//tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+4,4);	
		SPI_Flash_Read((void *)&tempfac,SAVE_ADDR_BASE+4,4);
		tp_dev.yfac=(float)tempfac/100000000;
		//tp_dev.xoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+8,2);	
		SPI_Flash_Read((void *)&tp_dev.xoff,SAVE_ADDR_BASE+8,2);
		//tp_dev.yoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+10,2);		
		SPI_Flash_Read((void *)&tp_dev.yoff,SAVE_ADDR_BASE+10,2);
		
 		//tp_dev.touchtype=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+12);
 		SPI_Flash_Read((void *)&tp_dev.touchtype,SAVE_ADDR_BASE+12,1);
		if(tp_dev.touchtype)
		{
			CMD_RDX=0X90;
			CMD_RDY=0XD0;	 
		}else				   
		{
			CMD_RDX=0XD0;
			CMD_RDY=0X90;	 
		}		 
		return 1;	 
	}
	return 0;
}	 
*/

const uint8_t* TP_REMIND_MSG_TBL="Please use the stylus click the cross on the screen.The cross will always move until the screen adjustment is completed.";
 					  

void TP_Adj_Info_Show(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t x3,uint16_t y3,uint16_t fac)
{	  
	POINT_COLOR=RED;
	LCD_ShowString(40,160,lcddev.width,lcddev.height,16,"x1:");
 	LCD_ShowString(40+80,160,lcddev.width,lcddev.height,16,"y1:");
 	LCD_ShowString(40,180,lcddev.width,lcddev.height,16,"x2:");
 	LCD_ShowString(40+80,180,lcddev.width,lcddev.height,16,"y2:");
	LCD_ShowString(40,200,lcddev.width,lcddev.height,16,"x3:");
 	LCD_ShowString(40+80,200,lcddev.width,lcddev.height,16,"y3:");
	LCD_ShowString(40,220,lcddev.width,lcddev.height,16,"x4:");
 	LCD_ShowString(40+80,220,lcddev.width,lcddev.height,16,"y4:");  
 	LCD_ShowString(40,240,lcddev.width,lcddev.height,16,"fac is:");     
	LCD_ShowNum(40+24,160,x0,4,16);	
	LCD_ShowNum(40+24+80,160,y0,4,16);	
	LCD_ShowNum(40+24,180,x1,4,16);		
	LCD_ShowNum(40+24+80,180,y1,4,16);
	LCD_ShowNum(40+24,200,x2,4,16);		
	LCD_ShowNum(40+24+80,200,y2,4,16);
	LCD_ShowNum(40+24,220,x3,4,16);		
	LCD_ShowNum(40+24+80,220,y3,4,16);
 	LCD_ShowNum(40+56,lcddev.width,fac,3,16); 	

}



void TP_Adjust(void)
{								 
	uint16_t pos_temp[4][2];
	uint8_t  cnt=0;	
	uint16_t d1,d2;
	u32 tem1,tem2;
	float fac; 	
	uint16_t outtime=0;
 	cnt=0;				
	POINT_COLOR=BLUE;
	BACK_COLOR =WHITE;
	LCD_Clear(WHITE);  
	POINT_COLOR=RED;
	LCD_Clear(WHITE);
	POINT_COLOR=BLACK;
	LCD_ShowString(40,40,160,100,16,(uint8_t*)TP_REMIND_MSG_TBL);
	TP_Drow_Touch_Point(20,20,RED);
	tp_dev.sta=0;
	tp_dev.xfac=0;
	while(1)
	{	
		tp_dev.scan(1);
		if((tp_dev.sta&0xc0)==TP_CATH_PRES)
		{	
			outtime=0;		
			tp_dev.sta&=~(1<<6);
						   			   
			pos_temp[cnt][0]=tp_dev.x;
			pos_temp[cnt][1]=tp_dev.y;
			cnt++;	  
			switch(cnt)
			{			   
				case 1:						 
					TP_Drow_Touch_Point(20,20,WHITE);
					TP_Drow_Touch_Point(lcddev.width-20,20,RED);
					break;
				case 2:
 					TP_Drow_Touch_Point(lcddev.width-20,20,WHITE);
					TP_Drow_Touch_Point(20,lcddev.height-20,RED);	
					break;
				case 3:
 					TP_Drow_Touch_Point(20,lcddev.height-20,WHITE);
 					TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,RED);
					break;
				case 4:	
					tem1=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
					tem2=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);
					
					tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
					tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);
					fac=(float)d1/d2;
					if(fac<0.9||fac>1.1||d1==0||d2==0)
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);
   	 					TP_Drow_Touch_Point(20,20,RED);		
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//显示数据   
 						continue;
					}
					tem1=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);
					
					tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);
					fac=(float)d1/d2;
					if(fac<0.9||fac>1.1)
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	
   	 					TP_Drow_Touch_Point(20,20,RED);								
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//显示数据   
						continue;
					}
								   
					
					tem1=abs(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);
	
					tem1=abs(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);
					fac=(float)d1/d2;
					if(fac<0.9||fac>1.1)
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);
   	 					TP_Drow_Touch_Point(20,20,RED);							
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//显示数据   
						continue;
					}
					
					tp_dev.xfac=(float)(lcddev.width-40)/(pos_temp[1][0]-pos_temp[0][0]);
					tp_dev.xoff=(lcddev.width-tp_dev.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;
						  
					tp_dev.yfac=(float)(lcddev.height-40)/(pos_temp[2][1]-pos_temp[0][1]);
					tp_dev.yoff=(lcddev.height-tp_dev.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;  
					if(abs(tp_dev.xfac)>2||abs(tp_dev.yfac)>2)
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	
   	 					TP_Drow_Touch_Point(20,20,RED);							
						LCD_ShowString(40,26,lcddev.width,lcddev.height,16,"TP Need readjust!");
						tp_dev.touchtype=!tp_dev.touchtype;
						if(tp_dev.touchtype)
						{
							CMD_RDX=0X90;
							CMD_RDY=0XD0;	 
						}else				  
						{
							CMD_RDX=0XD0;
							CMD_RDY=0X90;	 
						}			    
						continue;
					}		
					POINT_COLOR=BLUE;
					LCD_Clear(WHITE);
					LCD_ShowString(35,110,lcddev.width,lcddev.height,16,"Touch Screen Adjust OK!");
					delay_ms(1000);
			//		TP_Save_Adjdata();  
 					LCD_Clear(WHITE);
					return;				 
			}
		}
		delay_ms(10);
		outtime++;
		if(outtime>1000)
		{
		//	TP_Get_Adjdata();
			break;
	 	} 
 	}
}



uint8_t TP_Init(void)
{
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;										///Enabling Clock for PORTA
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;									  ///Enabling Clock for PORTB	
		RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;  //setting up alternate function for interrupts
	
	  ////////////CS-->B13/////////////// | clk --> Pin B12 | MOSIstm/DINtouch  --> Pin B14 
		GPIOB->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_CNF12 | GPIO_CRH_CNF14 );		///Set to General pushpull
		GPIOB->CRH |= (GPIO_CRH_MODE13 | GPIO_CRH_MODE12 | GPIO_CRH_MODE14 ); ///Set to Output 50Mhz Speed 
		
		/////|MISO-->B15 
		GPIOB->CRH &= ~GPIO_CRH_CNF15_1;							//FLoat Input						
		GPIOB->CRH |= GPIO_CRH_CNF15_0;
		GPIOB->CRH &= ~GPIO_CRH_MODE15;  //Input Mode
		//////////| INT--> A8/////////
		GPIOA->CRH &= ~GPIO_CRH_CNF8_1;								//FLoat Input					
		GPIOA->CRH |= GPIO_CRH_CNF8_0;
		GPIOA->CRH &= ~GPIO_CRH_MODE8;			//Input Mode              
	
		/*
		EXTI->FTSR |= (EXTI_FTSR_TR8); //Falling edge
		
		AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI8_PA; //setting up interrupt for lineport A8
		
		EXTI->IMR |= (EXTI_IMR_MR8); //enabling the interrupt for the line 0,1,2
		NVIC_EnableIRQ(EXTI9_5_IRQn);   */

   	SAVE_ADDR_BASE=40;
  	TP_Read_XY(&tp_dev.x,&tp_dev.y);
/*		if(TP_Get_Adjdata())return 0;
		else			   
			{ 										    
			//	LCD_Clear(WHITE);
			//  TP_Adjust();  
			//	TP_Save_Adjdata();	 
			}			
		TP_Get_Adjdata();	*/
		//settouchbit(1); //Test CodeLine
		return 1; 									 
}



