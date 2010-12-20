// Decoder.cpp

#include <map>
#include <string>

#include "Decoder.h"


Decoder::Decoder()
{
/* http://fretsonfire.wikidot.com/creating-songs-with-midi-files */
/*static string notes[][] = { {"C8","C#8","D8","D#8","E8"},
							{"C7","C#7","D7","D#7","E7"},
							{"C6","C#6","D6","D#6","E6"},
							{"C5","C#5","D5","D#5","E5"} };	*/	
	// EXPERT
	notes["C7"] = pair<buttonType,difficultyType>(B1,EXPERT);
	notes["C#7"] = pair<buttonType,difficultyType>(B2,EXPERT);
	notes["D7"] = pair<buttonType,difficultyType>(B3,EXPERT);
	notes["D#7"] = pair<buttonType,difficultyType>(B4,EXPERT);
	notes["E7"] = pair<buttonType,difficultyType>(B5,EXPERT);
	
	// HARD
	notes["C6"] = pair<buttonType,difficultyType>(B1,HARD);
	notes["C#6"] = pair<buttonType,difficultyType>(B2,HARD);
	notes["D6"] = pair<buttonType,difficultyType>(B3,HARD);
	notes["D#6"] = pair<buttonType,difficultyType>(B4,HARD);
	notes["E6"] = pair<buttonType,difficultyType>(B5,HARD);
	
	// MEDIUM
	notes["C5"] = pair<buttonType,difficultyType>(B1,MEDIUM);
	notes["C#5"] = pair<buttonType,difficultyType>(B2,MEDIUM);
	notes["D5"] = pair<buttonType,difficultyType>(B3,MEDIUM);
	notes["D#5"] = pair<buttonType,difficultyType>(B4,MEDIUM);
	notes["E5"] = pair<buttonType,difficultyType>(B5,MEDIUM);
	
	// EASY
	notes["C4"] = pair<buttonType,difficultyType>(B1,EASY);
	notes["C#4"] = pair<buttonType,difficultyType>(B2,EASY);
	notes["D4"] = pair<buttonType,difficultyType>(B3,EASY);
	notes["D#4"] = pair<buttonType,difficultyType>(B4,EASY);
	notes["E4"] = pair<buttonType,difficultyType>(B5,EASY);
}


Decoder::~Decoder() {}


music* Decoder::decodeMidi( string file, difficultyType difficulty )
/* reads the mid file calling decodeMidiEvent() for every event */
{
	music *theMusic = new music;
	
	smf_t *smf;
	smf_event_t *event;
	bool eof = false;
	
	int mspqn; // microseconds per quarter note
	
	// load the file
	smf = smf_load( file.c_str() );
	if (smf == NULL) {
		free(theMusic);
		return NULL;
	}
	
	// handles all events
	while (!eof) {
		event = smf_get_next_event(smf);
		
		if( !event ) 
			eof = true;
		
		else if( smf_event_is_metadata(event) )
		{
			//printf("%s\t %d\t %d\t %f\n", smf_event_decode(event), event->time_pulses, event->delta_time_pulses, event->time_seconds);
			 switch (event->midi_buffer[1]) {
				 case 0x51:
					mspqn = (event->midi_buffer[3] << 16) + (event->midi_buffer[4] << 8) + event->midi_buffer[5];
					break;
			}
		}
		else
			decodeMidiEvent(event,mspqn,theMusic,difficulty);
	}
	
	//cout << endl << "End of file." << endl;
	smf_delete(smf);
	
	if(theMusic->size() < MIN_MUSIC_SIZE) {
		free(theMusic);
		return NULL;
	}
	else
		return theMusic;
}

void Decoder::printMusic( music aMusic )
{
	cout << "MIDI parsed. This is your music:" << endl;
	for(unsigned int i = 0; i < aMusic.size(); i++) {
		cout << aMusic[i].time << " - " << aMusic[i].button << ": ";
		switch(aMusic[i].type)
		{
			case ON: cout << "ON"; break;
			case OFF: cout << "OFF"; break;
			default: break;
		}
		cout<<endl;
	}
}	


void Decoder::decodeMidiEvent( smf_event_t *event, int thisMspqn, music* theMusic, difficultyType difficulty )
{
	buttonType button = whatButton( note_from_int(event->midi_buffer[1]), difficulty );
	//cout << smf_event_decode(event) << endl;
	
	if(button != NIL)  {	// decoded the event successfully
		musicEvent newEvent;
		
		eventType type = whatEventType( event->midi_buffer[0], event->midi_buffer[2] );
		
		// inicializes newEvent fields and put newEvent on the event vector.
		newEvent.time = event->time_seconds;
		newEvent.button = button;
		newEvent.type  = type;	
		newEvent.mspqn = thisMspqn;
		theMusic->push_back(newEvent); // put this new event to the music events list
	}
}


string Decoder::note_from_int(int note_number)
/* extracted from smf_decode.c */
{
	int note, octave;
	string names[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

	octave = note_number / 12 - 1;
	note = note_number % 12;

	return names[note] + (char)(octave+48);
}


buttonType Decoder::whatButton( string note )
{	return notes[note].first;	}

buttonType Decoder::whatButton( string note, difficultyType difficulty )
{
	if( notes[note].second == difficulty )
		return notes[note].first;
	else
		return NIL;
}

eventType Decoder::whatEventType(u_char arg, u_char arg2)
{
	switch( arg & 0xF0 )
	{
		case 0x80: return OFF;
		case 0x90: 
			if ( arg2 != 0)
				return ON;
			else
				return OFF;
	}
	return ERROR;
}

