// stone.h
//

#ifndef STONE_H
#define STONE_H

#include "Decoder.h"

class stone
{
	public:
		stone(musicEvent event, double endTime);
		~stone();

		musicEvent	event;
		double		destroy_time;
		//double	x, y;		// coordinates
	
	//void draw();
	//void update();	
};

#endif
