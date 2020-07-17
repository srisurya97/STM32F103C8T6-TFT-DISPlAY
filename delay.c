#include "stm32f10x.h"
#include "delay.h"


int mytick=0;

void delayinit(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; //clock start
	TIM4->PSC = 0;
	TIM4->ARR = 144;
	TIM4->CR1 |= TIM_CR1_URS;  // update request source only overflow
	TIM4->DIER |= TIM_DIER_UIE; //interrupt enable
	TIM4->EGR |= TIM_EGR_UG;
	NVIC_EnableIRQ(TIM4_IRQn);
}

void TIM4_IRQHandler(void)
{
	mytick++;
	TIM4->SR &= ~TIM_SR_UIF;
}


void delay_ms(uint16_t ms)
{
	TIM4->CR1 |= TIM_CR1_CEN;
	mytick =0;
	while(mytick<(ms*1000));
	TIM4->CR1 &= ~TIM_CR1_CEN;
}



void delay_us(uint16_t us)
{
	TIM4->CR1 |= TIM_CR1_CEN;
	mytick =0;
	while(mytick<us);
	TIM4->CR1 &= ~TIM_CR1_CEN;
}




