#include "smf.h"

int main()
{
		int count=0;
		
        smf_t *smf;
        smf_event_t *event;

        smf = smf_load("notes.mid");
        if (smf == NULL) {
                printf("Whoops, something went wrong.");
                return 0;
        }
 
        while ((event = smf_get_next_event(smf)) != NULL) {
                
                //if (smf_event_is_metadata(event))
                //{
					//wait until event->time_seconds.
					//feed_to_midi_output(event->midi_buffer, event->midi_buffer_length);

					count++;
				//}
        }

        smf_delete(smf);
        
        printf("%i\n",count);

}	
