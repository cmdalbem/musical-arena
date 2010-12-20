// Decoder.h
#pragma once

#include <string>
#include <map>
#include <iostream>
using namespace std;

#include "smf.h"
#include "utils.h"


typedef vector<musicEvent> music;

#define MIN_MUSIC_SIZE 10

class Decoder
{
	public:
		Decoder();
		~Decoder();
		
		music* 		decodeMidi( string file, difficultyType difficulty );
		void 		printMusic( music aMusic );
		
	private:
		map< string,pair<buttonType,difficultyType> > notes;

		int 		readMidi( string file );
		void 		decodeMidiEvent( smf_event_t *event, int mspqn, music* theMusic, difficultyType difficulty );
		string 		note_from_int(int note_number);
		buttonType 	whatButton( string note );	
		buttonType 	whatButton( string note, difficultyType difficulty);
		eventType  	whatEventType(u_char arg0, u_char arg2);				
};
