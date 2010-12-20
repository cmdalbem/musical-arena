#include <irrlicht.h>
#include <vector>

#include "gui.h"

#pragma once
using namespace irr;
using namespace std;


class EventReceiver : public IEventReceiver
{
	public:
		//////////////////////////////////////////////////////////// CONSTRUCTOR
		EventReceiver(SAppContext * context);

	

		// We use this array to store the current state of each key
		bool KeyIsDown[KEY_KEY_CODES_COUNT];
		vector<SEvent> events;

		// This is the one method that we have to implement
		virtual bool OnEvent(const SEvent& _event);

		// This is used to check whether a key is being held down
		virtual bool IsKeyDown(EKEY_CODE keyCode) const
		{	return KeyIsDown[keyCode];	}
		
		bool 		refreshArray(const SEvent& _event);
		SEvent* 	getEvent();
		SEvent* 	getEvent(unsigned int i);
		int 		getEventsSize();
		void 		removeEvent();
		void 		clearEvents();
	
	private:
		 SAppContext * context;
};

