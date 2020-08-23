#include"stm32f10x.h"
#include"RTC.h"

//F424

void RTCinit()
{
	//poll RTOFF for 1
	//set CNF bit  tp enter config mode
	//write to RTC registers
	//clear CNF bit to exit config mode
	//poll RTOFF for 1
	RCC->APB1ENR |= RCC_APB1ENR_BKPEN | RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_DBP;
	RCC->BDCR |= RCC_BDCR_RTCSEL_HSE | RCC_BDCR_RTCEN;
		
	while((RTC->CRL & RTC_CRL_RSF) != RTC_CRL_RSF);		
	while((RTC->CRL & RTC_CRL_RTOFF) != RTC_CRL_RTOFF);
	RTC->CRL |= RTC_CRL_CNF;
	RTC->PRLL = 0xF424;
	RTC->CNTL = 0;
	RTC->CRL &= ~RTC_CRL_CNF;		
	while((RTC->CRL & RTC_CRL_RTOFF) != RTC_CRL_RTOFF);
}
