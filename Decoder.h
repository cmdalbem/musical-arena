// Decoder.h

#ifndef DECODER_H
#define DECODER_H

#include <string>
#include <map>
#include <iostream>

#include "utils.h"

class Decoder
{
	private:
		map< string,pair<buttonType,difficultyType> > notes;

		void initHash();

	public:
		Decoder();
		~Decoder();

		string note_from_int(int note_number);
		buttonType whatButton( string note );	
		buttonType whatButton( string note, difficultyType difficulty);
		eventType  whatEventType(u_char arg0, u_char arg2);
};
	
#endif
