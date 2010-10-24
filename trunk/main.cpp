#include "smf.h"

#include <string>
#include <map>
#include <sys/time.h>
#include <iostream>
using namespace std;

#include "Decoder.h"


#define ONE_BILLION 1000000000


Decoder decoder;



void double_to_timespec(double diff_time, timespec *timer)
{
	timer->tv_sec	= (long int)diff_time;
	diff_time -= timer->tv_sec;
	
	timer->tv_nsec	= diff_time * ONE_BILLION;
	//printf("double: %f\n", diff_time);
}

int main(int argc, char *argv[])
{
	smf_t *smf;
	smf_event_t *nextEvent, *actualEvent;
	double diff_time;
	
	timespec tempo;
	
	
	int terminou = 0;
	if( argc>1 )
		smf = smf_load(argv[1]);
	else {
		cout << "No argument passed, opening 'notes.mid' by default." << endl << endl;
		smf = smf_load("notes.mid");
	}
		
	if (smf == NULL)
		return 1;
	
	//if((event = smf_get_next_event(smf)) != NULL)
		//printf("%s\t %d\t %d\t %f\n", smf_event_decode(event), event->time_pulses, event->delta_time_pulses, event->time_seconds);
		

	while (!terminou) {
		nextEvent = actualEvent;
		if( (actualEvent = smf_get_next_event(smf)) == NULL) {
			terminou = 1;
			break;
		}
		if ( (diff_time = (actualEvent->time_seconds - nextEvent->time_seconds)) ) {
			double_to_timespec(diff_time, &tempo);
			nanosleep(&tempo, NULL);
		}
		
		//printf("%s\t %d\t %d\t %f\n", smf_event_decode(event), event->time_pulses, event->delta_time_pulses, event->time_seconds);
		

		string note = decoder.note_from_int( actualEvent->midi_buffer[1] );
		int noteButton = decoder.whatButton( note );
				
		switch( actualEvent->midi_buffer[0] & 0xF0 )
		{
			case 0x80: //means it's a "note off"
				cout << noteButton << " OFF" << endl;
				break;

			case 0x90: //means it's a "note onf"
				cout << noteButton << " ON" << endl;
				break;
		}
		
	}
	
	cout << endl << "End of file" << endl;
	
	smf_delete(smf);
		
	return 0;
}
