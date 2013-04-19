#include "midi-listener.h"

void MidiListener::listen()
{ 
	while (1) {
		if (ARDUINO_INPUT.available()) {
			handleByte(ARDUINO_INPUT.read()); }
	}
}

/* Callback fired when the Serial provides MidiListener with a byte of data */
void MidiListener::handleByte(unsigned char byte)
{
	if ( action )
		handleNote( byte );
	else
		action = byte;
}

/* Callback fired when handleByte is called and an action exists */
void MidiListener::handleNote(unsigned char byte)
{
	// if action and note are set, then all that remains is volume
	if ( action == MIDI_NOTE_ON && note ) {
		if (byte)
			noteOnCallback( note, byte );
		else // a volume value of 0 corresponds to MIDI_NOTE_OFF
			noteOffCallback( note );
		reset();
	// if action is set but not note, then note shall be set
	} else if ( action == MIDI_NOTE_ON ) {
		note = byte;
	// if the action is MIDI_NOT_OFF, then the only byte left in the packet is the note to clear
	} else if ( action == MIDI_NOTE_OFF ) {
		noteOffCallback( byte );
	// treat this byte as the first byte of the packet
	}	else {
		action = byte;
	}
}

/* Clear action & note. Called after noteOnCallback */
void MidiListener::reset()
{
	action = note = 0;
}