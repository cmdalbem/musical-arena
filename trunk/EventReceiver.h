#include <irrlicht.h>

#pragma once
using namespace irr;

class EventReceiver : public IEventReceiver
{
	public:
		// We use this array to store the current state of each key
		bool KeyIsDown[KEY_KEY_CODES_COUNT];
		SEvent event;
		
		bool	enabled;
	
		//////////////////////////////////////////////////////////// CONSTRUCTOR
		EventReceiver();

		// This is the one method that we have to implement
		virtual bool OnEvent(const SEvent& _event);

		// This is used to check whether a key is being held down
		virtual bool IsKeyDown(EKEY_CODE keyCode) const
		{	return KeyIsDown[keyCode];	}
};

