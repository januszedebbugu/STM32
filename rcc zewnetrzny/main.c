#include "stm32f4xx.h"                  // Device header

int main(void)
	
{

	RCC -> CR |= RCC_CR_HSEON | RCC_CR_HSEBYP;
	RCC -> PLLCFGR = 4ul<<0 | 96ul<<6 | 4ul<<16 | RCC_PLLCFGR_PLLSRC_HSE;
	RCC->SSCGR = 500ul<<0 | 44ul<<13 | RCC_SSCGR_SSCGEN;
	RCC -> CR |= RCC_CR_PLLON;
  // RCC->CFGR = RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2; dzielniki u nas sa 1
	RCC -> CFGR |= RCC_CFGR_SW_PLL;
	RCC -> CR &= ~RCC_CR_HSION;
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB -> MODER |= GPIO_MODER_MODER13_0;
	
	while(6)
	{
		GPIOB -> ODR ^= GPIO_ODR_ODR_13;
		for(uint32_t i = 4800000; i ; i--){}; 
	}
	
	
}
