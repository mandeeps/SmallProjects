// LED die, randomly blinks 1-6 times on button press
#include <io.h>
#include <signal.h>

#define	 LED1		 BIT0
#define	 LED_DIR	  P1DIR
#define	 LED_OUT	  P1OUT
#define	 SWITCH_BIT	   BIT3
#define	 SWITCH_DIR   P1DIR
#define	 SWITCH_IE	P1IE	  // interrupt enable
#define	 SWITCH_ES	P1IES	 // interrupt edge select

unsigned int count = 0;
unsigned int timer = 2000;
int rnd;
int rollcount = 0;
int roll = 0;
static volatile uint16_t ticks = 0;             // tick counter
static volatile uint16_t lastPressTicks = 0;    // tick count of last button press

interrupt(TIMERA0_VECTOR) TIMERA0_ISR(void) { // Timer0 ISR
	ticks++;
	if (roll && (rollcount < rnd)) {
		count++;
		if (count < timer) {
			LED_DIR = LED1;
			LED_OUT = LED1;
		}
		else {
			LED_DIR = 0;
			LED_OUT = 0;
		}
		if (count > timer * 2) {
			count = 0;
			rollcount++;
		}
	}
	else if (rollcount >= rnd) {
		rnd = 0;
		rollcount = 0;
		roll = 0;
	}
}

interrupt(PORT1_VECTOR) P1_ISR(void) {
	if((P1IFG & BIT3) == SWITCH_BIT) { // switch pressed
		if (lastPressTicks == 0) {
			lastPressTicks = ticks;
			srand(ticks);
		}
		else {lastPressTicks = 0;}
		rnd = (rand() % 6) + 1;
		if (rnd < 1) {rnd = -rnd;}
		if (rnd == 0) {rnd =1;}
		roll = 1;
	}
	P1IFG = 0x00;   // clear interrupt flags
}

void main(void) {
	WDTCTL = WDTPW + WDTHOLD; // Disable watchdog timer
    BCSCTL3 |= LFXT1S_2; // Set ACLK to use internal VLO (12kHz)
    TACTL = TASSEL_1 | MC_1; // TimerA aux clock in UP mode
    TACCTL0 = CCIE; // Enable interrupt for TACCR0 match
    TACCR0 = 1; // Set TACCR0, starts timer

    // setup switch interrupt
    SWITCH_DIR &= ~SWITCH_BIT;  // as input
    SWITCH_ES |= 0x01;          // interrupt on falling edge
    SWITCH_IE |= SWITCH_BIT;    // interrupt enable
    eint(); // Enable interrupts
}
