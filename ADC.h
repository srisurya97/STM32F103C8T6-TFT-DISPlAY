#ifndef ADC_H
#define ADC_H

#include "stm32f10x.h"

extern uint8_t mode;

void adcinit(void);
uint16_t ADCConvert (void);





#endif
