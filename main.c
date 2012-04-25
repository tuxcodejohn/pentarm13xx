#include <stdint.h>
#include "LPC13xx.h"
#include "main.h"
#include "uart.h"

volatile uint32_t msTicks = 0;

void SysTick_Handler(void) {
	msTicks++;
}

void delay_ms(uint32_t ms) {
	uint32_t now = msTicks;
	while ((msTicks-now) < ms)
		;
}

int main(void) {
	
	UARTInit(115200);
	
	SysTick_Config(SystemCoreClock/1000);

	LPC_GPIO1->DIR = (1<<0);	
	LPC_GPIO3->DIR = (1<<3)|(1<<2)|(1<<1)|(1<<0);	

	while (1) {
		LPC_GPIO1->DATA = 0;
		LPC_GPIO3->DATA = (1<<3)|(1<<2)|(1<<1)|(1<<0);
		delay_ms(100);
		LPC_GPIO3->DATA = 0;
		LPC_GPIO1->DATA = (1<<0);
		delay_ms(300);	
	

		LPC_UART->IER = IER_THRE | IER_RLS;			/* Disable RBR */
		uint8_t cmd1[] = {1,2,3}; 

		UARTSend(  cmd1, 3);
		LPC_UART->IER = IER_THRE | IER_RLS | IER_RBR;	/* Re-enable RBR */
	}
}

