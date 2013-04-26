/*
  Requires that you have the files for:
    #include "sounds/data-short.c"
    #include "sounds/epona.c"
    #include "sounds/fifties-short.c"
  ...each of which defines a song.

  This example creates a SongBank and passes MIDI events to every one of its songs,
  each of which has a callback which fires on completion, printing "COMPLETED SONG #{song name}"
  to the HardwareSerial.
*/

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "midi-listener.h"
#include "midi-song.h"

/* callback for MidiListener */
void noteOn( unsigned char note, unsigned char volume );
/* callback for MidiListener */
void noteOff( unsigned char note );

/* Build Stream input to receive MIDI signals */
SoftwareSerial mySerial(10,11);
/* Build MidiListener to handle MIDI signals */
MidiListener midiListener(mySerial, noteOn, noteOff);

/* include songs */
#include "sounds/data-short.c"
#include "sounds/epona.c"
#include "sounds/fifties-short.c"
/* Array of song pointers to make a songbank */
MjMidi::Song * song_pointers[] = { &data_short, &epona, &fifties_short };
/* Build a midi songbank */
MjMidi::SongBank songBank(&song_pointers[0], 3);

/* Song completion callbacks */
void complete_data_short() { Serial.println("COMPLETED SONG data_short"); }
void complete_epona() { Serial.println("COMPLETED SONG epona"); }
void complete_fifties_short() { Serial.println("COMPLETED SONG fifties_short"); }


/* Arduino-required function */
void setup()
{
  Serial.begin(9600);
  mySerial.begin(MIDI_BAUDRATE);
  /* Set callbacks for songs */
  data_short.setCallbacks( complete_data_short, NULL );
  epona.setCallbacks( complete_epona, NULL );
  fifties_short.setCallbacks( complete_fifties_short, NULL );
}

/* Arduino-required function */
void loop()
{
  midiListener.poll();
}

/* Send data to song */
void noteOn( unsigned char note, unsigned char volume ) {
  Serial.println("note on");
  songBank.handleMidiEvent(MIDI_NOTE_ON, note);
  Serial.println();
}
/* Send data to song */
void noteOff( unsigned char note ) {
  songBank.handleMidiEvent(MIDI_NOTE_OFF, note);
}
