/*
	Module for getting Midi events from arduino serial pin, then passing the 
	'on' and 'off' events to given functions.
*/

#ifndef MJ_MIDI_LISTENER
#define MJ_MIDI_LISTENER

#define ARDUINO_INPUT Serial1
#define NOTE_ON 144 // midi code for note on
#define NOTE_OFF 128 // midi code for note off

class MidiListener
{
	public:
		unsigned char action, note;
		void handleByte(unsigned char byte);
		void handleNote(unsigned char byte);
		void (* handleNoteOn)(unsigned char note, unsigned char volume);
		void (* handleNoteOff)(unsigned char note);
		void listen();
		void reset();
		static MidiListener * init( void (* p_noteOn)(unsigned char, unsigned char), void (* p_noteOff)(unsigned char) );
};

MidiListener * MidiListener::init( void (* p_noteOn)(unsigned char, unsigned char), void (* p_noteOff)(unsigned char) )
{
	MidiListener * listener = new MidiListener();
	listener->handleNoteOn = p_noteOn;
	listener->handleNoteOff = p_noteOff;
	return listener;
}

void MidiListener::listen()
{
	if (ARDUINO_INPUT.available()) { handleByte(ARDUINO_INPUT.read()); }
}

void MidiListener::handleByte(unsigned char byte)
{
	if ( action )
		handleNote( byte );
	else
		action = byte;
}

void MidiListener::handleNote(unsigned char byte)
{
	if ( action == 144) {
		if ( note )
			{ handleNoteOn( note, byte ); reset(); }
		else
			{ note = byte; }
	} else if ( action == 128 ) {
		handleNoteOff( byte );
	}	else
		action = byte;
}

void MidiListener::reset()
{
	action = note = 0;
}

#undef NOTE_ON
#undef NOTE_OFF

#endif