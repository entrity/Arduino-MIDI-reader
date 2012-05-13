/*
	Module for holding a midi song and digesting midi events (from midi-listener.h)
*/

#ifndef MJ_MIDI
#define MJ_MIDI

#include "simplevector.h"
#include <stdio.h>
#include "Arduino.h"

namespace mj_midi
{
	// note on or off
	class Action {
		public:
			// fields
			unsigned char note;
			unsigned char volume;
			bool complete; // whether this action has taken place
			// functions
			Action( unsigned char * arr ) : note(arr[0]), volume(arr[1]), complete(false) { }
			Action( unsigned char note, unsigned char volume) : note(note), volume(volume), complete(false) { }
			bool isOn() { volume; }
			bool isOff() { !volume; }
			static Action * load( FILE * streamin );
			void save( FILE * streamout);
			bool matches(unsigned char note, unsigned char volume, bool strict=false);
	};
	
	// all actions taking place at a given moment
	class Moment {
		public:
			// fields
			SimpleVector<Action *, char> actions; // array of actions
			// functions
			Moment(int initialSize=1) { if (initialSize > 1) actions.extend(initialSize-1); }
			~Moment();
			int add( Action * action );
			static Moment * load( FILE * streamin );
			void save( FILE * streamout );
	};

	// a collection of midi moments
	class Song {
		public:
			// fields
			int index; // which moment is earliest "incomplete" moment. a moment is incomplete if not all of the actions in it are flagged as complete. this is relevent when digesting a song
			SimpleVector<Moment *, unsigned short> moments; // array of midi moments
			// functions
			Song(int initialSize=1) : onComplete(0) { if (initialSize > 1) { moments.extend(initialSize-1); } }
			~Song();
			int add( Moment * moment );
			static Song * load( FILE * streamint );
			void save( FILE * streamount );
			static Song * fromArray(unsigned char * arr); // { momentsCount, m0.actionsCount, m0a0.note, m0a0.vol, m0a1.note, m0a1.vol, m1.actionsCount, ... }
			bool digest(unsigned char note, unsigned char volume); // Returns whether current moment in song has an action matching this one. If not, resets song (sets index to 0, sets all actions as not complete). If so, marks matching action as complete.
			void (* onComplete)(); // callback which fires after
			void (* onFail)();
			bool testEventAgainstCurrentMoment(unsigned char note, unsigned char volume);
			void reset(); // sets index to 0 and sets complete to false for all actions
		private:
			void incrementIndexIfNecessary();
	};

	/********* function definitions ************/	
	
}

#endif