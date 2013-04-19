/*
	Module for getting Midi events from arduino serial pin, then passing the 
	'on' and 'off' events to given functions.
*/

#ifndef MJ_MIDI_LISTENER
#define MJ_MIDI_LISTENER

#include <Arduino.h>

#define MIDI_BAUDRATE 31250 // not used
#define MIDI_NOTE_ON 144 // midi code for note on
#define MIDI_NOTE_OFF 128 // midi code for note off

class MidiListener
{
	Stream * p_input; // the HardwareSerial or SoftwareSerial where MidiListener gets MIDI input
	unsigned char		
		action, // holds the first byte of a MIDI signal
		note; 	// holds the second byte of a MIDI signal
	void(*noteOnCallback)(uint8_t, uint8_t), // callback fired by handleNote if the MIDI input initiates a note
	void(*noteOffCallback)(uint8_t, uint8_t), // callback fired by handleNote if the MIDI input terminates a note

public:

	/* Constructor */
	MidiListener(Stream * p_input,
		void(*noteOnCallback)(uint8_t, uint8_t),
		void(*noteOffCallback)(uint8_t)
		) : p_input(p_input), noteOnCallback(noteOnCallback), noteOffCallback(noteOffCallback) {}

	/* Callback fired when handleByte is called and an action exists */
	void handleByte(unsigned char byte);
	/* Callback fired when the Serial provides MidiListener with a byte of data */
	void handleNote(unsigned char byte);

	/* Start loop that repeatedly polls p_input for MIDI input */
	void listen();
	/* Clear action & note. Called after noteOnCallback */
	void reset();
};

#undef MIDI_NOTE_ON
#undef MIDI_NOTE_OFF

#endif