#include "ADC.h"
#include "stm32f10x.h"
#include "delay.h"
#include "lcd.h"
#include "MenuFramework.h"

//A4,A5,A6,A7
uint16_t Data[2]={0,0};
uint8_t mode;

void adcinit(void)
{
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6; //ADC Clock speed at 12MHz.It cannot Exceed 14MHz
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN ; //ADC,AltFunc,GPIOA Clock Enable
	
	//A5 Pin Config
	GPIOA->CRL &= ~GPIO_CRL_MODE5; //Input mode
	
	GPIOA->CRL &= ~GPIO_CRL_CNF5_0;
	GPIOA->CRL |= GPIO_CRL_CNF5_1; //Input PushPull
	//GPIOA->CRL &= ~GPIO_CRL_CNF5;
	
	ADC1->CR1 |=ADC_CR1_EOCIE; //Enable Interrupt
	ADC1->CR2 |= ADC_CR2_CONT; //Set to Continous Mode
	ADC1->CR2 |= ADC_CR2_ALIGN; //Left Align
	ADC1->SMPR2 |= ADC_SMPR2_SMP5; //Sampling rate in channel 5 to 239 cycles
	ADC1->SQR3 |= ADC_SQR3_SQ1_0 | ADC_SQR3_SQ1_2; //5th Channel
	ADC1->SQR1 &= ~ADC_SQR1_L; //1 Conversions
	
		
	if(mode ==1 || (mode == 2 && rem != 10 ))
		{
			RCC->AHBENR|= RCC_AHBENR_DMA1EN;
			//A6 Pin Config
			GPIOA->CRL &= ~GPIO_CRL_MODE6; //Input mode
			
			GPIOA->CRL &= ~GPIO_CRL_CNF6_0;
			GPIOA->CRL |= GPIO_CRL_CNF6_1; //Input PushPull
			//GPIOA->CRL &= ~GPIO_CRL_CNF6;
			
			ADC1->SMPR2 |= ADC_SMPR2_SMP6; //Sampling rate in channel 6 to 239 cycles
			ADC1->SQR3 |= ADC_SQR3_SQ2_1 | ADC_SQR3_SQ2_2; //6th Channel
			ADC1->SQR1 |= ADC_SQR1_L_0; //2 Conversions
			ADC1->CR1 |= ADC_CR1_SCAN; //Scan Mode ON
			ADC1->CR2 |= ADC_CR2_DMA;
			// setting up DMA
			DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR));
			DMA1_Channel1->CMAR = (uint32_t)Data;
			DMA1_Channel1->CNDTR = 2;
			DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0 | DMA_CCR1_MSIZE_0 | DMA_CCR1_CIRC | DMA_CCR1_MINC; 
	
			DMA1_Channel1->CCR |= DMA_CCR1_EN;
		}
		
	NVIC_EnableIRQ(ADC1_2_IRQn);
	
	ADC1->CR2 |= ADC_CR2_ADON;
	delay_ms(1);
	ADC1->CR2 |= ADC_CR2_CAL;
	while ((ADC1->CR2 & ADC_CR2_CAL)==1);	
}

