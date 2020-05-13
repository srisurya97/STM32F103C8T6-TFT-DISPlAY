#include "ADC.h"
#include "stm32f10x.h"
#include "delay.h"
#include "lcd.h"

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
	//ADC1->SQR1 &= ~ADC_SQR1_L; //Only 1 Conversion
	ADC1->SQR3 |= ADC_SQR3_SQ1_0 | ADC_SQR3_SQ1_2; //5th Channel
	NVIC_EnableIRQ(ADC1_2_IRQn);
	
	ADC1->CR2 |= ADC_CR2_ADON;
	delay_ms(1);
	ADC1->CR2 |= ADC_CR2_CAL;
	while ((ADC1->CR2 & ADC_CR2_CAL)==1);
	//ADC1->CR2 |= ADC_CR2_ADON;	
}

uint16_t ADCConvert (void)
{
	ADC1->CR2 |= ADC_CR2_ADON;	
}


void graph (uint16_t graphdata)
{
	static uint16_t x=22, preval=315,drawdata=0;
	uint16_t y=315;
	POINT_COLOR=WHITE;
	LCD_DrawRectangle(22,30,238,318);
	LCD_ShowString(1,30,30,20,12,"0V-");
	LCD_ShowString(1,73,30,20,12,"0.5");
	LCD_ShowString(1,116,30,20,12,"1V-");
	LCD_ShowString(1,159,30,20,12,"1.5");
	LCD_ShowString(1,202,30,20,12,"2V-");
	LCD_ShowString(1,245,30,20,12,"2.5");
	LCD_ShowString(1,288,30,20,12,"3V-");
	LCD_ShowString(1,307,30,12,12,"3.3");
	
	
	if(x >= 237)
		{
			LCD_Fill(22,31,237,317,BLACK);
			x=22;
	  }
	
		
	drawdata = graphdata/231;
	
			for(y=316;y>=(drawdata+34);y--)
			{
				LCD_DrawPoint(x,y);
			}
		
		
	x= x+2; 		
}  


void ADC1_2_IRQHandler(void)
{
	uint16_t Data=0;
	if(ADC1->SR & ADC_SR_EOC)
		{
			POINT_COLOR = WHITE;
			LCD_ShowString(50,17,120,20,12,"ADC Value ");
			Data = ADC1->DR; 	
  		POINT_COLOR = BLACK;
			LCD_ShowNum(110,17,Data,6,12);	
			graph(Data);
			
		}
		else{
			POINT_COLOR =RED;
			LCD_ShowString(20,20,150,20,16,"Error: UnKnown ");
		}
}




