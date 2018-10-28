#include <stdint.h>
#include "efm32gg.h"

void RAMblock(void)
{
  //Remember to reconfigure the Linker file to the smaller RAM size.
  //How much ram is this program using anyway?
  *EMU_MEMCTRL = 0b111; //Power down block 1,2,3. 32KB each.
}

void disableLFCLK(void)
{
  *CMU_LFCLKSEL = 0; //Disable LowFrequency clock A and B by writing 0 register.
}

void sleepmode(void)
{
  *SCR = 0x6; //Configre SCR to enter DEEPSLEEP and SLEEPONEXIT
}
