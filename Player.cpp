// Player.cpp
//

#include "Player.h"

#include <ctime>

/////////////////////////////////////////////////////////////////// CONSTRUCTORS
Player::Player()
{
	this->initialize();
}

//////////////////////////////////////////////////////////////////// DESTRUCTORS
Player::~Player()
{	}

////////////////////////////////////////////////////////////// GETTERS & SETTERS


////////////////////////////////////////////////////////////////// OTHER METHODS
void Player::initialize()
{
	srand((unsigned)time(0));
	
	maxHP = 10 + (rand() % 16);
	HP = maxHP;
	maxStamina = 30 + (rand() % 21);
	stamina = maxStamina;
	
	XP = 0;
	level = 1;
	gold = 0;
	gotAnEvent = 0;
	
	status = ST_NORMAL;
}

void Player::update()
{
	SEvent *anEvent;

	track->update();
	
	if ( track->nonPressedChord ) // the player left behind a chord he should have played
	{
		fretting->lostNote();
		//cout << "the player left " << track->notesOnChord << " behind" << endl;
	}
	else
	{
		// Creates an vector with only the interesting stones on the screen
		Stone* firstStones[NFRETS];
		gotAnEvent = 1;
		for(unsigned int i=0; i<NFRETS; i++)
			if(track->stones[i].size() > 0)
				firstStones[i] = track->stones[i].front();
			else
				firstStones[i] = NULL;
		if (fretting->receiver->enabled)
		{
			while((anEvent = (fretting->receiver->getEvent())) && (gotAnEvent != 0))
			{
				//sem_wait(fretting->receiver->semaphore);
				gotAnEvent = fretting->verifyEvents( anEvent, firstStones );
				
				if (gotAnEvent != 0)
					// removes the first event of the events vector (so we can deal with the others =D)
					fretting->receiver->removeEvent();
					
			}
			//cout << "vai verificar eventos" << endl;
			//cout << "terminou de verificar eventos" << endl;
		}
	}
}

void Player::addSkill( Skill s )
{
	skills.push_back(s);	
}

