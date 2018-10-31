#include <stdint.h>
#include <math.h>

#include "efm32gg.h"
#include "dac.h"
#include "gpio.h"
#include "timer.h"
#include "sound.h"
#include "sound_samples.h"

int main(void)
{
	uint32_t button;
	Sound *current_sound;
	Sound wave;

	uint16_t buffer[SAMPLE_RATE];
	wave.data = buffer;
	wave.loop = 1;

	Sound start, homer, tada, sweep;
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

	double melody_f[] = {100, 400, 200, 300, 200, 600, 400, 300, 500, 440, 480};
	uint32_t length = 11;

	setupGPIO();
	setupDAC();
	setupTimer();


	button = 0;
	while(1)
	{
		button=filterButtons(getPressedButtons());
		if(button<=8 && button>0)
		{
			update_led(button);
			if(button<=7)
			{
				switch (button) {
					case 1: {generateSineWave(&wave, 440, SAMPLE_RATE); current_sound = &wave; break;}
					case 2: {generateSawtoothWave(&wave, 440, SAMPLE_RATE); current_sound = &wave; break;}
					case 3: {generateSquareWave(&wave, 440, SAMPLE_RATE); current_sound = &wave; break;}
					case 4: {current_sound = &homer; break;}
					case 5: {current_sound = &start; break;}
					case 6: {current_sound = &tada; break;}
					case 7: {current_sound = &sweep; break;}
				}
				playSound(current_sound, button);
			}
			else playMelody(&wave, melody_f, length);
		}
		else
		{
			update_leds(button);
		}
	}

	return 0;
}
