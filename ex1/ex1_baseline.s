        .syntax unified
	
	      .include "efm32gg.s"

	/////////////////////////////////////////////////////////////////////////////
	//
  // Exception vector table
  // This table contains addresses for all exception handlers
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .section .vectors
	
	      .long   stack_top               /* Top of Stack                 */
	      .long   _reset                  /* Reset Handler                */
	      .long   dummy_handler           /* NMI Handler                  */
	      .long   dummy_handler           /* Hard Fault Handler           */
	      .long   dummy_handler           /* MPU Fault Handler            */
	      .long   dummy_handler           /* Bus Fault Handler            */
	      .long   dummy_handler           /* Usage Fault Handler          */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* SVCall Handler               */
	      .long   dummy_handler           /* Debug Monitor Handler        */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* PendSV Handler               */
	      .long   dummy_handler           /* SysTick Handler              */

	      /* External Interrupts */
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO even handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO odd handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler

	      .section .text

	/////////////////////////////////////////////////////////////////////////////
	//
	// Reset handler
  // The CPU will start executing here after a reset
	//
	/////////////////////////////////////////////////////////////////////////////

	      .globl  _reset
	      .type   _reset, %function
        .thumb_func
_reset:



	//load CMU base address
	ldr r1, cmu_base_addr

	// load current value of HFPERCLK ENABLE
	ldr r2, [r1, #CMU_HFPERCLKEN0]
	// set bit for GPIO clk.
	mov r3, #1			 
	lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
	orr r2, r2, r3
	// store new value
	str r2, [r1, #CMU_HFPERCLKEN0]
	
	//setup pin 8-15 for output (LED)
	// set high drive strength
	ldr r1, gpio_pa_base_addr
	mov r2, 0x2
	str r2, [r1, GPIO_CTRL]
	
	//set pins to output
	mov r4, 0x55555555
	str r4, [r1, GPIO_MODEH]
	
	// set all leds high (leds are active high)
	mov  r4, 0b1010101000000000
	str r4, [r1, GPIO_DOUT]
	
	// enable input port
	ldr r6, gpio_pc_base_addr
	
	mov r5, 0x33333333
	str r5, [r6, GPIO_MODEL]
	mov r7, 0xff
	str r7, [r6, GPIO_DOUT]
	
	// respond to input
loop1:
	bl read_button_status
	lsl r4, r4, #8
	str r4, [r1, GPIO_DOUT]
	
	      b loop1  // do nothing
	     
read_button_status:
	ldr r4, [r6, GPIO_DIN]
	bx lr
	      
cmu_base_addr:
	.long CMU_BASE
gpio_pa_base_addr:
	.long GPIO_PA_BASE
gpio_pc_base_addr:
	.long GPIO_PC_BASE
	
	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
gpio_handler:  

	mov  r4, 0b1111111000000000
	str r4, [r1, GPIO_DOUT]

	      b .  // do nothing
	
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
dummy_handler:  
        b .  // do nothing

