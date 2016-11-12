#include "stm32f4xx.h"                  // Device header
 
 int main() 
{
	RCC ->AHB1ENR = RCC_AHB1ENR_GPIOBEN;
	GPIOB -> MODER =  GPIO_MODER_MODER13_0;
	
	SysTick_Config(16000000/2);

	while(1);

	
	}

	
	void SysTick_Handler(void){
GPIOB -> ODR ^= GPIO_ODR_ODR_13;
}
