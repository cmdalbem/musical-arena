#include <map>
#include <string>
using namespace std;

#include "Decoder.h"


Decoder::Decoder()
{
	initHash();	
}

void Decoder::initHash()
{
/* http://fretsonfire.wikidot.com/creating-songs-with-midi-files */
/*static string notes[][] = { {"C8","C#8","D8","D#8","E8"},
							{"C7","C#7","D7","D#7","E7"},
							{"C6","C#6","D6","D#6","E6"},
							{"C5","C#5","D5","D#5","E5"} };	*/	
	// EXPERT
	notes["C7"] = pair<buttonsType,difficultyType>(B1,EXPERT);
	notes["C#7"] = pair<buttonsType,difficultyType>(B2,EXPERT);
	notes["D7"] = pair<buttonsType,difficultyType>(B3,EXPERT);
	notes["D#7"] = pair<buttonsType,difficultyType>(B4,EXPERT);
	notes["E7"] = pair<buttonsType,difficultyType>(B5,EXPERT);
	
	// MEDIUM
	notes["C6"] = pair<buttonsType,difficultyType>(B1,MEDIUM);
	notes["C#6"] = pair<buttonsType,difficultyType>(B2,MEDIUM);
	notes["D6"] = pair<buttonsType,difficultyType>(B3,MEDIUM);
	notes["D#6"] = pair<buttonsType,difficultyType>(B4,MEDIUM);
	notes["E6"] = pair<buttonsType,difficultyType>(B5,MEDIUM);
	
	// EASY
	notes["C5"] = pair<buttonsType,difficultyType>(B1,EASY);
	notes["C#5"] = pair<buttonsType,difficultyType>(B2,EASY);
	notes["D5"] = pair<buttonsType,difficultyType>(B3,EASY);
	notes["D#5"] = pair<buttonsType,difficultyType>(B4,EASY);
	notes["E5"] = pair<buttonsType,difficultyType>(B5,EASY);
	
	// SUPER EASY
	notes["C4"] = pair<buttonsType,difficultyType>(B1,SUPEREASY);
	notes["C#4"] = pair<buttonsType,difficultyType>(B2,SUPEREASY);
	notes["D4"] = pair<buttonsType,difficultyType>(B3,SUPEREASY);
	notes["D#4"] = pair<buttonsType,difficultyType>(B4,SUPEREASY);
	notes["E4"] = pair<buttonsType,difficultyType>(B5,SUPEREASY);
}	


Decoder::~Decoder() {}

string Decoder::note_from_int(int note_number)
/* extracted from smf_decode.c */
{
	int note, octave;
	string names[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

	octave = note_number / 12 - 1;
	note = note_number % 12;

	return names[note] + (char)(octave+48);
}


buttonsType Decoder::whatButton( string note )
{
	
	return notes[note].first;	
}

buttonsType Decoder::whatButton( string note, difficultyType difficulty )
{
	if( notes[note].second == difficulty )
		return notes[note].first;
	else
		return NIL;
}
