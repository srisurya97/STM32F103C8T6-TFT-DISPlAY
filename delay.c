#include "delay.h"
#include "stm32f10x.h"

static uint8_t  fac_us=0;
static uint16_t fac_ms=0;
#ifdef OS_CRITICAL_METHOD
void SysTick_Handler(void)
{				   
	OSIntEnter();		
  OSTimeTick(); 
  OSIntExit();  
}
#endif

void delay_init()	 
{

#ifdef OS_CRITICAL_METHOD
	uint32_t reload;
#endif
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us=SystemCoreClock/8000000;	
	 
#ifdef OS_CRITICAL_METHOD 	
	reload=SystemCoreClock/8000000;
	reload*=1000000/OS_TICKS_PER_SEC;
							
	fac_ms=1000/OS_TICKS_PER_SEC;
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;
	SysTick->LOAD=reload; 	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
#else
	fac_ms=(uint16_t)fac_us*1000;
#endif
}								    

#ifdef OS_CRITICAL_METHOD	

void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;	   	 
	ticks=nus*fac_us; 			
	tcnt=0;
	told=SysTick->VAL;      
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;
		}  
	}; 									    
}

void delay_ms(uint16_t nms)
{	
	if(OSRunning==TRUE)
	{		  
		if(nms>=fac_ms)
		{
   			OSTimeDly(nms/fac_ms);
		}
		nms%=fac_ms;				
	}
	delay_us((uint32_t)(nms*1000));	
}
#else

void delay_us(uint32_t nus)
	{
		uint32_t temp;	    	 
		SysTick->LOAD=nus*fac_us; 
		SysTick->VAL=0x00;        
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; 
		do
			{
				temp=SysTick->CTRL;
			}
		while(temp&0x01&&!(temp&(1<<16)));
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
		SysTick->VAL =0X00;      	 
	}


void delay_ms(uint16_t nms)
	{
		uint32_t temp;		   
		SysTick->LOAD=(uint32_t)nms*fac_ms;
		SysTick->VAL =0x00;           
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;
		
		do{
				temp=SysTick->CTRL;
			} while(temp&0x01&&!(temp&(1<<16)));  
		
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
		SysTick->VAL =0X00;  	    
	}	 
#endif



