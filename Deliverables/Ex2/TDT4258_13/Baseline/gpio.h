#include <stdint.h>

uint32_t getPressedButtons();

uint32_t filterButtons(uint32_t buttons);

void update_leds(uint32_t buttons);

void setupGPIO();

void update_led(uint32_t button);
