#include "libsmf/src/smf.h"

#include <string>
#include <sys/time.h>
#include <iostream>
using namespace std;



#define ONE_BILLION 1000000000

static string notes[] = {"C7","C#7","D7","D#7","E7"}; //these are the respective notes for the buttons of the difficulty "Amazing"


static void
note_from_int(string *buf, int note_number)
/* extracted from smf_decode.c */
{
	int note, octave;
	string names[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

	octave = note_number / 12 - 1;
	note = note_number % 12;

	*buf = names[note] + (char)(octave+48);
}


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
	smf_event_t *event, *event2;
	double diff_time;
	
	timespec tempo;
	
	
	int terminou = 0;

	smf = smf_load(argv[1]);
	if (smf == NULL) {
		puts("Whoops, something went wrong.");
		return 1;
	}
	
	if((event = smf_get_next_event(smf)) != NULL)
		printf("%s\t %d\t %d\t %f\n", smf_event_decode(event), event->time_pulses, event->delta_time_pulses, event->time_seconds);
		

	while (!terminou) {
		event2 = event;
		if((event = smf_get_next_event(smf)) == NULL)
		{
			terminou = 1;
			break;
		}
		if ( (diff_time = (event->time_seconds - event2->time_seconds)) )
		{
			double_to_timespec(diff_time, &tempo);
			nanosleep(&tempo, NULL);
		}
		
		string note;
		note_from_int(&note, event->midi_buffer[1]); //retrieves the note played
		int noteButton;
		
		//printf("%s\t %d\t %d\t %f\n", smf_event_decode(event), event->time_pulses, event->delta_time_pulses, event->time_seconds);
	
		for(int i=0; i<5; i++)
			if(note == notes[i])
				noteButton = i;
				
		switch (event->midi_buffer[0] & 0xF0) {
			case 0x80: //means it's a "note off"
				cout << noteButton << " OFF" << endl;
				break;

			case 0x90: //means it's a "note onf"
				cout << noteButton << " ON" << endl;
				break;
		}

		
		
		
		if (smf_event_is_metadata(event))
			continue;
		
	}
	smf_delete(smf);
		
	return 0;
}
