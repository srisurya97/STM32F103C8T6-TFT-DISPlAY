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
	GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5_0); //Input mode
	GPIOA->CRL |= GPIO_CRL_CNF5_1; //Input PushPull
	
	
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
			GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6_0); //Input mode
			GPIOA->CRL |= GPIO_CRL_CNF6_1; //Input PushPull

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

uint16_t ADCConvert (void)
{
	POINT_COLOR=WHITE;
	LCD_DrawRectangle(22,30,238,318);
	POINT_COLOR=GREEN;
	LCD_ShowString(1,27,30,20,12,"3.3");
	LCD_ShowString(1,49,30,20,12,"3V-");
	LCD_ShowString(1,92,30,20,12,"2.5");
	LCD_ShowString(1,135,30,20,12,"2V-");
	LCD_ShowString(1,178,30,20,12,"1.5");
	LCD_ShowString(1,221,30,20,12,"1V-");
	LCD_ShowString(1,264,30,20,12,"0.5");
	LCD_ShowString(1,307,30,12,12,"0V-");		
	
	ADC1->CR2 |= ADC_CR2_ADON;
}


void graph (uint16_t graphdata,uint8_t mode, uint16_t color) 
{
	static uint16_t x=24, preval=315,drawdata=0,settingdone=0,x0=24,y0=315,x1=24,y1=315;
	uint16_t y=315;
	
	drawdata = graphdata/231;
	/*
	if(settingdone == 0)
		{
			POINT_COLOR=WHITE;
			LCD_DrawRectangle(22,30,238,318);
			POINT_COLOR=GREEN;
			LCD_ShowString(1,27,30,20,12,"3.3");
			LCD_ShowString(1,49,30,20,12,"3V-");
			LCD_ShowString(1,92,30,20,12,"2.5");
			LCD_ShowString(1,135,30,20,12,"2V-");
			LCD_ShowString(1,178,30,20,12,"1.5");
			LCD_ShowString(1,221,30,20,12,"1V-");
			LCD_ShowString(1,264,30,20,12,"0.5");
			LCD_ShowString(1,307,30,12,12,"0V-");		
			//settingdone = 1;	
		} */
		
	if(x >= 237)
		{
			LCD_Fill(23,31,237,317,BLACK);
			x=24;
			
			x0=24;
			y0=316-drawdata;
			x1=24;
			y1=316-drawdata;
	  }
	if(mode == 0)
		{
			POINT_COLOR=color;
			for(y=316;y>=(316-drawdata);y--)
				{
					LCD_DrawPoint(x,y);
				}
				x= x+2; 			
		}
	if(mode == 1)
		{
			POINT_COLOR = RED;
			LCD_DrawLine(23,260,237,260);
			POINT_COLOR=BLACK;
			LCD_DrawLine(23,316-preval,237,316-preval);
			POINT_COLOR=color;
			LCD_DrawLine(23,316-drawdata,237,316-drawdata);
			preval = drawdata;
		}
	if(mode==2)
		{
			if(color==CYAN)
				{
					POINT_COLOR=color;
					LCD_DrawPoint(x,(316-drawdata));
					LCD_DrawLine(x0,316-y0,x,316-drawdata);
					x0=x;
					y0=drawdata;
					x++;
				} 
			if(color==BLUE)
				{
					POINT_COLOR=color;
					LCD_DrawPoint(x,(316-drawdata));
					LCD_DrawLine(x1,316-y1,x,316-drawdata);
					x1=x;
					y1=drawdata;
					x++;
				}				
		}
}  


void ADC1_2_IRQHandler(void)
{
	Data[0]=ADC1->DR;
	POINT_COLOR =BLUE;
	LCD_ShowString(1,1,150,15,12,"Channel 5:     V");
	POINT_COLOR=BLACK;
	LCD_ShowNum(60,1,((Data[0]>>4)*3.3/4095),5,12);
	graph(Data[0],mode,BLUE);
		
	if(ADC1->SQR1 == (ADC_SQR1_L_0))
		{
			POINT_COLOR=CYAN;
			LCD_ShowString(1,14,150,15,12,"Channel 6:     V ");
			POINT_COLOR=BLACK;
			LCD_ShowNum(60,14,(((Data[1]>>4)*3.3)/4095),5,12);
			graph(Data[1],mode,CYAN);
		}
		
}




