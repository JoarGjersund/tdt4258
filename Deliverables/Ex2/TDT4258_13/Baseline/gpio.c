#include <stdint.h>

#include "efm32gg.h"


void setupGPIO()
{
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;	// enable GPIO clock
	*GPIO_PA_CTRL = 2;	// set high drive strength
	*GPIO_PA_MODEH = 0x55555555;	// set pins A8-15 as output
	*GPIO_PA_DOUT = 0xff00;	// turn off LEDs (LEDs are active low)

	// Setup buttons
	*GPIO_PC_MODEL = 0x33333333;	// set button pins to push pull with filter
	*GPIO_PC_DOUT = 0xff;	// enable internal pull up
}

uint32_t getPressedButtons()
{
	return (*GPIO_PC_DIN)^0xff;
}

uint32_t filterButtons(uint32_t buttons)
{
	uint32_t button=0;
	uint32_t mask=1;
	if(buttons!=0)
	{
		while(!(buttons&(mask << button))) button++;
		return button+1;
	}
	return button = 0;
}

void update_led(uint32_t button)
{
	uint32_t mask = (1 << (button-1));
	*GPIO_PA_DOUT = (mask^0xff) << 8;
}

void update_leds(uint32_t buttons)
{
	*GPIO_PA_DOUT = (buttons^0xff) << 8;
}
