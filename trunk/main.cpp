#include "smf.h"

#include <stdlib.h>
#include <string>
#include <map>
#include <iostream>
using namespace std;
#include <sys/time.h>
#include <pthread.h>


#include "Decoder.h"


#define ONE_BILLION 1000000000
#define SCREENY 50
#define DEBUG 0

#define DIFFICULTY EXPERT

Decoder decoder;

int mspqn;
string defaultFile = "example.mid";
bool update = false;
int keys[5] = {0};
int screen[SCREENY][5] = {{0}};


void double_to_timespec(double diff_time, timespec *timer)
{
	timer->tv_sec	= (long int)diff_time;
	diff_time -= timer->tv_sec;
	
	timer->tv_nsec	= diff_time * ONE_BILLION; 
	//printf("double: %f\n", diff_time);
}

void decodeEvent( smf_event_t *event )
/* decodes what button is updates it in keys[] vector */
{
	string note = decoder.note_from_int( event->midi_buffer[1] );

	int noteButton = decoder.whatButton( note, DIFFICULTY );
	
	//cout<<note<<" -> "<<noteButton<<endl;
	
	if(noteButton != NIL)
		switch( event->midi_buffer[0] & 0xF0 )
		{
			case 0x80: //NOTE OFF"
				keys[noteButton] = 0;
				//cout << noteButton << " OFF" << endl;
				if(DEBUG)
					cout << note << " OFF" << endl;
				break;

			case 0x90: //NOTE ON"
				keys[noteButton] = 1;
				//cout << noteButton << " ON" << endl;
				if(DEBUG)
					cout << note << " ON" << endl;
				break;
		}
	
}

static void *interpreter(void *argument)
/* reads the mid file synchronously and calls decodeEvent() for every event */
{
	smf_t *smf;
	smf_event_t *event2, *event;
	double diff_time;
	timespec tempo;
	
	bool eof = false;
	smf = smf_load( defaultFile.c_str() );
		
	if (smf == NULL)
		return NULL;
	
	if( (event = smf_get_next_event(smf)) != NULL )
		if( smf_event_is_metadata(event) ) {
			mspqn = (event->midi_buffer[3] << 16) + (event->midi_buffer[4] << 8) + event->midi_buffer[5];
			printf("%dms per quarter note\n", mspqn);
		}
		

	while (!eof) {
		event2 = event;
		if( (event = smf_get_next_event(smf)) == NULL) {
			eof = true;
			break;
		}
		if ( (diff_time = (event->time_seconds - event2->time_seconds)) ) {
			double_to_timespec(diff_time, &tempo);
			nanosleep(&tempo, NULL);
		}
		
		decodeEvent(event);
		
		//printf("%s\t %d\t %d\t %f\n", smf_event_decode(event), event->time_pulses, event->delta_time_pulses, event->time_seconds);		
	}
	
	cout << endl << "End of file." << endl;
	
	smf_delete(smf);
		
	return 0;	
}

static void *drawer(void *argument) 
{
  if(!DEBUG)
	while(1)
	{
		usleep(mspqn/4); // "ms per quarternote" divided by 4 (just a number for a good resolution)
		system("clear"); 
		
		// if in the last update the note has been turned ON
		//  and wasnt turned OFF yet, changes its code to 2,
		//  which will be represented with an "|" in the screen.
		for(int i=0; i<5; i++)
			if(keys[i]==1 && screen[0][i]==1)
				keys[i]=2;
				
		// propagates the lines of the matrix	
		for(int lin=SCREENY-1; lin>0; lin--)
			for(int col=0; col<5; col++)
				screen[lin][col] = screen[lin-1][col];
				
		// updates the first line of the matrix with the actual configuration		
		for(int i=0; i<5; i++)
			screen[0][i] = keys[i];
			
		//prints the matrix		
		for(int lin=0; lin<SCREENY; lin++)
		{
			for(int col=0; col<5; col++)
			{
				switch(screen[lin][col]){
					case 0: cout<<" "; break;
					case 1: cout<<"O"; break;						
					case 2: cout<<"|"; break;
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
	cout<<"Press enter to start"<<endl; getchar();
	
	pthread_t threads[2];
	int arg = 1;

	pthread_create(&threads[0], NULL, interpreter, (void *) arg);
	pthread_create(&threads[1], NULL, drawer, (void *) arg);
	
	/* wait for all threads to complete */
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);

}
