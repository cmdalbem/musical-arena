// EventReceiver.cpp
//

#include "EventReceiver.h"

EventReceiver::EventReceiver()
{
	for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
		KeyIsDown[i] = false;
	
	enabled = false;
}
	
bool EventReceiver::OnEvent(const SEvent& event)
{
	
	if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
		// Remember whether each key is down or up
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		if (enabled)
		{
			// Creates an vector with only the interesting stones on the screen
			Stone* firstStones[NUMBER_OF_FRETS];
			for(unsigned int i=0; i<NUMBER_OF_FRETS; i++)
				if(player1->track->stones[i].size()>0)
					firstStones[i] = player1->track->stones[i].front();
				else
					firstStones[i] = NULL;
			
			// passes the vector to the verifyEvents function
			player1->fretting->verifyEvents(event, firstStones);
		}
	}
	
	return false;
}
