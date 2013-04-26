1. Use midi.rb (and supply it a filename) to turn a midi file into a .c file which defines an MjMidi::Song.
2. Include that .c file in your source file.
3. Create a MidiListener, give it callbacks for MIDI_NOTE_ON and MIDI_NOTE_OFF.
4. In loop, have MidiListener poll its Serial input.

E.g.

    #include <Arduino.h>
    #include <SoftwareSerial.h>
    #include "midi-listener.h"
    #include "midi-song.h"

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