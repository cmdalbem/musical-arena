// fretting.h
//

#pragma once

#include "Stone.h"
#include "eventReceiver.h"
#include "utils.h"

class Fretting
{
	public:
		Fretting();
		Fretting(EKEY_CODE events[NUMBER_OF_FRETS], eventReceiver *receiver);
		~Fretting();
		
		
		void setEvents(EKEY_CODE events[NUMBER_OF_FRETS]);
		void setReceiver(eventReceiver *receiver);
		
		void verify_event(int color, vector<Stone*> stonesOnScreen[NUMBER_OF_FRETS], double musicTime, const double tolerance);

	eventReceiver *receiver;
	vector<EKEY_CODE> events;
	bool trackPressed[NUMBER_OF_FRETS];		// tell us the state of the tracks on the last
	bool rightPressed[NUMBER_OF_FRETS];		// frame
};

