#include "stm32f4xx.h"                  // Device header

#define AFRH AFR[1]
#define AFRH9_AF7 0x00000070
#define AFRH10_AF7 0x00000700
#define USARTDIV 16000000/115200
#define PSC_VALUE 1600-1
#define ARR_VALUE 2-1
#define NDTR_VALUE 5000
#define PETLA_USART 5000

static volatile uint16_t wyniki [5000];
static volatile uint16_t wyniki1 [5000];
uint16_t i = 0;

int main()
{
	
	
	// ****** wlaczenie taktowania ********* //
	RCC -> AHB1ENR |= RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_GPIOBEN|RCC_AHB1ENR_GPIOAEN;
	RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN |RCC_APB1ENR_TIM3EN;
	RCC -> APB2ENR |= RCC_APB2ENR_ADC1EN |RCC_APB2ENR_USART1EN;
	__DSB();
	
	// ****** konfiguracja LED ********* //
	GPIOB -> MODER |= GPIO_MODER_MODER13_0|GPIO_MODER_MODER14_0;
	GPIOB -> ODR |= GPIO_ODR_ODR_13|GPIO_ODR_ODR_14;
	
	
	
	// ****** konfiguracja USART1 ********* //
	GPIOA -> MODER  |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1; //Funkcja alternatywna: PA9(TX), PA10(RX)
	GPIOA -> OTYPER |= GPIO_OTYPER_OT_10; // Open-drain dla RX
	GPIOA -> AFRH |= AFRH9_AF7 | AFRH10_AF7; // AF7 = 0111 (usart1)
	USART1 -> BRR = USARTDIV; // baudrate
	USART1 -> CR1 |= USART_CR1_UE | USART_CR1_RXNEIE | USART_CR1_TE | USART_CR1_RE; 
	USART1 -> CR3 |= USART_CR3_CTSE;
	
	// ****** konfiguracja TIM3 ********* //
	TIM3->PSC = PSC_VALUE;
  TIM3->ARR = ARR_VALUE;
  TIM3->EGR = TIM_EGR_UG;
  TIM3->CR2 = TIM_CR2_MMS_1;

	// ****** konfiguracja DMA2 ********* //
	DMA2_Stream0->M0AR = (uint32_t)wyniki;
	DMA2_Stream0->M1AR = (uint32_t)wyniki1;
	DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;
	DMA2_Stream0->NDTR = NDTR_VALUE;
	DMA2_Stream0->CR = DMA_SxCR_MSIZE_1 | DMA_SxCR_PSIZE_0 | DMA_SxCR_MINC | DMA_SxCR_EN| DMA_SxCR_TCIE|DMA_SxCR_CIRC|DMA_SxCR_DBM ;
	DMA2_Stream0->FCR = DMA_SxFCR_DMDIS | DMA_SxFCR_FTH_0;
	
	// ****** konfiguracja ADC1 ********* //
	GPIOA -> MODER |= GPIO_MODER_MODER1;
	ADC1->CR2 = ADC_CR2_ADON  | ADC_CR2_DDS | ADC_CR2_DMA | ADC_CR2_EXTEN_0|ADC_CR2_EXTSEL_3;
	ADC1->CR1 = ADC_CR1_SCAN |ADC_CR1_RES_1 | ADC_CR1_EOCIE;
	ADC1->SMPR1 = 7<<9;
	ADC1->SMPR2 = 7<<21 | 7<<15;
	ADC1->SQR3 = 1<<0;
	
	ADC1->CR2 |= ADC_CR2_SWSTART;
	TIM3->CR1 |= TIM_CR1_CEN;
	
	// ****** zezwolenie na przerwania ********* //
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);



while(1) ;

}
void DMA2_Stream0_IRQHandler()
{
if(!(DMA2_Stream0 -> CR & DMA_SxCR_CT)) 
	{
		GPIOB -> ODR ^= GPIO_ODR_ODR_14;
	
		for(i=0; i<PETLA_USART;i++)
		{
				while(!(USART1 -> SR & USART_SR_TC));
				USART1 -> DR = wyniki1[i];
				
		}

	GPIOB -> ODR ^= GPIO_ODR_ODR_14;
	DMA2 ->LIFCR =DMA_LIFCR_CTCIF0;
	DMA2 ->LIFCR =DMA_LIFCR_CHTIF0;
	}
	
	else if(DMA2_Stream0 -> CR & DMA_SxCR_CT) 
	{
		GPIOB -> ODR ^= GPIO_ODR_ODR_14;
	
		for(i=0; i<PETLA_USART;i++)
		{
			if(USART1->SR & USART_SR_TXE)
			{
				while(!(USART1 -> SR & USART_SR_TC));
				USART1 -> DR = wyniki[i];
			}
		}

	GPIOB -> ODR ^= GPIO_ODR_ODR_14;
	DMA2 ->LIFCR =DMA_LIFCR_CTCIF0;
	DMA2 ->LIFCR =DMA_LIFCR_CHTIF0;
	}
}

