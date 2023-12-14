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


//	@brief	MIDI �V���A���|�[�g�̏��������s��
void MidiInit(void)
{
	midiUartInit();
}


//	@brief	MIDI Parser
/*!
	@brief	MIDI Parser

	@param	�Ȃ�

	@note	MIDI�V�X�e�����b�Z�[�W����͂��Asystem exclusive�͏��O���Ă���ȊO�̕K�v�ȃ��b�Z�[�W���\���̂ɋL�^����Bmain loop �Œ���I�ɌĂяo�����ƁB
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

	@param	�Ȃ�
	@return	��������MIDI Channel Message�������1

	@note	MIDI�V�X�e�����b�Z�[�W����͂��Asystem exclusive�͏��O���Ă���ȊO�̕K�v�ȃ��b�Z�[�W��MidiSystemMessage�\���̂ɋL�^����B
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

	@param	�Ȃ�
	@return	�Ȃ�

	@note	MIDI�`���l�����b�Z�[�W����͂��A�K�v�ȃ��b�Z�[�W��MidiChannelMessage�\���̂ɋL�^����B1��̃R�[���ɂ�1�o�C�g���������A�i����ԓ���midi_byte�ɂĊǗ����Ă���B
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
