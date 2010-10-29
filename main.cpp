// main.cpp
//
// Most important file on musical-arena, since here we have the main function =D
//

#include "Decoder.h"
#include "stone.h"
#include "utils.h"

// Addicional Libraries
#ifdef HAVE_IRRKLANG
  #include <irrKlang.h>
#endif
#include "smf.h"

// says what note is being read in each moment
Decoder decoder;

// stores an entire MIDI file, so we don't need to keep it opened during too much time
vector<musicEvent> theMusic;

// stores the stones present on the screen
vector<stone*>	stonesOnScreen[5];

// indicates the end of the music for the keyboard polling function
bool endOfMusic;

string defaultFile = "example.mid", songFile = "example.ogg";
int mspqn; //ms per quarter note
note theScreen[SCREEN_Y][5];

void matrix_update()
{
	// propagates the lines of the matrix	
	for(int lin=SCREEN_Y-1; lin>0; lin--)
		for(int col=0; col<5; col++)
			theScreen[lin][col] = theScreen[lin-1][col];
}

void matrix_print()
{
	//prints the matrix	
	for(int lin=0; lin<SCREEN_Y; lin++)
	{
		for(int col=0; col<5; col++)
		{
			switch(theScreen[lin][col])
			{
				case NOTHING: cout<<" "; break;
				case STRIKE: cout<<"O"; break;						
				case HOLD: cout<<"|"; break;
			}
			cout<<"  ";
		}
		cout<<endl;
	}
}

void decodeMidiEvent( smf_event_t *event )
{
	buttonType button = decoder.whatButton( decoder.note_from_int(event->midi_buffer[1]), EXPERT );
	//cout << smf_event_decode(event) << endl;
	
	if(button != NIL)  {	// decoded the event successfully
		musicEvent newEvent;
		
		eventType type = decoder.whatEventType( event->midi_buffer[0], event->midi_buffer[2] );
		
		// inicializes newEvent fields and put newEvent on the event vector.
		newEvent.time = event->time_seconds;
		newEvent.button = button;
		newEvent.type  = type;	
		theMusic.push_back(newEvent); // put this new event to the music events list
	}
}

int readMidi()
/* reads the mid file calling decodeMidiEvent() for every event */
{
	smf_t *smf;
	smf_event_t *event;
	bool eof = false;
	
	// load the file
	smf = smf_load( defaultFile.c_str() );
	if (smf == NULL) 
		return 1;
	
	while (!eof) {
		event = smf_get_next_event(smf);
		if( !event ) 
			eof = true;
		else if( smf_event_is_metadata(event) )	// ignores these events
			;
		else
			decodeMidiEvent(event);
	}
	
	cout << endl << "End of file." << endl;
	smf_delete(smf);
		
	return 0;	
}

static void* drawer(void *argument) 
{
	struct	timeval start;
	double	musicTime;

	// get the time before starting the music (so we can know how much time passed in each note)
	gettimeofday(&start, NULL);
	
	while( !endOfMusic )
	{
		usleep(80000);
		musicTime = time_diff(start);
		
		cout << "music time: "	   << musicTime		 << endl
			 << "upcoming event: " << theMusic[0].time << endl;

		matrix_update();

		while( (musicTime + STONE_DELAY) > theMusic[0].time ) {
			// updates the first line of the matrix with the actual configuration
			switch(theMusic[0].type) {
				case ON:	{	// definitely, the last event on this track was an OFF
					puts("entrou no ON");
					theScreen[0][theMusic[0].button] = STRIKE;
					// creates a new stone
					int track = buttonType_to_int(theMusic[0].button);
					stone *newStone = new stone(theMusic[0], -1);

					// puts the stone on the end of the queue, i.e., it is, now, the
					// last stone to be destroyed
					stonesOnScreen[track].push_back(newStone);
					
					break;
				}
				case OFF:	{	// definitely, the last event on this track was an ON
					puts("entrou no OFF");
					theScreen[0][theMusic[0].button] = NOTHING;
					
					int track = buttonType_to_int(theMusic[0].button);
					int vector_size = stonesOnScreen[track].size();
					
					// sets the destroy_time of the last element of the vector of
					// stones of the referred track.
					cout << "size: " << vector_size << endl;
					if(vector_size > 0)
						stonesOnScreen[track][vector_size-1]->destroy_time = theMusic[0].time;
					else	{
						puts ("received an OFF without an ON O.o");
						exit (1);
					}
					
					break;
				}
			}
			// avoiding segmentation faults =D
			if(theMusic.size() > 0)
				theMusic.erase(theMusic.begin());
			else
				endOfMusic = true;
		}
		
		/*
		// desalocate the stones for which the time has already gone
		int has_more = 1;
		for (int i = 0; i < 5; i++, has_more = 1)
			while (has_more == 1)
				if( musicTime > stonesOnScreen[i][0]->destroy_time )
					stonesOnScreen[i].erase(stonesOnScreen[i].begin());
				else
					has_more = 0;
		*/
		matrix_print();
	}
	

		
	return NULL;	
}

