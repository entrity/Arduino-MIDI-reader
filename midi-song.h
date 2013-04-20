#ifndef MIDI_SONG_H
#define MIDI_SONG_H

/*
  The MidiSong holds many MidiInstants, which each hold many MidiActions.
  Each of these three models has a field for completion, to indicate that the
  action, instant, or song has been completed.
*/

#include <Arduino.h>
#include "midi-vars.h"

enum MIDI_ATTEMPT_OUTCOME {
  MIDI_ATTEMPT_FAILED = 0,
  MIDI_ATTEMPT_SUCCEEDED,
  MIDI_OBJ_COMPLETE
};

/* A MIDI_NOTE_ON or MIDI_NOTE_OFF */
class MidiAction
{
  bool complete;
  uint16_t n;
  uint8_t action, note;
public:
  MidiAction(uint8_t action, uint8_t note) : n(0), action(action), note(note) {}
  /* Returns this->complete */
  bool isComplete();
  /* Marks this object complete if there is a match */
  uint8_t attempt(uint8_t action, uint8_t note);
  /* Clear complete */
  void reset();
};

class MidiInstant
{
  bool complete;
  uint16_t n;
  MidiAction * p_actions; // array
  /* Checks whether all actions are complete. Sets this->complete if so. Called by attempt(). */
  bool checkCompletion();
public:
  MidiInstant(MidiAction * p_actions) : n(0), p_actions(p_actions) {}
  /* Returns this->complete */
  bool isComplete();
  /* Send attempt to all incomplete actions */
  uint8_t attempt(uint8_t action, uint8_t note);
  /* Clear complete and reset all MidiActions */
  void reset();
};

class MidiSong
{
  bool complete; // not really used
  uint16_t i, n;
  MidiInstant * p_instants; // array
  void (* completionCallback)(void);
public:
  MidiSong(MidiInstant * p_instants, void(* callback)(void))
    : i(0), n(0), p_instants(p_instants), completionCallback(callback) {}
  /* Returns this->complete. Not used. */
  bool isComplete();
  /* Send attempt to current instant. If this attempt completes the last instant, fire callback. */
  uint8_t attempt(uint8_t action, uint8_t note);
  /* Clear complete, reset index, and reset all MidiInstants */
  void reset();
};

class MidiSongCollection
{
  uint16_t n;
  MidiSong * p_songs; // array
public:
  MidiSongCollection(MidiSong * p_songs) : n(0), p_songs(p_songs) {}
  /* Send attempt to every song. */
  void attempt(uint8_t action, uint8_t note);
};

#endif