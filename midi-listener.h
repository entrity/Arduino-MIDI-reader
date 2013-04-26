/*
	Module for getting Midi events from arduino serial pin, then passing the 
	'on' and 'off' events to given functions.

	Usage:
	==============================================================

	void myCallback1(uint8_t note, uint8_t vol);
	void myCallback2(uint8_t note);

	SoftwareSerial mySerial( 10, 11 );
	MidiListener myListener( &mySerial, myCallback1, myCallback2 );

	void setup() { mySerial.begin(31250); }

	void loop() { myListener.poll(); }
*/

#ifndef MJ_MIDI_LISTENER
#define MJ_MIDI_LISTENER

#include <Arduino.h>
#include "midi-vars.h"

class MidiListener
{
	Stream * p_input; // the HardwareSerial or SoftwareSerial where MidiListener gets MIDI input
	uint8_t
		action, // holds the first byte of a MIDI signal
		note; 	// holds the second byte of a MIDI signal
	void(*noteOnCallback)(uint8_t, uint8_t); 	// callback fired by handleNote if the MIDI input initiates a note
	void(*noteOffCallback)(uint8_t); 					// callback fired by handleNote if the MIDI input terminates a note

	/* Callback fired when the Serial provides MidiListener with any byte */
	void handleByte(uint8_t byte);
	/* Callback fired when the Serial provides MidiListener with the packet's 2nd byte */
	void handleNote(uint8_t byte);
	/* Callback fired when the Serial provides MidiListener with the packet's 3rd byte */
	void handleVolume(uint8_t byte);

public:

	/* Constructor */
	MidiListener(
		Stream & p_input,
		void(*noteOnCallback) (uint8_t, uint8_t),
		void(*noteOffCallback)(uint8_t)
		) : p_input(&p_input), noteOnCallback(noteOnCallback), noteOffCallback(noteOffCallback) {}

	/* Query Serial for MIDI input. Process any available data. */
	void poll();
};

#endif