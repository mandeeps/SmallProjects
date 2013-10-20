// Press switch to turn on LED blinking, press again to turn off

#include <io.h>
#include <signal.h>

unsigned int timer = 1000;
unsigned int count = 0;

interrupt(TIMERA0_VECTOR) TIMERA0_ISR(void) { // Timer0 ISR
		count++;
		if (count < timer) {
			P1DIR = BIT5;
			P1OUT = BIT5;
		}
		else {
			P1DIR = BIT0;
			P1OUT = BIT0;
		}
		if (count > timer * 2) {
			count = 0;
		}
}

interrupt(PORT1_VECTOR) P1_ISR(void) {
	if((P1IFG & BIT3) == BIT3) { // switch pressed
		if (TACCR0 == 1) {
			TACCR0 = 0; // disable timer interrupt
			P1OUT = 0; // disable LED output
			_BIS_SR_IRQ(4); // enter low power mode 4
		}
		else {
			TACCR0 = 1;
			_BIS_SR_IRQ(4);
		}
	}
	P1IFG = 0x00;   // clear interrupt flags
}


void main(void) {
	WDTCTL = WDTPW + WDTHOLD; // Disable watchdog timer
    BCSCTL3 |= LFXT1S_2; // Set ACLK to use internal VLO (12kHz)
    TACTL = TASSEL_1 | MC_1; // TimerA aux clock in UP mode
    TACCTL0 = CCIE; // Enable interrupt for TACCR0 match

    // setup switch interrupt
    P1DIR &= ~BIT3;  // as input
    P1IES |= 0x01;          // interrupt on falling edge
    P1IE |= BIT3;    // interrupt enable
    eint(); // Enable interrupts
    TACCR0 = 0;
	_BIS_SR_IRQ(4); // enter low power mode 4
}
