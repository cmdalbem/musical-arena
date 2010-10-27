#include <stdlib.h>
#include <string>
#include <map>
#include <iostream>
#include <vector>
using namespace std;
#include <time.h>
#include <pthread.h>

#include <irrKlang.h>
#include "smf.h"

#include "Decoder.h"


#define ONE_BILLION 1000000000
#define SCREEN_Y 50
#define DEBUG 0


enum note//types of notes of the screen
{
	STRIKE, HOLD, NOTHING
};

enum eventType
{
	ON, OFF, STAY
};

typedef struct 
{
	buttonType button;
	eventType type;
	double time; //seconds
} musicEvent;


Decoder decoder;
vector<musicEvent> aMusic;
string defaultFile = "example.mid", songFile = "example.ogg";
int mspqn; //ms per quarter note
note screen[SCREEN_Y][5] = {{NOTHING}};


void double_to_timespec(double diff_time, timespec *timer)
{
	timer->tv_sec	= (long int)diff_time;
	diff_time -= timer->tv_sec;
	
	timer->tv_nsec	= diff_time * ONE_BILLION; 
	//printf("double: %f\n", diff_time);
}

void decodeMidiEvent( smf_event_t *event )
{
	buttonType button = decoder.whatButton( decoder.note_from_int(event->midi_buffer[1]), EXPERT );
	//cout << smf_event_decode(event) << endl;
	
	if(button != NIL)  { // decoded the event successfully
		
		musicEvent newEvent;
		
		newEvent.time = event->time_seconds;
		newEvent.button = button;
		switch( event->midi_buffer[0] & 0xF0 ) {
			case 0x80: newEvent.type = OFF; break;
			case 0x90: newEvent.type = ON; break;
		}			
		
		aMusic.push_back(newEvent); // put this new event to the music events list
	}
}

int readMidi()
/* reads the mid file calling decodeMidiEvent() for every event */
{
	smf_t *smf;
	smf_event_t *event;
	
	bool eof = false;
	smf = smf_load( defaultFile.c_str() );
		
	if (smf == NULL) 
		return 1;
	
	while (!eof) {
		event = smf_get_next_event(smf);
		if( !event ) 
			eof = true;
		else if( smf_event_is_metadata(event) ) {
			//mspqn = (event->midi_buffer[3] << 16) + (event->midi_buffer[4] << 8) + event->midi_buffer[5];
			//printf("%dms per quarter note\n", mspqn);
			//cout << smf_event_extract_text(event) << endl;
		}
		else
			decodeMidiEvent(event);
	}
	
	cout << endl << "End of file." << endl;
	smf_delete(smf);
		
	return 0;	
}

static void *updater(void *argument) 
{
	clock_t actualTime, baseTime = clock();
	float musicTime;
	
	while( aMusic.size()>0 )
	{
		actualTime = clock();
		musicTime = (float) (actualTime - baseTime) / CLOCKS_PER_SEC;
		//cout<<"actualTime - baseTime = "<<actualTime<<"-"<<baseTime<<" = "<<actualTime - baseTime<<endl;
		//cout<<musicTime<<endl;
		//cout<<aMusic[0].time<<endl;
		while( musicTime > aMusic[0].time ) {
			//cout << "EVENT LAUNCHED" << endl;
			// updates the first line of the matrix with the actual configuration		
			switch(aMusic[0].type) {
				case ON:
					screen[0][aMusic[0].button] = STRIKE;
					break;
				case OFF:
					screen[0][aMusic[0].button] = NOTHING;
					break;
				case STAY:
					screen[0][aMusic[0].button] = screen[1][aMusic[0].button];
					break;
			}
				
			aMusic.erase(aMusic.begin());
		}		
	}
	
	return NULL;
}



static void *drawer(void *argument) 
{		
	//prints the matrix		
	for(int lin=0; lin<SCREEN_Y; lin++)
		for(int col=0; col<5; col++)
			screen[lin][col] = NOTHING;
	
	clock_t actualTime, baseTime = clock();
	float musicTime;
	
	while( 1 )
	{
		usleep(10000);
		system("clear"); 
	
		actualTime = clock();
		musicTime = (((float)actualTime - baseTime)) / (float)CLOCKS_PER_SEC;
		//cout<<"actualTime - baseTime = "<<actualTime<<"-"<<baseTime<<" = "<<actualTime - baseTime<<endl;
		cout<<"music time: "<<(float)musicTime<<endl;
		cout<<"upcoming event: "<<aMusic[0].time<<endl;
		while( musicTime > aMusic[0].time ) {
			//cout << "EVENT LAUNCHED" << endl;
			// updates the first line of the matrix with the actual configuration		
			switch(aMusic[0].type) {
				case ON:
					screen[0][aMusic[0].button] = STRIKE;
					break;
				case OFF:
					screen[0][aMusic[0].button] = NOTHING;
					break;
				case STAY:
					screen[0][aMusic[0].button] = screen[1][aMusic[0].button];
					break;
			}
				
			aMusic.erase(aMusic.begin());
		}				
		
		// propagates the lines of the matrix	
		for(int lin=SCREEN_Y-1; lin>0; lin--)
			for(int col=0; col<5; col++)
				screen[lin][col] = screen[lin-1][col];
				
		//prints the matrix		
		for(int lin=0; lin<SCREEN_Y; lin++)
		{
			for(int col=0; col<5; col++)
			{
				switch(screen[lin][col])
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
		
	return NULL;	
}

int main(int argc, char *argv[])
{
	irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();
	irrklang::ISoundSource* oggMusic = soundEngine->addSoundSourceFromFile(songFile.c_str()); 

	pthread_t thread[2];
	int arg = 1;
	 
	readMidi();
	 
	cout<<"MIDI parsed. This is your music:"<<endl;
	for(unsigned int i=0; i<aMusic.size(); i++) {
		cout<<aMusic[i].time<<" - "<<aMusic[i].button << ": ";
		switch(aMusic[i].type)
		{
			case ON: cout << "ON"; break;
			case OFF: cout << "OFF"; break;
			case STAY: cout << "STAY"; break;
		}
		cout<<endl;
	}
	
	cout<<"Press enter to start the music!"<<endl; getchar();
	
	soundEngine->play2D(oggMusic, true);
	pthread_create(&thread[0], NULL, drawer, (void *) arg);
	//pthread_create(&thread[1], NULL, updater, (void *) arg);
	
	// wait for all threads to complete
	pthread_join(thread[0], NULL);
	//pthread_join(thread[1], NULL);
	
	soundEngine->drop();
}
