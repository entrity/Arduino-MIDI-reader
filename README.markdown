1. Use midi.rb (and supply it a filename) to turn a midi file into string that represents an array of unsigned char in C.
2. Make that array into a global var in the Arduino app (a la midi_mega_listen).
3. Use function mj_midi::Song::fromArray on that array to create a Song.
4. Set an appropriate function for that Song's 'onComplete' and 'onFail' values.
5. Create a listener, have it listen in the loop.
6. Set noteOn and noteOff callbacks for the listener which pass the midi events on to the song.


/*************************** e.g. *******************************/

MidiListener * midiListener;
Song * song;

unsigned char songData[] = { 16, 3, 57, 79, 52, 79, 45, 79, 4, 57, 0, 52, 0, 45, 0, 52, 79, 2, 52, 0, 57, 79, 2, 57, 0, 59, 79, 2, 59, 0, 64, 79, 2, 64, 0, 59, 79, 2, 59, 0, 57, 79, 2, 57, 0, 60, 79, 3, 60, 0, 64, 79, 60, 79, 2, 64, 0, 60, 0, 1, 57, 79, 2, 57, 0, 57, 79, 2, 57, 0, 55, 79, 2, 55, 0, 55, 79, 2, 55, 0, 55, 79, 1, 55, 0 };

void noteOn( unsigned char note, unsigned char volume ) { /* callback for MidiListener */ song->digest(note, volume); }
void noteOff( unsigned char note ) { /* callback for MidiListener */ song->digest(note, 0); }
void onFail() { /* callback for Song, when user causes a midi event that doesn't match song */ }
void onComplete() { /* callback for Song, when user finishes playing song on midi event */ }

void setup()
{
	midiListener = MidiListener::init( noteOn, noteOff );
	song = Song::fromArray( songData );
	song->onFail = onFail;
	song->onComplete = onComplete;
}

void loop()
{
	midiListener.listen();
}