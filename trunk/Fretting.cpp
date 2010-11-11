// fretting.cpp
//
#include "Fretting.h"
#include "limits.h"

/////////////////////////////////////////////////////////////////// CONSTRUCTORS
Fretting::Fretting()
{
	for (int i = 0; i < NUMBER_OF_FRETS; i++)
	{
		this->trackPressed[i] = false;
		this->rightPressed[i] = false;
	}
}

Fretting::Fretting(EKEY_CODE events[NUMBER_OF_FRETS], eventReceiver *receiver)
{
	setEvents(&(events[NUMBER_OF_FRETS]));
	setReceiver(receiver);
	for (int i = 0; i < NUMBER_OF_FRETS; i++)
	{
		this->trackPressed[i] = false;
		this->rightPressed[i] = false;
	}
}

//////////////////////////////////////////////////////////////////// DESTRUCTORS
Fretting::~Fretting()
{	}

////////////////////////////////////////////////////////////// GETTERS & SETTERS
void Fretting::setEvents(EKEY_CODE events[NUMBER_OF_FRETS])
{
	for (int i = 0; i < NUMBER_OF_FRETS; i++)
		this->events.push_back(events[i]);
}

void Fretting::setReceiver (eventReceiver *receiver)
{	this->receiver = receiver;	}

////////////////////////////////////////////////////////////////// OTHER METHODS

int Fretting::verify_event(int color, Stone* stone, double musicTime, const double tolerance)
{
	double noteCreationTime = INT_MAX;
	double noteDestructionTime = INT_MIN;

	if (stone != NULL) {
		noteCreationTime = stone->event.time;
		noteDestructionTime = stone->destroyTime;
	}
	
	if (receiver->IsKeyDown(events[color]))
	{
		if(trackPressed[color])
		{
			if( musicTime < noteDestructionTime &&  // tem uma nota tocando
				rightPressed[color]) 				// acertei (no último frame)

			return 1;
				//ganho ponto
			/*
			else if ( // tem uma nota tocando
					  // errei
					)
				;
			else if ( // nao tem nada tocando)
				;
			*/
		}
		else
		{
			if( (musicTime > noteCreationTime - tolerance) &&
				(musicTime < noteCreationTime + tolerance) ) // eu acertei AGORA
			{
				// ganho ponto
				trackPressed[color] = true;
				rightPressed[color] = true;
				return 1;
			}
			else
			{
				// perco multiplier, vida, etc
				trackPressed[color] = true;
				rightPressed[color] = false;
				return -1;
			}
		}
	}
	else
	{
		trackPressed[color] = false;
		rightPressed[color] = false;
	}
	
	return 0;
}
