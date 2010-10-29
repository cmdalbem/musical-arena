// stone.cpp
//

#include "stone.h"

stone::stone(musicEvent event, double endTime)
{
	this->event = event;
	this->destroy_time = endTime;
	//this->x = 0;
	//this->y = 0;
} 

stone::~stone() {}

/*
void stone::draw()
{}

void stone::update()
{}
*/
