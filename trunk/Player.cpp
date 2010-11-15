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
Fretting* Player::getFretting()
{	return fretting;	}


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
}

void Player::processEvent( musicEvent event )
{
	track->processEvent(event);
	
	// tests if the player left behind some notes
	if (track->nonPressedChord)
	{
		// causes some damage on the player, related to the number of wrong notes on track;
	}
}


