// Decoder.h
#pragma once


#include <string>
#include <map>
#include <iostream>
using namespace std;

#include "smf.h"
#include "utils.h"

typedef vector<musicEvent> music;


class Decoder
{
	private:
		map< string,pair<buttonType,difficultyType> > notes;

		int 		readMidi( string file );
		void 		decodeMidiEvent( smf_event_t *event, music* theMusic );
		string 		note_from_int(int note_number);
		buttonType 	whatButton( string note );	
		buttonType 	whatButton( string note, difficultyType difficulty);
		eventType  	whatEventType(u_char arg0, u_char arg2);		
		
	public:
		Decoder();
		~Decoder();
		
		music 		decodeMidi( string file );
};