void* fretting (void *arg)
{
	system("stty cbreak");
	
	while(!endOfMusic)
	{
		// poll the keyboard testing if the player has pressed the right notes.
		if(getchar() == 'a')
			;
	}
	
	system("stty icanon");
	
	return NULL;
}

void musa_init()
{
	readMidi();
	
	//inicializes the matrix
	for(int lin=0; lin<SCREEN_Y; lin++)
		for(int col=0; col<5; col++)
			theScreen[lin][col] = NOTHING;
}

int main(int argc, char *argv[])
{
#ifdef HAVE_IRRKLANG
	irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();
	irrklang::ISoundSource* oggMusic = soundEngine->addSoundSourceFromFile(songFile.c_str());
#endif
	
	pthread_t thread[3];
	int arg = 1;
	
	musa_init();
	 
	cout<<"MIDI parsed. This is your music:"<<endl;
	for(unsigned int i=0; i<theMusic.size(); i++) {
		cout<<theMusic[i].time<<" - "<<theMusic[i].button << ": ";
		switch(theMusic[i].type)
		{
			case ON: cout << "ON"; break;
			case OFF: cout << "OFF"; break;
		}
		cout<<endl;
	}
	cout<<"Press enter to start the music!"<<endl; getchar();

#ifdef HAVE_IRRKLANG
	soundEngine->play2D(oggMusic, true);
#endif
	endOfMusic = false;
	pthread_create(&thread[0], NULL, drawer, (void *) arg);
	pthread_create(&thread[2], NULL, fretting, (void *) arg);
	
	// wait for all threads to complete
	pthread_join(thread[0], NULL);
	//pthread_join(thread[1], NULL);

#ifdef HAVE_IRRKLANG
	soundEngine->drop();
#endif
	return 0;
}



//////////////////////////////
static void *updater(void *argument) 
{
	clock_t actualTime, baseTime = clock();
	float musicTime;
	
	while( theMusic.size()>0 )
	{
		actualTime = clock();
		musicTime = (float) (actualTime - baseTime) / CLOCKS_PER_SEC;
		//cout<<"actualTime - baseTime = "<<actualTime<<"-"<<baseTime<<" = "<<actualTime - baseTime<<endl;
		//cout<<musicTime<<endl;
		//cout<<theMusic[0].time<<endl;
		while( musicTime > theMusic[0].time ) {
			//cout << "EVENT LAUNCHED" << endl;
			// updates the first line of the matrix with the actual configuration		
			switch(theMusic[0].type) {
				case ON:
					theScreen[0][theMusic[0].button] = STRIKE;
					break;
				case OFF:
					theScreen[0][theMusic[0].button] = NOTHING;
					break;
			}
				
			theMusic.erase(theMusic.begin());
		}		
	}
	
	return NULL;
}

