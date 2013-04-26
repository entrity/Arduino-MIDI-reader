#include "midi-song.h"

/* return true if the given event has a matching action and note for the params */
inline bool matchesEvent(MjMidi::event * p_event, uint8_t action, uint8_t note)
{
  return p_event->action == action && p_event->note == note;
}

namespace MjMidi
{
  /* Loop through events from current instant until an event is completed or the 
    end of the instant is reached or the end of the song is reached. 

    If none is completed in this attempt and the end of the instant or song is
    reached, the song shall be reset.

    If the final event is completed, the song's callback shall be fired, and the
    song shall be reset. */
  void Song::attempt(uint8_t action, uint8_t note)
  {
    bool songIsCompleteSoFar = true;
    // loop through subsequent events in this song, breaking when there is an offset or song end
    for (unsigned int i = this->i; i < n; i ++) {
      // Failure (i.e. wrong midi event/note):
      // terminate loop if the current event is offset from the current instant
      // and this is not the first time in the loop and incomplete events are known
      // to exist before index i.
      if (p_events[i].offset && i != this->i && !songIsCompleteSoFar) {
        if (failureCallback)
          failureCallback();
        reset();
        return; // failure
      }
      // Neither failure nor success:
      // move on to next event if the current one is already complete
      if (p_events[i].complete)
        continue;
      // Success:
      // mark the current event complete if it matches the input
      if (matchesEvent(&p_events[i], action, note)) {
        p_events[i].complete = true;
        // update events index i if the song is complete so far
        if (songIsCompleteSoFar) {
          this->i = i + 1;
          // recognize end of song if i == n
          if (this->i >= n) {
            if (completionCallback)
              completionCallback();
            reset();
          }
        }
        return; // success
      }
      // at least one event in the current instant was incomplete and not matched
      songIsCompleteSoFar = false;
    }
  }

  /* Clear i. Clear .complete on all events */
  void Song::reset()
  {
    this->i = 0;
    for (unsigned int i = 0; i < this->n; i++) {
      p_events[i].complete = false;
    }
  }

  void Song::setCallbacks(void(*completionCallback)(void), void(*failureCallback)(void))
  {
    this->completionCallback = completionCallback;
    this->failureCallback = failureCallback;
  }
}
