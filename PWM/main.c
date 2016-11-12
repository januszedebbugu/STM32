#include "stm32f4xx.h"                  // Device header
	uint32_t tmp1 = 0;
	
int main()
{
	

	
	// Inicjalizacja zewnetrznego zegara 48 Mhz 
	
	RCC->CR |= RCC_CR_HSEON | RCC_CR_HSEBYP; // wlaczenie oscylatora HSE
	RCC->PLLCFGR = 4ul<<0 | 96ul<<6 | 4ul<<16 | RCC_PLLCFGR_PLLSRC_HSE; // ust. xM xN xP -> PLL i wl. sygnalu  HSE na PLL Source MUX
	RCC->SSCGR = 500ul<<0 | 44ul<<13 | RCC_SSCGR_SSCGEN; //Spread spectrum modulation ENABLE konfiguracja rozmycia widma
	RCC -> CR |= RCC_CR_PLLON;
	RCC -> CFGR |= RCC_CFGR_SW_PLL; // ustawienie sygnalu PLL na System Clock Mux
	RCC -> CR &= ~RCC_CR_HSION; //wylaczenie oscylatora HSI
	
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // taktowanie GPIOB
	RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN; // taktowanie TIMERA

	
	// Inicjalizacja portów GPIOB
	
	GPIOB -> MODER |= GPIO_MODER_MODER13_1 | GPIO_MODER_MODER14_1 | GPIO_MODER_MODER15_1; // fuckcja alternatywna
	GPIOB -> OTYPER |= GPIO_OTYPER_OT_13 | GPIO_OTYPER_OT_14 | GPIO_OTYPER_OT_15; //otwarty dren
	GPIOB -> PUPDR |= GPIO_PUPDR_PUPDR13_0 | GPIO_PUPDR_PUPDR14_0 | GPIO_PUPDR_PUPDR15_0; //pull up
	GPIOB -> AFR[1] = (1ul << 20) | (1ul << 24) | (1ul << 28);  // alternative function map
	
	// Inicjazlizacja TIM1
	
	TIM1 -> CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; // pwm mode 1 ch 1

	TIM1 -> DIER |= TIM_DIER_UIE;

	TIM1 -> CCER |= TIM_CCER_CC1NE ; //przesylanie sygnalu na odpowiedni pin wyj.
	TIM1->BDTR = TIM_BDTR_MOE;// OC and OCN outputs are enabled
	
	TIM1->PSC = 800-1;
	TIM1->ARR = 200-1;
	

 NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);

TIM1->CR1 = TIM_CR1_CEN; //Counter enabled
	while(1) ;
}


		void TIM1_UP_TIM10_IRQHandler()
	{
		if(TIM1 -> SR & TIM_SR_UIF)
			{
			TIM1 -> SR &= ~TIM_SR_UIF;
				
				tmp1 = tmp1 + 1;
				if (tmp1 == 199)
				{
				tmp1=0;
				}
					TIM1->CCR1 = tmp1; // blue
				
			}
	}
