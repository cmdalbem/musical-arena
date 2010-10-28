// main.cpp
//
// Most important file on musical-arena, since here we have the main function =D
//

// Standard Libraries
#include <cstdlib>
#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

// Addicional Libraries
//#include <irrKlang.h>
#include "smf.h"

// used headers
#include "Decoder.h"

using namespace std;

#define ONE_BILLION 1000000000
#define ONE_MILLION 1000000
#define SCREEN_Y 20
#define DEBUG 0

/*
 * Macros --> ideally, this is gonna be transferred to another file on the future.
 */
#define DOUBLE_TO_TIMESPEC(diff_time, timer) \
{ \
	timer.tv_sec	= (long int)diff_time; \
	diff_time -= timer.tv_sec; \
	\
	timer.tv_nsec	= diff_time * ONE_BILLION; \
}

#define DOUBLE_TO_TIMEVAL(diff_time, timer) \
{ \
	timer.tv_sec	= (long int)diff_time; \
	diff_time -= timer.tv_sec; \
	\
	timer->tv_nsec	= diff_time * ONE_MILLION; \
}

#define TIMEVAL_TO_DOUBLE(timevalue, var) \
{ \
	var = timevalue.tv_sec; \
	var = (double)(var + ((timevalue.tv_usec) / ONE_MILLION)); \
}

/*
 * Types of notes of the screen
 */
enum note
{
	STRIKE,
	HOLD,
	NOTHING
};

/*
 * Types of MIDI events
 */
enum eventType
{
	ON,
	OFF
};

typedef struct 
{
	buttonType button;
	eventType type;
	double time; //	time in seconds
} musicEvent;

// this object says what note is being read in each moment
Decoder decoder;

// stores an entire MIDI file, so we don't need to keep it opened during too much time
vector<musicEvent> aMusic;

string defaultFile = "example.mid";
int mspqn; //ms per quarter note
note screen[SCREEN_Y][5] = {{NOTHING}};

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
			}
				
			aMusic.erase(aMusic.begin());
		}		
	}
	
	return NULL;
}



static void *drawer(void *argument) 
{

/*
    struct timeval start, end;

    long mtime, seconds, useconds;    

    gettimeofday(&start, NULL);
    usleep(2000);
    gettimeofday(&end, NULL);

    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

    printf("Elapsed time: %ld milliseconds\n", mtime);

*/		
	//prints the matrix		
	for(int lin=0; lin<SCREEN_Y; lin++)
		for(int col=0; col<5; col++)
			screen[lin][col] = NOTHING;
	
	struct timeval start, end;
	double musicTime;

	gettimeofday(&start, NULL);
	while( 1 )
	{
		usleep(10000);
		system("clear"); 

		gettimeofday(&end, NULL);
		
		end.tv_sec  = end.tv_sec  - start.tv_sec;
		end.tv_usec = end.tv_usec - start.tv_usec;
		
		//musicTime = (double)((end.tv_usec)/ONE_MILLION);
		TIMEVAL_TO_DOUBLE(end, musicTime);
		
		cout << "music time: " << (double)musicTime << endl;
		cout << "upcoming event: " << aMusic[0].time << endl;
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
		}
		cout<<endl;
	}
	
	cout<<"Press enter to start the music!"<<endl; getchar();
	
	drawer((void*)arg);
	//pthread_create(&thread[0], NULL, drawer, (void *) arg);
	//pthread_create(&thread[1], NULL, updater, (void *) arg);
	
	// wait for all threads to complete
	//pthread_join(thread[0], NULL);
	//pthread_join(thread[1], NULL);
}
