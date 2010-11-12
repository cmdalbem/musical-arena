#include <irrlicht.h>
#include "Player.h"

#pragma once
using namespace irr;

class eventReceiver : public IEventReceiver
{
	private:
		// We use this array to store the current state of each key
		bool KeyIsDown[KEY_KEY_CODES_COUNT];

	public:
		Player *player1;
		Player *player2;
		bool	enabled;
	
		//////////////////////////////////////////////////////////// CONSTRUCTOR
		eventReceiver()
		{
			for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
				KeyIsDown[i] = false;
			
			enabled = false;
		}

		// This is the one method that we have to implement
		virtual bool OnEvent(const SEvent& event)
		{
			// Remember whether each key is down or up
			if (event.EventType == irr::EET_KEY_INPUT_EVENT)
				KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

			if (enabled)
				player1->fretting->verify_events(KeyIsDown, player1->track->stones);

			return false;
		}

		// This is used to check whether a key is being held down
		virtual bool IsKeyDown(EKEY_CODE keyCode) const
		{	return KeyIsDown[keyCode];	}
};

