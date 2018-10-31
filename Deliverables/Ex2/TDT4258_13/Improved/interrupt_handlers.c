#include <stdint.h>

#include "timer.h"
#include "gpio.h"
#include "efm32gg.h"
#include "sound.h"
#include "dac.h"
#include "main.h"


void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
  *TIMER1_IFC = 1;
  switch(play)
  {
    case SOUND: playSound(); break;
    case MELODY: playMelody(); break;
    case FREE: break;
    case BUSY: break;
  }
}


void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
  *GPIO_IFC = *GPIO_IF;
  if(play==FREE)
  {
    *SCR = 0;

    button=filterButtons(getPressedButtons());
    update_led(button);
    if(button!=0)
    {
      switch (button) {
        case 1: {generateSineWave(&wave, 440, SAMPLE_RATE); current_sound = &wave; break;}
        case 3: {generateSquareWave(&wave, 440, SAMPLE_RATE); current_sound = &wave; break;}
        case 5: {current_sound = &start; break;}
        case 7: {current_sound = &sweep;}
      }
      n=0;
      complete = 0;
      play = SOUND;

      setupDAC();
      setupTimer(current_sound->sample_rate);
    }
  }
}

void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
  *GPIO_IFC = *GPIO_IF;
  if(play==FREE)
  {
    *SCR = 0; 

    button=filterButtons(getPressedButtons());
    update_led(button);
    if(button<=6 && button>0)
    {
      switch (button) {
        case 2: {generateSawtoothWave(&wave, 440, SAMPLE_RATE); current_sound = &wave; break;}
        case 4: {current_sound = &homer; break;}
        case 6: {current_sound = &tada;}
      }
      n=0;
      complete = 0;
      play = SOUND;
      setupDAC();
      setupTimer(current_sound->sample_rate);
    }
    else if(button==8){
      f=0;
      t=0;
      n=0;
      seconds = 0.5;
      play = MELODY;
      setupDAC();
      setupTimer(SAMPLE_RATE);
    }
  }
}
