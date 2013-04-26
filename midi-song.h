#ifndef MIDI_SONG_H
#define MIDI_SONG_H

/*
  The MidiSong holds many MidiInstants, which each hold many MidiActions.
  Each of these three models has a field for completion, to indicate that the
  action, instant, or song has been completed.
*/

#include <Arduino.h>
#include "midi-vars.h"

namespace MjMidi
{
  /* A Midi event */
  typedef struct {
    uint16_t offset;
    uint8_t
      action,
      note;
    bool complete;
  } event;

  /* A collection of Midi events */
  class Song
  {
    event * p_events;
    unsigned short i;
    unsigned short n;
    void(*completionCallback)(void);
    void(*failureCallback)(void);
  public:
    /* constructor */
    Song(event * p_events, unsigned short n_events)
      : p_events(p_events), i((unsigned short) 0), n(n_events) {};
    /* other methods */
    void attempt(uint8_t action, uint8_t note);
    void reset();
    void setCallbacks(void(*completionCallback)(void), void(*failureCallback)(void));
  };

  /* A collection of songs */
  class Collection
  {
  public:
    void handleMidiEvent(uint8_t action, uint8_t note);
  };
}

#endif