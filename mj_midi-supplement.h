#ifndef MJ_MIDI_SUPPLEMENT
#define MJ_MIDI_SUPPLEMENT

#include "mj_midi.h"
using namespace mj_midi;

unsigned char SongData[] = { 16, 3, 57, 79, 52, 79, 45, 79, 4, 57, 0, 52, 0, 45, 0, 52, 79, 2, 52, 0, 57, 79, 2, 57, 0, 59, 79, 2, 59, 0, 64, 79, 2, 64, 0, 59, 79, 2, 59, 0, 57, 79, 2, 57, 0, 60, 79, 3, 60, 0, 64, 79, 60, 79, 2, 64, 0, 60, 0, 1, 57, 79, 2, 57, 0, 57, 79, 2, 57, 0, 55, 79, 2, 55, 0, 55, 79, 2, 55, 0, 55, 79, 1, 55, 0 };

char noteSymbols[] = "A0 A#0B0 C1 C#1D1 D#1E1 F1 F#1G1 G#1A2 A#2B2 C2 C#2D2 D#2E2 F2 F#2G2 G#2A3 A#3B3 C3 C#3D3 D#3E3 F3 F#3G3 G#3A4 A#4B4 C4 C#4D4 D#4E4 F4 F#4G4 G#4A5 A#5B5 C5 C#5D5 D#5E5 F5 F#5G5 G#5A6 A#6B6 C6 C#6D6 D#6E6 F6 F#6G6 G#6A7 A#7B7 C7 C#7D7 D#7E7 F7 F#7G7 G#7A8 A#8B8 C8 C#8D8 D#8E8 F8 F#8G8 G#8";

char * noteName(unsigned char note);
void displayNote(unsigned char note, char destLength4[]);

Song * defaultSong();
void inspectSong(Song *);
Moment * dummyMoment( char, char * );
// inspect to stdout
void inspectMoment(Moment * moment, int id=-1);
void inspectAction(Action * action, int id=-1);
// count bytes
int songBytesN(Song * song);
int momentBytes(Moment * moment);
// to stdout
void actionToStdout(Action * action, FILE * fp=stdout);
void momentToStdout(Moment * moment, FILE * fp=stdout);
void songToStdout(Song * song, FILE * fp=stdout);


Song * defaultSong()
{
	Song * song = new Song(1);
	Moment * moment = new Moment(2);
	Action * a = new Action(33,2);
	Action * b = new Action(128,256);
	moment->add(a);
	moment->add(b);
	song->add(moment);
	Moment * mb = dummyMoment( 3, "2h4dsa" );
	song->add(mb);
	song->add( dummyMoment( 2, "abcd" ) );
	song->add( dummyMoment( 1, "2h" ) );
	song->add( dummyMoment( 4, "2h4dsac3" ) );
	return song;
}

// Supply a string and length of string/2, get back a ptr to Moment, populated with n Actions
Moment * dummyMoment(char actionsCount, char arr[])
{
	Action * action;
	Moment * moment = new Moment(actionsCount);
	for (int i=0; i < actionsCount; i++)
	{
		// make Action
		action = new Action(arr[2*i], arr[2*i+1]);
		// push onto Moment
		moment->add(action);
	}
	return moment;
}

void inspectSong(Song * song)
{
	int size = song->moments.size();
	printf("song: (%d moments)\n", size);
	for (int i=0; i < size; i++) 
		{ inspectMoment( song->moments[i], i ); }
}

void inspectMoment(Moment * moment, int id)
{
	int size = moment->actions.size();
	printf("\tmoment (%d): %d actions\n", id, size);
	for (int i=0; i < size; i++)
		{ inspectAction( moment->actions[i], i ); }
}

void inspectAction(Action * action, int id)
{
	char symbol[4] = {0,0,0,0};
	memcpy(symbol, noteName(action->note), 3);
	printf("\t\taction (%d): %d : %d ( %s : %s )\n",
	id,
	action->note,
	action->volume,
	symbol,
	action->volume ? "ON" : "OFF"
	);
}

// Return length of song in bytes
int songBytesN(Song * song)
{
	int sum = sizeof(song);
	for (int i=0; i < song->moments.size(); i++)
		sum += momentBytes(song->moments[i]);
	return sum;
}

// Return length of moment in bytes
int momentBytes(Moment * moment)
{
	return sizeof(moment) + moment->actions.size() * sizeof(Action);
}

void actionToStdout(Action * action, FILE * fp)
{
	fprintf( fp, "%c%c", action->note, action->volume );
}

void momentToStdout(Moment * moment, FILE * fp)
{
	fprintf(fp, "%c", moment->actions.size());
	for (int i=0; i < moment->actions.size(); i++)
		actionToStdout(moment->actions[i], fp);
}

void songToStdout(Song * song, FILE * fp)
{
	fprintf(fp, "%c", song->moments.size());
	for (int i=0; i < song->moments.size(); i++)
		momentToStdout(song->moments[i], fp);
	fprintf(fp, "\n");
}

// points to 3-char array symbolizing given note e.g. "A#3"
char * noteName(unsigned char note)
{
	if (note > 108) note %= 12;
	note -= 21; // 21 == A0
	note *= 3; // each note takes up 3 chars in the array
	return &noteSymbols[note];
}

// write note name symbol to an array (whose last char should be NULL) 
void displayNote(unsigned char note, char destLength4[])
{
	memcpy(destLength4, noteName(note), 3);
}

#endif