void GraphLayout (void)
{
	POINT_COLOR=WHITE;
	LCD_DrawRectangle(22,20,lcddev.width-2,lcddev.height-2); //238  318
	POINT_COLOR=GREEN;
	//LCD_ShowString(1,(lcddev.height-20)-((3.6*((lcddev.height-4-26)/3.6))-6),30,20,12,"3.6-");
	LCD_ShowString(1,(lcddev.height-14)-(3.3*((lcddev.height-10-26)/3.3)),30,20,12,"3.3-");
	LCD_ShowString(1,(lcddev.height-14)-(3.2*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(3.1*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(3*((lcddev.height-10-26)/3.3)),30,20,12," 3V-");
	LCD_ShowString(1,(lcddev.height-14)-(2.9*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(2.8*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(2.7*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(2.6*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(2.5*((lcddev.height-10-26)/3.3)),30,20,12,"2.5-");
	LCD_ShowString(1,(lcddev.height-14)-(2.4*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(2.3*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(2.2*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(2.1*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(2*((lcddev.height-10-26)/3.3)),30,20,12," 2V-");
	LCD_ShowString(1,(lcddev.height-14)-(1.9*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(1.8*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(1.7*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(1.6*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(1.5*((lcddev.height-10-26)/3.3)),30,20,12,"1.5-");
	LCD_ShowString(1,(lcddev.height-14)-(1.4*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(1.3*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(1.2*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(1.1*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(1*((lcddev.height-10-26)/3.3)),30,20,12," 1V-");
	LCD_ShowString(1,(lcddev.height-14)-(0.9*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(0.8*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(0.7*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(0.6*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(0.5*((lcddev.height-10-26)/3.3)),30,20,12,"0.5-");
	LCD_ShowString(1,(lcddev.height-14)-(0.4*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(0.3*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(0.2*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(0.1*((lcddev.height-10-26)/3.3)),30,20,12,"   -");
	LCD_ShowString(1,(lcddev.height-14)-(0*((lcddev.height-10-26)/3.3)),30,12,12," 0V-");
	LCD_Fill(23,21,lcddev.width-3,lcddev.height-3,BLACK);
}

void ADCConvert (void)
{
	GraphLayout();
	ADC1->CR2 |= ADC_CR2_ADON;
}


void PlotGraph (uint16_t graphdata,uint8_t mode, uint16_t color) 
{
	static uint16_t x=24, preval=0,drawdata=0,x0=24,y0=0,x1=24,y1=0;
	uint16_t y=lcddev.height-10;
	
	drawdata = (graphdata*(lcddev.height-10-26))/4095;//14.5;
	
	if(x >= lcddev.width-3)
		{
			x=24;
			x0=24;
			x1=24;
			LCD_Fill(23,21,lcddev.width-3,lcddev.height-3,BLACK);
			
	  }
	if(mode == 0)
		{
			POINT_COLOR=color;
			for(y=(lcddev.height-10);y>=((lcddev.height-10)-drawdata);y--)
				{
					LCD_DrawPoint(x,y);
				}
				x= x+2; 			
		}
	if(mode == 1)
		{
			POINT_COLOR = RED;
			LCD_DrawLine(23,(lcddev.height-20)-((0.5*((lcddev.height-10-26)/3.3))-6),lcddev.width-3,(lcddev.height-20)-((0.5*((lcddev.height-10-26)/3.3))-6)); //237
			POINT_COLOR=BLACK;
			LCD_DrawLine(23,y-preval,lcddev.width-3,y-preval); 
			POINT_COLOR=color;
			LCD_DrawLine(23,y-drawdata,lcddev.width-3,y-drawdata);
			preval = drawdata;
		}
	if(mode==2)
		{
			if(color==CYAN)
				{
					POINT_COLOR=color;
					LCD_DrawLine(x0,y-y0,x,y-drawdata);
					x0=x;
					y0=drawdata;
					x++;
				} 
			if(color==BLUE)
				{
					POINT_COLOR=color;
					LCD_DrawLine(x1,y-y1,x,y-drawdata);
					x1=x;
					y1=drawdata;
					x++;
				}				
		}
}  


void ADC1_2_IRQHandler(void)
{
	if(mode ==0 || (mode == 2 && rem == 10 ))	Data[0]=ADC1->DR;
	POINT_COLOR =BLUE;
	LCD_ShowString(1,1,150,15,12,"Channel 5:     V");
	POINT_COLOR=BLACK;
	LCD_ShowNum(60,1,((Data[0]>>4)*3.3/4095),5,12);
	PlotGraph((Data[0]>>4),mode,BLUE);
		
	if(ADC1->SQR1 == (ADC_SQR1_L_0))
		{
			POINT_COLOR=CYAN;
			LCD_ShowString(105,1,150,15,12,"Channel 6:     V ");
			POINT_COLOR=BLACK;
			LCD_ShowNum(165,1,(((Data[1]>>4)*3.3)/4095),5,12);
			PlotGraph((Data[1]>>4),mode,CYAN);
		}
		
}


