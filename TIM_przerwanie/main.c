#include "stm32f4xx.h"                  // Device header
int main()
	{
		
		RCC ->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
		RCC ->APB2ENR |= RCC_APB2ENR_TIM1EN;

		GPIOB -> MODER |= GPIO_MODER_MODER14_0;
		
		TIM1 -> PSC = (1600UL-1UL);
		TIM1 -> ARR = (10000UL- 1UL);
		TIM1 -> DIER |= TIM_DIER_UIE;
		TIM1 -> CR1 |= TIM_CR1_CEN;
		
		NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
		while(10) ; 
	}
	
void TIM1_UP_TIM10_IRQHandler()
	{
		if(TIM1 -> SR & TIM_SR_UIF)
			{
			TIM1 -> SR &= ~TIM_SR_UIF;
			GPIOB -> ODR ^= GPIO_ODR_ODR_14;
			}
	}
