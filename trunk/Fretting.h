// fretting.h
//

#pragma once

#include "Stone.h"
#include "eventReceiver.h"
#include "utils.h"
#include "tree.hh"
#include "Skill.h"

#define ERROR 0
enum {
	HIT = 1, DO_NOTHING, NO_BUTTON_PRESSED
};


struct skillTreeNode {
	buttonType button;
	Skill *skill;
};


class Fretting
{
	public:
		Fretting();
		Fretting(EKEY_CODE events[NUMBER_OF_FRETS], eventReceiver *receiver);
		~Fretting();

		void 	setEvents(EKEY_CODE events[NUMBER_OF_FRETS]);
		void 	setReceiver(eventReceiver *receiver);

		int 	verify_event(vector<Stone*> stones[NUMBER_OF_FRETS], double musicTime, const double tolerance);

	private:
		tree<skillTreeNode> skillsTree;
		eventReceiver 		*receiver;
		vector<EKEY_CODE> 	events;
		bool 				trackPressed[5];		// tell us the state of the tracks on the last
		bool 				rightPressed[5];		// frame		
};

