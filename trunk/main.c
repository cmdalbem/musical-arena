#include "libsmf/src/smf.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define ONE_BILLION 1000000000

void double_to_timespec(double diff_time, timespec *timer)
{
	timer->tv_sec	= (long int)diff_time;
	diff_time -= timer->tv_sec;
	
	timer->tv_nsec	= diff_time * ONE_BILLION;
	//printf("double: %f\n", diff_time);
}

int main ()
{
	smf_t *smf;
	smf_event_t *event, *event2;
	double diff_time;
	
	timespec tempo;
	
	
	int terminou = 0;

	smf = smf_load("roadtobabylon.mid");
	if (smf == NULL) {
		puts("Whoops, something went wrong.");
		exit(1);
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
		if (diff_time = (event->time_seconds - event2->time_seconds))
		{
			double_to_timespec(diff_time, &tempo);
			nanosleep(&tempo, NULL);
		}
		//printf("tempo->u_sec : %d\t tempo->u_nsec : %d\n", tempo.tv_sec, tempo.tv_nsec);
		
		printf("%s\t %d\t %d\t %f\n", smf_event_decode(event), event->time_pulses, event->delta_time_pulses, event->time_seconds);
		
		
		
		if (smf_event_is_metadata(event))
			continue;
		
	}
	puts("teste");
	smf_delete(smf);
		
	return 0;
}
