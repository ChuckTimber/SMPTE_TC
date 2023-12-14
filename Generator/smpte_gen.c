/*
 * smpte_gen.c
 *
 * ----------------
 * http://harleyhacking.blogspot.fr/2011/06/avr-smpte-ltc-audio-time-code-generator.html
 * This uses OCRA to generate a SMPTE time code waveform starting at 0 using a 16 bit counter.
 * The timing is set for a 12Mhz ceramic resonator, ports are set for a ATtiny4313.  Released under GPLv3
 * ----------------
 *
 * Created: 2014/03/15 9:59:07
 *  Author: Chuck
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <string.h>
#define CPUFREQ 12000000

unsigned char frame = 0, secs = 0, mins = 0, hour = 0;
void incsmpte()
{
	frame++;
	if ((frame & 0x0f) > 9)
	frame += 6;
	if (frame < 0x30)
	return;
	frame = 0;
	secs++;
	if ((secs & 0x0f) > 9)
	secs += 6;
	if (secs < 0x60)
	return;
	secs = 0;
	mins++;
	if ((mins & 0x0f) > 9)
	mins += 6;
	if (mins < 0x60)
	return;
	mins = 0;
	hour++;
	if ((hour & 0x0f) > 9)
	hour += 6;
	if (hour < 0x24)
	return;
	hour = 0;
}

unsigned char user[8] = {2,4,6,8,1,3,5,7};
static unsigned char block[10];
void fillblock()
{
    block[0] = (user[0] << 4) | ( frame & 0xf );
    block[1] = (user[1] << 4) | ( frame >> 4 );
    block[2] = (user[2] << 4) | ( secs & 0xf );
    block[3] = (user[3] << 4) | ( secs >> 4 );
    block[4] = (user[4] << 4) | ( mins & 0xf );
    block[5] = (user[5] << 4) | ( mins >> 4 );
    block[6] = (user[6] << 4) | ( hour & 0xf );
    block[7] = (user[7] << 4) | ( hour >> 4 );
    block[8] = 0xfc;
    block[9] = 0xbf;

    unsigned char par, i;
    par = 1; // last two constants
    for (i = 0; i < 8; i++)
        par ^= block[i];
    par ^= par >> 4;
    par ^= par >> 2;
    par ^= par >> 1;
    if (par & 1)
        block[3] |= 8;
}

unsigned char step = 0;
unsigned char oneflg = 0;
#define WIDONE (CPUFREQ/4800)
ISR(TIMER1_COMPA_vect)
{
    if (oneflg) {
        OCR1A += WIDONE;
        oneflg = 0;
        return;
    }
    if (1 & (block[step >> 3] >> (step & 7)))
        oneflg = 1;
    if (oneflg)
        OCR1A += WIDONE;
    else
        OCR1A += WIDONE*2;
    step++;
    if (step == 80)
        step = 0;
}

unsigned hitime = 0;
ISR(TIMER1_OVF_vect)
{
    hitime++;
}

ISR(TIMER1_CAPT_vect)
{
}

/**************************************************************************/
int main(void)
{
    MCUCR = 0x20;               // enable sleep as idle
#if defined(__AVR_ATtiny2313a__) || defined(__AVR_ATtiny4313__)
    PRR = 0x06;                 // shut down USI and Timer 0
    BODCR = 3;                  // disable brown-out detect during sleep
    BODCR = 2;
#endif
    DIDR = 3;
    ACSR = 0x80;

    // Select clock, etc for timer |1 = xtal/1, |2 = xtal/8,,,
    GTCCR = 0x81;               // stop for reset - normally to sync multiples
    TCNT1 = 0;
    TCCR1A = 0xA0;
    TCCR1C = 0xC0;              // force clear
    TCCR1A = 0x50;              // now toggle
    TCCR1B = 0xC0 | 1;          // NoiseC, edge ...  off,1,8,64,256,1024,ext
    GTCCR = 0x80;               // start

    TIFR |= 0xC8;               // reset interrupt triggers
    TIMSK |= 0xC8;              // enable interrupt
    DDRB = 0x18;
 
// Not adjusted for CPU Freq - not using UART yet
    UBRRL = 12;                 //div 13 for 115200 baud from 12MHz
    UBRRH = 0;

    UCSRC = 0x06;               //8N1 (should be this from reset)
    UCSRA = 0xE2;               // clear Interrupts, UART at 2x (xtal/8)
    UCSRB = 0x18;               // oring in 0x80 would enable rx interrupt

    sei();

    for (;;) {
        sleep_mode();           // INT will waken
        if (step)
            continue;
        incsmpte();
        fillblock();
    }
}
