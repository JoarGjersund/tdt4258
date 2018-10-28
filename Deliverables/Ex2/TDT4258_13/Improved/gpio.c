#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/*
 * function to set up GPIO mode and interrupts
 */
void setupGPIO(void)
{
	/*
	 * TODO set input and output pins for the joystick
	 */

	/*
	 * Example of HW access from C code: turn on joystick LEDs D4-D8 check
	 * efm32gg.h for other useful register definitions
	 */
	// Setup LEDs
	*CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO;	// enable GPIO clock
	*GPIO_PA_CTRL = 1;	// set Lowest drive strength
	*GPIO_PA_MODEH = 0x55555555;	// set pins A8-15 as output
	*GPIO_PA_DOUT = 0xfE00;	// turn off LEDs (LEDs are active low), but keep one led on for fun.

	// Setup buttons
	*GPIO_PC_MODEL = 0x33333333;	// set button pins to push pull with filter
	*GPIO_PC_DOUT = 0xff;	// enable internal pull up

	// Setup button Interrupts
	*GPIO_EXTIPSELL = 0x22222222;	// Select external interrupt port -> Port C pins 0-7
	*GPIO_EXTIRISE = 0xff;	// Select interrupt trigger on rising edge for pins 0-7
	*GPIO_EXTIFALL = 0xff;	// Select interrupt trigger on falling edge for pins 0-7
	*GPIO_IEN = 0xff;	// Enable external interrupt for pins 0-7
}

bool read_button(int btn)
{
	//*GPIO_PA_DOUT = *GPIO_PC_DIN << 8;

	return (*GPIO_PC_DIN >> btn) & 1;
}

bool isButtonPushed(int button)
{
	return !((*GPIO_PC_DIN >> button) & 1);
}
