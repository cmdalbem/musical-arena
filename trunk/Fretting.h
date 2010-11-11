// fretting.h
//

#pragma once

#include "Stone.h"
#include "eventReceiver.h"
#include "utils.h"

#define ERROR				0
#define HIT					1
#define	DO_NOTHING			2
#define NO_BUTTON_PRESSED	3

class Fretting
{
	public:
		Fretting();
		Fretting(EKEY_CODE events[NUMBER_OF_FRETS], eventReceiver *receiver);
		~Fretting();

		void setEvents(EKEY_CODE events[NUMBER_OF_FRETS]);
		void setReceiver(eventReceiver *receiver);

		int 	verify_event(vector<Stone*> stones[NUMBER_OF_FRETS], double musicTime, const double tolerance);

	private:
		eventReceiver *receiver;
		vector<EKEY_CODE> events;
		bool trackPressed[5];		// tell us the state of the tracks on the last
		bool rightPressed[5];		// frame		
};

