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

void Fretting::verify_event(int color, vector<Stone*> stonesOnScreen[NUMBER_OF_FRETS], double musicTime, const double tolerance)
{
	double noteCreationTime = INT_MAX;
	double noteDestructionTime = INT_MIN;

	if (stonesOnScreen[color].size() > 0)
	{
		// semáforo lock;
		noteCreationTime = stonesOnScreen[color][0]->event.time;
		noteDestructionTime = stonesOnScreen[color][0]->destroyTime;
		// semáforo unlock
	}
	
	if (receiver->IsKeyDown(events[color]))
	{
		if(trackPressed[color])
		{
			if( musicTime < noteDestructionTime &&  // tem uma nota tocando
				rightPressed[color]) 				// acertei (no último frame)
				cout << "ganho ponto" << endl;
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
				cout << "ganho ponto" << endl;
				trackPressed[color] = true;
				rightPressed[color] = true;
				// ganho ponto
			}
			else
			{
				// perco multiplier, vida, etc
				cout << "preco multiplier, vida, etc" << endl;
				trackPressed[color] = true;
				rightPressed[color] = false;
			}
		}
	}
	else
	{
		trackPressed[color] = false;
		rightPressed[color] = false;
	}
}
