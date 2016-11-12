#include "stm32f4xx.h"                  // Device header
 
 int main() 
{

	uint16_t button = 0;
	
	RCC ->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB -> MODER |=  GPIO_MODER_MODER15_1;
	GPIOB -> MODER &= ~GPIO_MODER_MODER12;
	
	while(1)
		
	{

		
 if(GPIOB -> IDR & GPIO_IDR_IDR_12)
{
  GPIOB -> ODR |= GPIO_ODR_ODR_15;
}else 
{
	  GPIOB -> ODR &= ~GPIO_ODR_ODR_15;
}
		
		
		
	}
	


}

