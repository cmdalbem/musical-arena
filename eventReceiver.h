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
		int contador;
	
		//////////////////////////////////////////////////////////// CONSTRUCTOR
		eventReceiver()
		{
			for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
				KeyIsDown[i] = false;
			
			enabled = false;
			contador = 0;
		}

		// This is the one method that we have to implement
		virtual bool OnEvent(const SEvent& event)
		{
			// Remember whether each key is down or up
			if (event.EventType == irr::EET_KEY_INPUT_EVENT)
				KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

			cout << "contador: " << contador++ << endl;
			if (enabled)				
				player1->fretting->verify_events(event, player1->track->stones);
			
			return false;
		}

		// This is used to check whether a key is being held down
		virtual bool IsKeyDown(EKEY_CODE keyCode) const
		{	return KeyIsDown[keyCode];	}
};

