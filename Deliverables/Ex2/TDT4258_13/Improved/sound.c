#include <math.h>
#include "sound.h"
#include "dac.h"
#include "timer.h"
#include "gpio.h"
#include "efm32gg.h"
#include "main.h"


void generateSineWave(Sound *wave, double analog_freq, double sample_rate)
{
	uint32_t n = 0;
	uint32_t length = round(sample_rate/analog_freq);
	double f = analog_freq/sample_rate;

	for(n=0; n<length; n++)
	{
		wave->data[n] = round(1024*sin(2*PI*f*n)) + 2048;
	}
	wave->length = length;
	wave->sample_rate = sample_rate;
}

void generateSawtoothWave(Sound *wave, double analog_freq, double sample_rate)
{
	uint32_t n = 0;
	uint32_t length = round(sample_rate/analog_freq);
	double m = analog_freq/sample_rate;

	for(n=0; n<length; n++)
	{
		wave->data[n] = round(2048*m*n);
	}
	wave->length = length;
	wave->sample_rate = sample_rate;
}

void generateSquareWave(Sound *wave, double analog_freq, double sample_rate)
{
	uint32_t n = 0;
	uint32_t length = round(sample_rate/analog_freq);

	for(n=0; n<length/2; n++)
	{
		wave->data[n] = 4095;
	}
	for(n=length/2; n<length; n++)
	{
		wave->data[n] = 0;
	}
	wave->length = length;
	wave->sample_rate = sample_rate;
}

void sendStereoData(uint16_t l, uint16_t r)
{
	*DAC0_CH0DATA = l;
	*DAC0_CH1DATA = r;
}

void playSound()
{
	if(button!=filterButtons(getPressedButtons()) || (current_sound->loop==0 && complete==1))
	{
		stopTimer();
		stopDAC();
		update_leds(0);
		*SCR = 0x6;
		play = FREE;
	}
	else
	{
		n%=current_sound->length;
		sendStereoData(current_sound->data[n], current_sound->data[n]);
		n++;
		if(n==current_sound->length) complete = 1;
	}
}

void playMelody()
{
	if(f==length)
	{
		stopTimer();
		stopDAC();
		update_leds(0);
		*SCR = 0x6;
		play = FREE;
	}
	else
	{
		if(t==0 || t==round(SAMPLE_RATE*seconds))
		{
			generateSineWave(&wave, freq[f], SAMPLE_RATE);
			current_sound = &wave;
			t = 0;
			f++;
			n = 0;
		}
		n%=current_sound->length;
		sendStereoData(current_sound->data[n], current_sound->data[n]);
		n++;
		t++;
	}
}
