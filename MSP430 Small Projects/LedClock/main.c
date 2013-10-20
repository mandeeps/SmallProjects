// LED minute clock, counts off 1 min periods, set time on power up
// by pusing button, eg if you want 5 min timer push button 5 times
// and see red led blink 5 times, then in 5 minutes it will light green

#include <io.h>
#include <signal.h>

int min = 0;
int blinks = 0;
unsigned long timer = 500;
unsigned int count = 0;
unsigned int count2 = 0;
int repeatBlinks = 0;
int takingInput = 1;
unsigned long seconds = 0;

interrupt(TIMERA0_VECTOR) TIMERA0_ISR(void) { // Timer0 ISR
	if (blinks < min) {
		count++;
		if (count < timer) {
			P1DIR = BIT0;
			P1OUT = BIT0;
		}
		else {
			P1DIR = 0;
			P1OUT = 0;
		}
		if (count > timer * 2) {
			count = 0;
			blinks++;
		}
	}
	count2++;
	if (count2 > timer * 3) {
		if (repeatBlinks < min) {
			count++;
			if (count < timer) {
				P1DIR = BIT0;
				P1OUT = BIT0;
			}
			else {
				P1DIR = 0;
				P1OUT = 0;
			}
			if (count > timer * 2) {
				count = 0;
				repeatBlinks++;
			}
		}
		else {takingInput = 0;}
	}
	if (!takingInput) {
		seconds++;
		if (seconds / 60000 == min) {
			P1DIR = BIT6;
			P1OUT = BIT6;
		}
	}
}

interrupt(PORT1_VECTOR) P1_ISR(void) {
	if (takingInput) {
		if((P1IFG & BIT3) == BIT3) { // switch pressed
			if (min < 5) {
				min++;
			}
			TACCR0 = 10;
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
    P1IES |= 0x01;   // interrupt on falling edge
    P1IE |= BIT3;    // interrupt enable
    eint(); // Enable interrupts
    TACCR0 = 0;
//	_BIS_SR_IRQ(4); // enter low power mode 4
}
