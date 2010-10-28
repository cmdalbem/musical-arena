// Decoder.h

#ifndef DECODER_H
#define DECODER_H

#include <string>
#include <map>
#include <iostream>

using namespace std;

enum buttonType
{
	B1, B2, B3, B4, B5, NIL
};

enum difficultyType
{
	EXPERT, MEDIUM, EASY, SUPEREASY
};

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
};
	
#endif
