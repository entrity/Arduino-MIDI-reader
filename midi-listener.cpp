#include "midi-listener.h"

void MidiListener::poll()
{ 
	if (p_input->available()) {
		handleByte(p_input->read()); }
}

/* Callback fired when the Serial provides MidiListener with a byte of data.
	Sets this->action if not set. Else, calls handleNote or handleVolume. */
void MidiListener::handleByte(uint8_t byte)
{
	if ( note ) 			// the 3rd byte in the packet indicates the volume
		handleVolume( byte );
	else if ( action ) // the 2nd byte in the packet indicates the note
		handleNote( byte );
	else 							// the 1st byte in the packet indicates NOTE_ON or NOTE_OFF
		action = byte;
}

/* Callback fired when handleByte is called and action is set.
	Sets this-> note or fires noteOffCallback. */
void MidiListener::handleNote(uint8_t byte)
{
	if ( action == MIDI_NOTE_ON )
		note = byte;
	else if ( action == MIDI_NOTE_OFF )
		noteOffCallback( byte );
	else 	// The previous/current input packet was malformed. Treat this byte as the first byte of the packet
		action = byte;
}

/* Callback fired when handleByte is called and note is set.
	Fires noteOnCallback or noteOffCallback. Clears action and note. */
void MidiListener::handleVolume(uint8_t byte)
{
	// fire a callback for NOTE_ON or NOTE_OFF
	if (byte)	// if volume is not 0
		noteOnCallback(note, byte);
	else 			// volume of 0 is equivalent to MIDI_NOTE_OFF
		noteOffCallback(note);
	// reset
	action = note = 0;
}