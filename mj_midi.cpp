#include "mj_midi.h"
#define SIZE_O_ACTION_ON_DISK 2 // how much space an instance of Action uses when saved to nonvolatile memory

namespace mj_midi
{
	char buffer[SIZE_O_ACTION_ON_DISK];

	/* Song functions -----------------------------------------------------------*/

	Song::~Song() {
		for ( int i=0; i < moments.size(); i++ ) // destruct all moments in vector
			{ moments[i]->~Moment(); }
	}

	Song * Song::load( FILE * streamin ) {
		// printf("loading song\n");
		unsigned char count;
		fread( &count, 1, 1, streamin );
		// printf("song of %d moments\n", count);
		Song * song = new Song( count );
		Moment * moment;
		for (int i=0; i < count; i++) {
			moment = Moment::load( streamin );
			( *song ).add( moment ); // add new moment to this song
		}
		return song;
	}

	// write this song to given streamout
	void Song::save( FILE * streamout ) {
		/* Song consists of number of Moments, array of Moments */
		unsigned char count = moments.size();
		// printf("song of %d moments\n", count);
		fwrite( &count, 1, 1, streamout );
		for (int i=0; i < moments.size(); i++)
			{ moments[i]->save( streamout ); }
	}

	// add a midi moment to the end of this song
	int Song::add( Moment * moment ) {
		return moments.push( moment );
	}

	// Reads a byte array (following format of data file), populates, returns a song.
	Song * Song::fromArray(unsigned char * arr)
	{
		unsigned char momentCount = arr[0];
		int actionCount;
		Action * action;
		Moment * moment;
		
		Song * song = new Song(momentCount);

		int i = 1;
		while (momentCount--)
		{	
			actionCount = arr[i++];
			moment = new Moment(actionCount);
			song->add(moment);
			while (actionCount--)
			{
				action = new Action(&arr[i]);
				moment->add(action);
				i+=2;
			}
		}
		return song;
	}

	/* 
	Receives data on one midi event, compares it against current moment 
	Returns whether current moment in song has an action matching these params.
	If not, resets song (sets index to 0, sets all actions as not complete).
	If so, marks matching action as complete. If all actions for current moment are
	complete, song's index gets incremented.
	*/
	bool Song::digest(unsigned char note, unsigned char volume)
	{
		if ( testEventAgainstCurrentMoment(note, volume) )
			return true;
		reset();
		// try this action against the first moment
		return testEventAgainstCurrentMoment(note, volume);
	}

	// return whether this midi event matches any in the midi moment at song's current index
	bool Song::testEventAgainstCurrentMoment(unsigned char note, unsigned char volume)
	{
		Moment * moment = moments[index];
		for (int j = 0; j < moment->actions.size(); j++) {
			Action * action = moment->actions[j];
			if ( action->complete ) continue;
			if ( action->matches(note, volume) ) {
				action->complete = true;
				incrementIndexIfNecessary();
				return true;
			}
		}
		if ( onFail ) { onFail(); }
		return false;
	}

	// if current moment's actions are all complete, increment song index
	void Song::incrementIndexIfNecessary()
	{
		bool complete = true;
		Moment * moment = moments[index];
		for (int i=0; i < moment->actions.size(); i++) {
			if ( !moment->actions[i]->complete ) {
				complete = false;
				break;
			}
		}
		if ( complete ) {
			index++;
			if ( index >= moments.n && onComplete )
				{ onComplete(); }
		}
	}

	// Set index to 0. Set complete to 0 for all Actions.
	void Song::reset()
	{
		index = 0;
		for (int i=0; i < moments.size(); i++) {
			for (int j=0; j < moments[i]->actions.size(); j++) {
				moments[i]->actions[j]->complete = 0;
			}
		}
	}


	/* Moment functions ---------------------------------------------------------*/

	Moment::~Moment() {
		for ( int i=0; i < actions.size(); i++ ) // destruct all actions in vector
			{ actions[i]->~Action(); }
	}

	Moment * Moment::load( FILE * streamin ) {
		unsigned char count;
		fread( &count, 1, 1, streamin ); // read number of actions in this moment
		// printf("\tmoment of %d actions\n", count);
		Moment * moment = new Moment( count );
		Action * action;
		for (int i=0; i < count; i++) {
			action = Action::load( streamin );
			( *moment ).add( action ); // add new action to this moment
		}
		return moment;
	}

	void Moment::save( FILE * streamout ) {
		/* Moment consists of # of Actions, array of Actions */
		buffer[0] = actions.size();
		// printf("\tmoment of %d actions\n", buffer[0]);
		fwrite( &buffer, 1, 1, streamout ); // write number of actions in this moment
		for (int i=0; i < actions.size(); i++)
			{ actions[i]->save( streamout ); } // write out all actions in this moment
	}

	// Add MIDI Action to this MIDI Moment
	int Moment::add( Action * action ) {
		return actions.push( action );
	}

	/* Action functions ---------------------------------------------------------*/
	
	Action * Action::load( FILE * streamin ) {
		fread( &buffer, 2, 1, streamin );
		// printf("\t\taction read %d %d\n", buffer[0], buffer[1]);
		return new Action( buffer[0], buffer[1] );
	}
	
	void Action::save( FILE * streamout ) {
		fwrite( this, 2, 1, streamout );
		// printf( "%d %d...\n", note, volume ); 
	}

	// 'strict' means that volumes match. if not strict then volumes are only treated as booleans
	bool Action::matches(unsigned char note, unsigned char volume, bool strict)
	{
		if ( note == this->note )
			return strict ? volume == this->volume : !volume == !this->volume;
		return false;
	}

}