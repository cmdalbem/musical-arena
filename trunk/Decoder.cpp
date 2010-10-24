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
	notes["C8"] = B1;
	notes["C#8"] = B2;
	notes["D8"] = B3;
	notes["D#8"] = B4;
	notes["E8"] = B5;
	
	// MEDIUM
	notes["C7"] = B1;
	notes["C#7"] = B2;
	notes["D7"] = B3;
	notes["D#7"] = B4;
	notes["E7"] = B5;
	
	// EASY
	notes["C6"] = B1;
	notes["C#6"] = B2;
	notes["D6"] = B3;
	notes["D#6"] = B4;
	notes["E6"] = B5;
	
	// SUPER EASY
	notes["C5"] = B1;
	notes["C#5"] = B2;
	notes["D5"] = B3;
	notes["D#5"] = B4;
	notes["E5"] = B5;
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
	
	return notes[note];	
}
