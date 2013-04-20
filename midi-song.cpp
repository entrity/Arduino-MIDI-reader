#include "midi-song.h"

/* Marks this object complete if there is a match */
uint8_t MidiAction::attempt(uint8_t action, uint8_t note)
{
  if (action == this->action && note == this->note) {
    complete = true;
    return MIDI_OBJ_COMPLETE;
  }
  return MIDI_ATTEMPT_FAILED;
}

/* Send attempt to all incomplete actions */
uint8_t MidiInstant::attempt(uint8_t action, uint8_t note)
{
  MidiAction * p_action;
  // loop through MidiActions
  for (uint16_t i = 0; i < n; i++) {
    p_action = &p_actions[i];
    if (!p_action->isComplete()) {
      // if the attempt succeeds, check whether this Instant is complete and return true
      if (p_action->attempt(action, note) == MIDI_OBJ_COMPLETE) {
        return checkCompletion() ? MIDI_OBJ_COMPLETE : MIDI_ATTEMPT_SUCCEEDED;
      }
    }
  }
  // if the attempt did not succeed, reset the instant
  reset();
  return MIDI_ATTEMPT_FAILED;
}

/* Send attempt to current instant. If this attempt completes the last instant, fire callback. */
uint8_t MidiSong::attempt(uint8_t action, uint8_t note)
{
  MidiInstant * p_instant = &p_instants[i];
  // if an instant was completed check whether this object is complete
  switch (p_instant->attempt(action, note)) {
  case MIDI_OBJ_COMPLETE:
    // check whether the completed instant was the final one in this song
    if (this->i == this->n - 1) {
      completionCallback();
      reset();
      return MIDI_OBJ_COMPLETE;
    }
  case MIDI_ATTEMPT_SUCCEEDED:
    return MIDI_ATTEMPT_SUCCEEDED;
  }
  // if the attempt did not succeed, reset the song
  reset();
  return MIDI_ATTEMPT_FAILED;
}

/* Send attempt to every song. */
void MidiSongCollection::attempt(uint8_t action, uint8_t note)
{
  for (uint16_t i = 0; i < n; i++) {
    p_songs[i].attempt(action, note);
  }
}

/* Clear complete */
void MidiAction::reset()
{
  complete = false;
}

/* Clear complete and reset all MidiActions */
void MidiInstant::reset()
{
  // loop through MidiActions
  for (uint16_t j = 0; j < n; j++) {
    p_actions[j].reset();
  }
  // clear complete
  complete = false;
}

/* Clear complete, reset index, and reset all MidiInstants */
void MidiSong::reset()
{
  // loop through MidiInstants
  for (uint16_t j = 0; j < n; j++) {
    p_instants[j].reset();
  }
  // clear complete
  this->complete = false;
  // reset index
  this->i = 0;
}

/* Checks whether all actions are complete. Sets this->complete if so. Called by attempt(). */
bool MidiInstant::checkCompletion()
{
  for (uint16_t i = 0; i < n; i++) {
    if (!p_actions[i].isComplete())
      return false;
  }
  this->complete = true;
  return true;
}

bool MidiAction::isComplete()
{
  return complete;
}