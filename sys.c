#include "sys.h"
#include "misc.h"

void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

}
void inttohex(uint16_t value,uint8_t *hex)
{
	uint16_t x1;
	hex[0]=value/(16*16*16)+'0';
	x1=value%(16*16*16);
	hex[1]=x1/(16*16)+'0';
	x1=value%(16*16);
	hex[2]=x1/16+'0';
	x1=value%16;
	hex[3]=x1+'0';	
	hex[4]=' ';		 

}

