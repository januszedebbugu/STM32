#include "stm32f4xx.h" // Device header
#include <cstring>

#define AFRH AFR[1]
#define AFRH9_AF7 0x00000070
#define AFRH10_AF7 0x00000700
#define USARTDIV 16000000/9600
#define DLUGOSC_HASLA 3

char haslo[DLUGOSC_HASLA] = "aaa";
char wprowadzane_haslo[DLUGOSC_HASLA];
const char zgodnosc[DLUGOSC_HASLA] = "bla";
const char niezgodnosc = 'e';
volatile uint32_t indeks = 0;

/***************** USART 1 *******************
 
 TX D8  PORT A9  -> AF07		Push - pull
 RX D2  PORT A10 <- AF07		Open - drain

**********************************************/

int main()
{
	RCC -> APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN;
	
	GPIOA -> MODER  |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1; //Funkcja alternatywna: PA9(TX), PA10(RX)
	GPIOA -> OTYPER |= GPIO_OTYPER_OT_10; // Open-drain dla RX
	
	GPIOA -> AFRH |= AFRH9_AF7 | AFRH10_AF7; // AF7 = 0111 (usart1)
	
	USART1 -> BRR = USARTDIV; // baudrate
	USART1 -> CR1 |= USART_CR1_UE | USART_CR1_RXNEIE | USART_CR1_TE | USART_CR1_RE; 
	
	NVIC_EnableIRQ(USART1_IRQn);

	while(1);
}

void USART1_IRQHandler()
{
	if(USART1 -> SR & USART_SR_RXNE) // warunek nie pustego buffora odbiorczego
	{
  USART1 -> SR &= ~USART_SR_RXNE;
	wprowadzane_haslo[indeks] = USART1 -> DR;
	indeks++;
		if(USART1 -> DR == 0x0A)
		{
			if((!memcmp(haslo,wprowadzane_haslo,sizeof(wprowadzane_haslo)))
				&& indeks == sizeof(wprowadzane_haslo)+2)
			{
				for (int i = 0 ; i < sizeof(zgodnosc) ; i++)
				{
					while(!(USART1 -> SR & USART_SR_TC));
					USART1 -> DR = zgodnosc[i];
				}
				while(!(USART1 -> SR & USART_SR_TC));
				USART1 -> DR = '\n';
			}
			else
			{
			USART1 -> DR = 'e';
			while(!(USART1 -> SR & USART_SR_TC));
			USART1 -> DR = '\n';
			}
			indeks=0;
		}
	}
}


