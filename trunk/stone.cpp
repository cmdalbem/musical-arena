// stone.cpp
//

#include "stone.h"

stone::stone(musicEvent event, double endTime)
{
	this->event = event;
	this->destroy_time = endTime;
	x = 0;
	y = 0;
} 

stone::~stone() {}

void stone::draw()
{}

void stone::update()
{}
