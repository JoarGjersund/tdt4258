#include <stdint.h>

#include "efm32gg.h"
#include "dac.h"
#include "gpio.h"
#include "timer.h"
#include "sound.h"
#include "sound_samples.h"

volatile State play = BUSY;
volatile uint32_t button=0, t=0, n=0, f=0, complete=0, hold=0;
volatile double seconds=0.5;

volatile Sound *current_sound, wave, start, homer, tada, sweep;
volatile double freq[] = {100, 400, 200, 300, 200, 600, 400, 300, 500, 440, 480};
volatile uint32_t length = 11;

void setupNVIC()
{
	*ISER0 |= 1 << 1;	// Enable interrupt request in NVIC for interrupt line 1 GPIO_EVEN
	*ISER0 |= 1 << 11;	// Enable interrupt request in NVIC for interrupt line 1 GPIO_ODD
	*ISER0 |= 1 << 12;	// Enable interrupt request in NVIC for interrupt line 12 TIMER1
}

int main(void)
{
	uint16_t buffer[SAMPLE_RATE];
	wave.data = buffer;
	wave.loop = 1;

	start.data = data_start;
	homer.data = data_homer;
	tada.data = data_tada;
	sweep.data = data_sweep;

	start.length = START_LEN;
	homer.length = HOMER_LEN;
	tada.length = TADA_LEN;
	sweep.length = SWEEP_LEN;

	start.sample_rate = homer.sample_rate = tada.sample_rate = sweep.sample_rate = 44100;
	start.loop = homer.loop = tada.loop = sweep.loop = 0;

	setupGPIO();
	setupNVIC();

	uint32_t skip_interrupt_noise = 0;

	while(skip_interrupt_noise < 10000) skip_interrupt_noise++;

	play = FREE;
	*SCR = 0x6;
	while(1)
	{
		__asm__("wfi\n\t");
	}

	return 0;
}
