These modules enable the reading of midi signals over a HardwareSerial or SoftwareSerial on an Arduino.

For barebones usage, you can simply read signals and attach callbacks to MIDI_NOTE_ON and MIDI_NOTE_OFF.

For stronger usage, you can build entire songs or songbanks, which have their own callbacks for failure or completion. E.g. someone plays a MIDI keyboard, hooked up to your Arduino, and if they play a given song all the way through with no wrong notes, the completion callback which you designate for that song becomes triggered.

###Barebones listener example (see also the examples dir):

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

    /* Callback when MIDI_NOTE_ON occurs */
    void noteOn( unsigned char note, unsigned char volume ) { 
      Serial.print("note on ");
      Serial.println((int) note);
    }

    /* Callback when MIDI_NOTE_OFF occurs */
    void noteOff( unsigned char note ) {
      Serial.print("note off ");
      Serial.println((int) note);
    }