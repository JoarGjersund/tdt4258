#include <stdint.h>
#include "efm32gg.h"

typedef struct Sound {
  uint32_t length;
  uint32_t sample_rate;
  uint8_t loop;
  uint16_t *data;
} Sound;


void generateSineWave(Sound *wave, double analog_freq, double sample_rate);

void generateSawtoothWave(Sound *wave, double analog_freq, double sample_rate);

void generateSquareWave(Sound *wave, double analog_freq, double sample_rate);

void playSound(Sound *wave, uint32_t button);

void playMelody(Sound *wave, double *freq, uint32_t length);
