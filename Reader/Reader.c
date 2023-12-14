/*
 * Reader.c
 *
 * Created: 2014/03/21 16:40:08
 *  Author: Chuck
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "led7seg.h"

 
#define SHIFT_PORT PORTB
#define SHIFT_RCK PORTB2
 


enum { NO_SYNC, SYNC_CANDI1, SYNC_CANDI2, SYNC_FWD, SYNC_REV };
volatile uint16_t bit_time;
volatile uint8_t gate = 1;
volatile uint8_t ones = 0;
volatile uint8_t current_bit;
volatile uint8_t state_tc =  NO_SYNC;
volatile uint8_t cnt_ones;
volatile int8_t tc_bit_ptr;
volatile const uint8_t bit_set_tbl[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
volatile uint8_t tc[8];
volatile uint8_t tc_ready;
volatile uint8_t timeCode[11];
uint16_t tmp1;
uint16_t tmp2;


ISR(TIMER1_CAPT_vect)
{
	TCCR1B ^= _BV(ICES1);	

	if (!gate) {
		ones = 1; PORTD |= (1<<PORTD7);
	} else {

		// elseif (gate) 
		bit_time = ICR1;
		TCNT1 = 0;
	
		tmp2 = bit_time>>1;
		tmp1 = bit_time>>2;
		OCR1A = tmp1;
		OCR1B = tmp1 + tmp2;
		
		current_bit = 0;
		if (ones) {
			current_bit = 1;
			ones = 0; PORTD &= ~(1<<PORTD7);
		}

		switch (state_tc) {
		case NO_SYNC:
			if (current_bit==1) cnt_ones++;
			else                cnt_ones = 0;
			if (cnt_ones==12) {
				state_tc = SYNC_CANDI1;
				cnt_ones = 0;
			}
			break;
		case SYNC_CANDI1:
			if (current_bit==0) state_tc = SYNC_CANDI2;
			else                state_tc = NO_SYNC;
			break;
		case SYNC_CANDI2:
			if (current_bit==1) { state_tc = SYNC_FWD; tc_bit_ptr = 0;  }
			else                { state_tc = SYNC_REV; tc_bit_ptr = 63; }
			break;
		case SYNC_FWD:
			tmp1 = tc_bit_ptr / 8;
			tmp2 = tc_bit_ptr % 8;
			if(tmp2==0)          tc[tmp1] = 0;
			if (current_bit==1)  tc[tmp1] |= bit_set_tbl[tmp2];
			tc_bit_ptr++;
			if(tc_bit_ptr > 63) {
				state_tc = NO_SYNC;
				tc_ready = 1;
			}
			break;
		case SYNC_REV:
			tmp1 = tc_bit_ptr / 8;
			tmp2 = tc_bit_ptr % 8;
			if(tmp2==7)          tc[tmp1] = 0;
			if (current_bit==1)  tc[tmp1] |= bit_set_tbl[tmp2];
			tc_bit_ptr--;
			if(tc_bit_ptr < 0) {
				state_tc = NO_SYNC;
				tc_ready = 1;
			}
			break;
		}

	}

}

ISR(TIMER1_COMPA_vect)
{
	gate = 0; PORTB &= ~(1<<PORTB1);
}
ISR(TIMER1_COMPB_vect)
{
	gate = 1; PORTB |= (1<<PORTB1);
}

void _shift_rck()
{  //ラッチクロックを一つ送信
	SHIFT_PORT &= ~(1<<SHIFT_RCK);
	SHIFT_PORT |= (1<<SHIFT_RCK);
}

void shift_out(uint8_t data)
{
    SPDR = data;     //データセットで送信開始
    while(!(SPSR & (1<<SPIF)));    //転送完了まで待機
    _shift_rck();   //ラッチを更新
}
 
void shift_out8(uint8_t *data)
{
	int8_t i;
	uint8_t *ptr;
	
	for (i = 0, ptr = data; i < 8; i++) {
		SPDR = *ptr++;     //データセットで送信開始
		while (!(SPSR & (1<<SPIF)))
			;    //転送完了まで待機
	}
	_shift_rck();   //ラッチを更新
}

void init_spi(){
    //SPIクロックは最速のfOSC/2
    SPCR = 0b01010000;
    SPSR = 0b00000000;
}

uint8_t deci[] = {
	LED_0, LED_1, LED_2, LED_3, LED_4,
	LED_5, LED_6, LED_7, LED_8, LED_9
};


int main(void)
{
	
	// initializer
	DDRB |= (1<<PORTB1);	// for test output
	DDRD |= (1<<PORTD7);	// for test output

	DDRB |= (1<<PORTB5);	// SCK output
	DDRB |= (1<<PORTB3);	// MOSI output
	DDRB |= (1<<PORTB2);	// SCK output
	PORTB |= (1<<PORTB0);

	TCCR1A = 0x00;
	TCCR1B = (1<<ICNC1)|(1<<ICES1)|(0<<CS12)|(1<<CS11)|(0<<CS10); // ICP noise, edge, CK/8
	TCCR1C = 0x00;
	TIMSK1 = (1<<ICIE1)|(1<<OCIE1A)|(1<<OCIE1B);
	
	TCNT1 = 0;

	init_spi();
	
	shift_out(LED_3);
	shift_out(LED_0);
	shift_out(LED_1);
	shift_out(LED_2);
	shift_out(LED_4);
	shift_out(LED_5);
	shift_out(LED_6);
	shift_out(LED_7);
	sei();

    while(1)
    {
        //TODO:: Please write your application code 
		if (tc_ready) {
/*
			timeCode[10] = (tc[0]&0x0F)+0x30;      // frames
			timeCode[9] = (tc[1]&0x03)+0x30;      // 10's of frames
			timeCode[8] =  '.';
			timeCode[7] = (tc[2]&0x0F)+0x30;      // seconds
			timeCode[6] = (tc[3]&0x07)+0x30;      // 10's of seconds
			timeCode[5] =  ':';
			timeCode[4] = (tc[4]&0x0F)+0x30;      // minutes
			timeCode[3] = (tc[5]&0x07)+0x30;      // 10's of minutes
			timeCode[2] = ':';
			timeCode[1] = (tc[6]&0x0F)+0x30;      // hours
			timeCode[0] = (tc[7]&0x03)+0x30;      // 10's of hours
*/
			timeCode[0] = deci[(tc[0]&0x0F)];      // frames
			timeCode[1] = deci[(tc[1]&0x03)];      // 10's of frames
			timeCode[2] = deci[(tc[2]&0x0F)] & LED_dot;      // seconds
			timeCode[3] = deci[(tc[3]&0x07)];      // 10's of seconds
			timeCode[4] = deci[(tc[4]&0x0F)] & LED_dot;      // minutes
			timeCode[5] = deci[(tc[5]&0x07)];      // 10's of minutes
			timeCode[6] = deci[(tc[6]&0x0F)] & LED_dot;      // hours
			timeCode[7] = deci[(tc[7]&0x03)];      // 10's of hours

			shift_out8(timeCode);

			tc_ready = 0;
		}			
    }
}