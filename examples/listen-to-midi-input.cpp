/*
  Listen to midi input through a SoftwareSerial port.
  When midi event occurs, HardwareSerial prints the name of the event and the note.
*/

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "midi-listener.h"

/* callback for MidiListener */
void noteOn( unsigned char note, unsigned char volume );
/* callback for MidiListener */
void noteOff( unsigned char note );

SoftwareSerial mySerial(10,11);
MidiListener midiListener(mySerial, noteOn, noteOff);

/* Arduino-required function */
void setup()
{
  Serial.begin(9600);
  mySerial.begin(MIDI_BAUDRATE);
}

/* Arduino-required function */
void loop()
{
  midiListener.poll();
}

/* Send data to song */
void noteOn( unsigned char note, unsigned char volume ) { 
  Serial.print("note on ");
  Serial.println((int) note);
}

/* Send data to song */
void noteOff( unsigned char note ) {
  Serial.print("note off ");
  Serial.println((int) note);
}