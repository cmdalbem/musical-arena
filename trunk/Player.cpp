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
		// causes some damage on the player, to be defined yet
		// HP = HP - ( ((algum_valor) * notesOnChord) - alguma_defesa), etc...
		
		for (int i = 0; i < NUMBER_OF_FRETS; i++)
			if (fretting->_hitting[i] != 0)
				fretting->_hitting[i] = 2;	// even some buttons are pressed, it's like if it were pressed wrong
			else
				fretting->_hitting[i] = 0;
	}
}


