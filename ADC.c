#include "ADC.h"
#include "stm32f10x.h"
#include "delay.h"
#include "lcd.h"
#include "MenuFramework.h"

//A4,A5,A6,A7

void adcinit(void)
{
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6; //ADC Clock speed at 12MHz.It cannot Exceed 14MHz
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN; //ADC,AltFunc,GPIOA Clock Enable
	//A5 Pin Config
	GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5_0); //Input mode
	GPIOA->CRL |= GPIO_CRL_CNF5_1; //Input PushPull
		
	ADC1->CR1 |=ADC_CR1_EOCIE; //Enable Interrupt
	ADC1->CR2 |= ADC_CR2_CONT; //Set to Continous Mode
	ADC1->CR2 |= ADC_CR2_ALIGN; //Left Align
	ADC1->SMPR2 |= ADC_SMPR2_SMP5; //Sampling rate in channel 5 to 239 cycles
	ADC1->SQR3 |= ADC_SQR3_SQ1_0 | ADC_SQR3_SQ1_2; //5th Channel,
	NVIC_EnableIRQ(ADC1_2_IRQn);
	
	ADC1->CR2 |= ADC_CR2_ADON;
	delay_ms(1);
	ADC1->CR2 |= ADC_CR2_CAL;
	while ((ADC1->CR2 & ADC_CR2_CAL)==1);
}

uint16_t ADCConvert (void)
{
	ADC1->CR2 |= ADC_CR2_ADON;	
}


void graph (uint16_t graphdata,uint8_t mode) 
{
	static uint16_t x=22, preval=315,drawdata=0;
	uint16_t y=315;
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
	
	if(x >= 237)
		{
			LCD_Fill(22,31,237,317,BLACK);
			x=22;
	  }
	drawdata = graphdata/231;
	POINT_COLOR=BLUE;
	if(mode == 0)
		{
			for(y=316;y>=(316-drawdata);y--)
				{
					LCD_DrawPoint(x,y);
				}
		}
		else
			{
						LCD_DrawPoint(x,(316-drawdata));
			}
	if(mode == 0)
		{
			x= x+2; 		
		}
	else { x++; }		
}  


void ADC1_2_IRQHandler(void)
{
	uint16_t Data=0;
	if(ADC1->SR & ADC_SR_EOC)
		{			
			Data = ADC1->DR; 	
			graph(Data,1);
		}
		else{
			POINT_COLOR =RED;
			LCD_ShowString(20,20,150,20,16,"Error: UnKnown ");
		}
}




