// Player.cpp
//

#include "Player.h"

#include <ctime>

Player::Player()
{
	this->inicialize();
}

void Player::inicialize()
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

Player::~Player()
{	}


