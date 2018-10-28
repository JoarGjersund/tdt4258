#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "efm32gg.h"

/*
 * TODO calculate the appropriate sample period for the sound wave(s) you
 * want to generate. The core clock (which the timer clock is derived
 * from) runs at 14 MHz by default. Also remember that the timer counter
 * registers are 16 bits.
 */
/*
 * The period between sound samples, in clock cycles
 */
#define TIMER_CLK_FREQUENCY 14000000	//counts per sec.
#define SAMPLE_FREQUENCY 	44100	// samples per sec.
#define SAMPLE_PERIOD		TIMER_CLK_FREQUENCY/SAMPLE_FREQUENCY	// counts per sample.


//Declaration of Power Saving functions
void RAMblock();
void disableLFCLK();
void sleepmode();
void __WFI()	//Wait for Interrupt - Enter DeepSleep mode

 //Declaration of peripheral setup functions
void setupTimer(uint32_t period);
void setupDAC();
void setupNVIC();
void setupGPIO();


int main(void)
{
	// Call power saving functions
	RAMblock();
	disableLFCLK();
	sleepmode();
	/*
	 * Call the peripheral setup functions
	 */
	setupGPIO();
	setupDAC();
	setupTimer(SAMPLE_PERIOD);

	setupNVIC();		// Enable interrupt handling


	__WFI();  // Wait for Interrupt

	while (1) ;
	return 0;
}

void setupNVIC()
{

	*ISER0 |= 1 << 1;	// Enable interrupt request in NVIC for interrupt line 1 GPIO_EVEN
	*ISER0 |= 1 << 11;	// Enable interrupt request in NVIC for interrupt line 1 GPIO_ODD
	*ISER0 |= 1 << 12;	// Enable interrupt request in NVIC for interrupt line 12 TIMER1
}

/*
 * if other interrupt handlers are needed, use the following names:
 * NMI_Handler HardFault_Handler MemManage_Handler BusFault_Handler
 * UsageFault_Handler Reserved7_Handler Reserved8_Handler
 * Reserved9_Handler Reserved10_Handler SVC_Handler DebugMon_Handler
 * Reserved13_Handler PendSV_Handler SysTick_Handler DMA_IRQHandler
 * GPIO_EVEN_IRQHandler TIMER0_IRQHandler USART0_RX_IRQHandler
 * USART0_TX_IRQHandler USB_IRQHandler ACMP0_IRQHandler ADC0_IRQHandler
 * DAC0_IRQHandler I2C0_IRQHandler I2C1_IRQHandler GPIO_ODD_IRQHandler
 * TIMER1_IRQHandler TIMER2_IRQHandler TIMER3_IRQHandler
 * USART1_RX_IRQHandler USART1_TX_IRQHandler LESENSE_IRQHandler
 * USART2_RX_IRQHandler USART2_TX_IRQHandler UART0_RX_IRQHandler
 * UART0_TX_IRQHandler UART1_RX_IRQHandler UART1_TX_IRQHandler
 * LEUART0_IRQHandler LEUART1_IRQHandler LETIMER0_IRQHandler
 * PCNT0_IRQHandler PCNT1_IRQHandler PCNT2_IRQHandler RTC_IRQHandler
 * BURTC_IRQHandler CMU_IRQHandler VCMP_IRQHandler LCD_IRQHandler
 * MSC_IRQHandler AES_IRQHandler EBI_IRQHandler EMU_IRQHandler
 */
