/*
  Listen to midi input through a SoftwareSerial port. 
  If a wrong note is played, HardwareSerial outputs "fail."
  If the song is played successfully, HardwareSerial outputs "success."
  Every time a MIDI_NOTE_ON signal is received, HardwareSerial outputs "note on #{note}."
*/

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "midi-listener.h"
#include "midi-song.h"

/* callback for MidiListener */
void noteOn( unsigned char note, unsigned char volume );
/* callback for MidiListener */
void noteOff( unsigned char note );
/* callback for successful completion of song */
void onSongComplete();
/* callback for wrong note in song */
void onSongFail();


SoftwareSerial mySerial(10,11);
MidiListener midiListener(mySerial, noteOn, noteOff);

/* Array of midi events to make a song */
MjMidi::event data_short_midiEvents[] = {
  {  0,MIDI_NOTE_ON , 57},{  0,MIDI_NOTE_ON , 52},{  0,MIDI_NOTE_ON , 45},{240,MIDI_NOTE_OFF, 57},{  0,MIDI_NOTE_OFF, 52},{  0,MIDI_NOTE_OFF, 45},{  0,MIDI_NOTE_ON , 52},{240,MIDI_NOTE_OFF, 52},{  0,MIDI_NOTE_ON , 57},{240,MIDI_NOTE_OFF, 57},{  0,MIDI_NOTE_ON , 59},{240,MIDI_NOTE_OFF, 59},{  0,MIDI_NOTE_ON , 64},{240,MIDI_NOTE_OFF, 64},{  0,MIDI_NOTE_ON , 59},{240,MIDI_NOTE_OFF, 59},{  0,MIDI_NOTE_ON , 57},{240,MIDI_NOTE_OFF, 57},{  0,MIDI_NOTE_ON , 60},{240,MIDI_NOTE_OFF, 60},{  0,MIDI_NOTE_ON , 64},{  0,MIDI_NOTE_ON , 60},{240,MIDI_NOTE_OFF, 64},{  0,MIDI_NOTE_OFF, 60},{240,MIDI_NOTE_ON , 57},{240,MIDI_NOTE_OFF, 57},{  0,MIDI_NOTE_ON , 57},{240,MIDI_NOTE_OFF, 57},{  0,MIDI_NOTE_ON , 55},{240,MIDI_NOTE_OFF, 55},{  0,MIDI_NOTE_ON , 55},{240,MIDI_NOTE_OFF, 55},{  0,MIDI_NOTE_ON , 55},{240,MIDI_NOTE_OFF, 55}
};
/* A midi song */
MjMidi::Song song(&data_short_midiEvents[0], 34);


/* Arduino-required function */
void setup()
{
  Serial.begin(9600);
  mySerial.begin(MIDI_BAUDRATE);
  song.setCallbacks( onSongComplete, onSongFail );
}

/* Arduino-required function */
void loop()
{
  midiListener.poll();
}

/* Send data to song */
void noteOn( unsigned char note, unsigned char volume ) {
  Serial.println("note on");
  song.attempt(MIDI_NOTE_ON, note);
  Serial.println();
}
/* Send data to song */
void noteOff( unsigned char note ) {  song.attempt(MIDI_NOTE_OFF, note); }
/* Print success message */
void onSongComplete() { Serial.println("success"); }
/* Print fail message */
void onSongFail() { Serial.println("fail"); }
