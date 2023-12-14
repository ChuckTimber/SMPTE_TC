/*!
	@brief	MIDI parsing library

	@file	midi.c
	@author	Satoshi "Chuck" Takagi
	@date	2009/04/03
 */


#include <inttypes.h>
#include "midi.h"

static uint8_t _midiSystemMessage(void);
static void _midiChannelMessage(void);

static uint8_t rb;				//!< read byte from UART
static uint8_t midi_stat = 255;	//!< MIDI parsing status initial value
static uint8_t midi_ch;			//!< MIDI receive channel from stream
static uint8_t midi_byte[3];	//!< MIDI parsing work array


//	@brief	MIDI シリアルポートの初期化を行う
void MidiInit(void)
{
	midiUartInit();
}


//	@brief	MIDI Parser
/*!
	@brief	MIDI Parser

	@param	なし

	@note	MIDIシステムメッセージを解析し、system exclusiveは除外してそれ以外の必要なメッセージを構造体に記録する。main loop で定期的に呼び出すこと。
*/

void MidiParse(void)
{
	int16_t c;

    rb = c = midiUartGetByte();
    if(c != -1){
		if( _midiSystemMessage() ){
			_midiChannelMessage();
		}
	}
}



/*!
	@brief	MIDI System Message parser

	@param	なし
	@return	引き続くMIDI Channel Messageがあれば1

	@note	MIDIシステムメッセージを解析し、system exclusiveは除外してそれ以外の必要なメッセージをMidiSystemMessage構造体に記録する。
*/
static uint8_t _midiSystemMessage(void)
{
	static uint8_t sysex;

	if (sysex) {					// in sysex process
		if (rb == 0xF7) sysex=0;	// end of sysex process
	} else {
		if (rb < 0xF8) { 
			if (rb >= 0xF0) {		// common msg
				if (rb == 0xF0)	sysex = 1;	// start of sysex process
				midi_stat = 100;
			} else if (rb > 0x7F) {	// voice msg
				midi_ch = rb & 0x0F;
				//if (midi_ch == midich_rx) { // for multi-channel recieve
					midi_stat = 0;
				//}
			} else {				// store midi data byte 00-7F
				midi_byte[midi_stat & 0x01] = rb;
 			}
 			return 1;
 		} else {					// realtime msg >= 0xF8
			switch (rb) {
			case 0xF8:	// timing clock
			case 0xF9:	
			case 0xFA:	// start
			case 0xFB:	// continue
			case 0xFC:	// stop
			case 0xFD:	
			case 0xFE:	// active sensing
			case 0xFF:	// system reset
					break;
			}
		}
	}

	return 0;
}

/*!
	@brief	MIDI Channel Message parser

	@param	なし
	@return	なし

	@note	MIDIチャネルメッセージを解析し、必要なメッセージをMidiChannelMessage構造体に記録する。1回のコールにつき1バイトずつ処理し、進捗状態等をmidi_byteにて管理している。
*/
static void _midiChannelMessage(void)
{
	switch (midi_stat) {
	case 0:
		switch ( rb & 0xF0 ) {
#ifdef USE_MIDI_CC
		case MIDI_CC				: midi_byte[2] = rb; midi_stat=40; break;
#endif // USE_MIDI_CC
		default:
			midi_stat=255;
		}
		break;

#ifdef USE_MIDI_CC
	/* Control Change */
	case 40: midi_stat=41; break;
	case 41: midi_stat=40;			// data and for running status
			MidiChannelMessage.Channel = midi_ch;
			MidiChannelMessage.ControlChange.Control = midi_byte[0];
			MidiChannelMessage.ControlChange.Value = midi_byte[1];
			MidiChannelMessage.Status |= fMidiControlChange;
			break;
#endif // USE_MIDI_CC

	default: break;
	}

}

void MidiSendSystemMsg(void)
{
/*
	if (MidiSystemMessage.Status & fMidiTimingClock) {
		midiUartSendByte(0xF8);
		MidiSystemMessage.Status &= ~fMidiTimingClock;
	}	
	if (MidiSystemMessage.Status & fMidiStart) {
		midiUartSendByte(0xFA);
		MidiSystemMessage.Status &= ~fMidiStart;
	}
	if (MidiSystemMessage.Status & fMidiStop) {
		midiUartSendByte(0xFC);
		MidiSystemMessage.Status &= ~fMidiStop;
	}
*/
}
