#include "stm32f4xx.h"                  // Device header
#include "string.h" // dla funkcji memcp

int main()
{
	
	static char buf1[] = "Janusz cebuli";
	static char buf2[50]; 
	const uint16_t size = sizeof(buf1);
	
	RCC -> AHB1ENR |= RCC_AHB1ENR_DMA2EN | RCC_AHB1ENR_GPIOBEN;
	__DSB();
	
	GPIOB -> MODER |= GPIO_MODER_MODER13_0|GPIO_MODER_MODER14_0;
	
	DMA2_Stream0 -> PAR = (uint32_t) buf1;
	DMA2_Stream0 -> M0AR = (uint32_t) buf2;
	DMA2_Stream0 -> NDTR = size;
	DMA2_Stream0 -> CR = DMA_SxCR_MINC|DMA_SxCR_PINC | DMA_SxCR_DIR_1;
	DMA2_Stream0 -> CR |= DMA_SxCR_EN;

while ( ! (DMA2->LISR & DMA_LISR_TCIF0) );
if ( memcmp(buf1, buf2, size) ) GPIOB -> ODR |= GPIO_ODR_ODR_13;
else ( GPIOB -> ODR |= GPIO_ODR_ODR_14);
while (1);

}
