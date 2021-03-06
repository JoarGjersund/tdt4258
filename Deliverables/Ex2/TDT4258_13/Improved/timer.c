#include <stdint.h>
#include <math.h>

#include "efm32gg.h"

#define TIMER1_DIV (4 << 24)

void setupTimer(uint16_t period)
{
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
  *TIMER1_CTRL |= TIMER1_DIV;
  *TIMER1_TOP = round(14000000.0/(16*period));
  *TIMER1_CMD = 1;
  *TIMER1_IEN = 1;
}

void stopTimer()
{
  *CMU_HFPERCLKEN0 &= ~CMU2_HFPERCLKEN0_TIMER1;
  *TIMER1_CMD = 0;
  *TIMER1_IEN = 0;
}
