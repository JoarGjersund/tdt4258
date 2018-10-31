#include <math.h>
#include "sound.h"
#include "timer.h"
#include "gpio.h"
#include "efm32gg.h"

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

void playSound(Sound *wave, uint32_t button)
{
	uint32_t n=0;
	uint8_t complete = 0;
	startTimer(wave->sample_rate);
	while(button==filterButtons(getPressedButtons()))
	{
		if(*TIMER1_IF==1 && !(wave->loop==0 && complete==1))
		{
      n%=wave->length;
    	sendStereoData(wave->data[n], wave->data[n]);
      n++;
			if(n==wave->length) complete = 1;
			*TIMER1_IFC = 1;
		}
	}
	stopTimer();
}

void playMelody(Sound *wave, double *freq, uint32_t length)
{
	uint32_t n, f;
	double seconds = 0.5;
	uint32_t t;

	for(f=0; f<length; f++)
	{
		generateSineWave(wave, freq[f], SAMPLE_RATE);
		startTimer(SAMPLE_RATE);
		t = 0;
		n = 0;
		while(t<round(SAMPLE_RATE*seconds))
		{
			if(*TIMER1_IF==1)
			{
				n%=wave->length;
				sendStereoData(wave->data[n], wave->data[n]);
				n++;
				t++;
				*TIMER1_IFC = 1;
			}
		}
		stopTimer();
	}
}
