#include "stm32f4xx.h"                  // Device header

volatile uint16_t  i = 0;
uint16_t blue[2] = {0,1};
uint16_t red[2] = {0,0};
uint16_t green[2] = {0,0};

#define PSC_VALUE 624 
#define ARR_VALUE 255
#define SYS_TICKS 16000000
#define AFRH AFR[1]
#define AFRH13_AF1 0x00100000
#define AFRH14_AF1 0x01000000
#define AFRH15_AF1 0x10000000

int main()
{
	
		/** Inicjalizacja zewnetrznego zegara 48 MHz 

		RCC->CR |= RCC_CR_HSEON | RCC_CR_HSEBYP; // wlaczenie oscylatora HSE
		RCC->PLLCFGR = 4ul<<0 | 96ul<<6 | 4ul<<16 | RCC_PLLCFGR_PLLSRC_HSE; // ust. xM xN xP -> PLL i wl. sygnalu  HSE na PLL Source MUX
		RCC->SSCGR = 500ul<<0 | 44ul<<13 | RCC_SSCGR_SSCGEN; //Spread spectrum modulation ENABLE konfiguracja rozmycia widma
		RCC -> CR |= RCC_CR_PLLON;
		RCC -> CFGR |= RCC_CFGR_SW_PLL; // ustawienie sygnalu PLL na System Clock Mux
		RCC -> CR &= ~RCC_CR_HSION; //wylaczenie oscylatora HSI **/
	
	// TAKTOWANIE WEWNETRZNYM OSCYLATOREM 16MHz
	
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // taktowanie GPIOB
	RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN; // taktowanie TIMERA

	// Inicjalizacja portów GPIOB
	
	GPIOB -> MODER |= GPIO_MODER_MODER13_1 | GPIO_MODER_MODER14_1 | GPIO_MODER_MODER15_1; // fuckcja alternatywna
	GPIOB -> OTYPER |= GPIO_OTYPER_OT_13 | GPIO_OTYPER_OT_14 | GPIO_OTYPER_OT_15; //otwarty dren
	GPIOB -> PUPDR |= GPIO_PUPDR_PUPDR13_0 | GPIO_PUPDR_PUPDR14_0 | GPIO_PUPDR_PUPDR15_0; //pull up
	GPIOB -> AFRH |= AFRH13_AF1 | AFRH14_AF1 | AFRH15_AF1; // alternative function map
	
	// Inicjazlizacja TIM1
	TIM1 -> CR1 |= TIM_CR1_DIR; // downcounter
	TIM1 -> CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; // pwm mode 1 ch 1
	TIM1 -> CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;// pwm mode 1 ch 2
	TIM1 -> CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1; //pwm mode 1 ch 3
	
	TIM1 -> CCER |= TIM_CCER_CC1NE | TIM_CCER_CC2NE | TIM_CCER_CC3NE; //przesylanie sygnalu na odpowiedni pin wyj.
	TIM1->BDTR = TIM_BDTR_MOE;// OC and OCN outputs are enabled
	
	TIM1->PSC = PSC_VALUE;
	TIM1->ARR = ARR_VALUE;

	TIM1->CR1 = TIM_CR1_CEN; //Counter enabled
	TIM1 -> CCER |= TIM_CCER_CC1NP | TIM_CCER_CC2NP | TIM_CCER_CC3NP ;

SysTick_Config(SYS_TICKS);
	
while(1) ;

}

void SysTick_Handler(void)
{
	TIM1->CCR1 = blue[i]; // blue
	TIM1->CCR2 = red[i] ; //red
	TIM1->CCR3 = green[i]; //green
	i++;
	if(i>1){i=0;}
}

