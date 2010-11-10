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
		Fretting(EKEY_CODE events[5], eventReceiver *receiver);
		~Fretting();
		
		
		void setEvents(EKEY_CODE events[5]);
		void setReceiver(eventReceiver *receiver);
		
		void verify_event(int color, vector<Stone*> stonesOnScreen[5], double musicTime, const double tolerance);

	eventReceiver *receiver;
	vector<EKEY_CODE> events;
	bool trackPressed[5];		// tell us the state of the tracks on the last
	bool rightPressed[5];		// frame
};

