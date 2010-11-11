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
		
		void 	setEvents(EKEY_CODE events[5]);
		void 	setReceiver(eventReceiver *receiver);
		

		int 	verify_event(int color, Stone* stone, double musicTime, const double tolerance);

	private:
		eventReceiver *receiver;
		vector<EKEY_CODE> events;
		bool trackPressed[5];		// tell us the state of the tracks on the last
		bool rightPressed[5];		// frame		
};
		
	

