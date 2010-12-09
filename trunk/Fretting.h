// fretting.h
//

#pragma once

#include "tree.hh"
#include <irrlicht.h>
using namespace irr;

#include "Stone.h"
#include "utils.h"
#include "Skill.h"
#include "EventReceiver.h"


#define ERROR 0
enum {
	HIT = 1, DO_NOTHING, NO_BUTTON_PRESSED
};

enum {
	KEYBOARD, JOYSTICK, AI
};


class skillTreeNode {
	
	public:
		skillTreeNode( buttonType b, Skill* s ) : button(b), skill(s) {}
		
		buttonType button;
		Skill *skill;
				
};
typedef tree<skillTreeNode> skillSearchTree;

										
class Fretting
{
	public:
		Fretting();
		Fretting(vector<Skill> *skills);
		Fretting(EKEY_CODE events[NFRETS]);
		//Fretting(int buttons[NUMBER_OF_FRETS], core::array<SJoystickInfo> joystickInfo, int joystickNumber);
		~Fretting();
		
		int type;
		double 	*musicTime;
		double 	tolerance;
		int 	frettingState;
		int		keyState[5];
		int		_hitting[NFRETS];
		EventReceiver *receiver;

		void 	setEvents(EKEY_CODE events[NFRETS]);
		void 	setEvents(int buttons[NFRETS], core::array<SJoystickInfo> 
							joystickInfo, int joystickNumber);

		int 	verifyEvents(SEvent *event, Stone* firstStones[NFRETS], int usingSkill);
		int		getFrettingState();
		void	printHitFret();
		void	lostNote();

	private:
		int keyboardPreFretting(SEvent *event);
		int joystickPreFretting(SEvent *event);

		skillSearchTree 			skillsTree;
		skillSearchTree::iterator 	actualSkillNode;
		
		vector<EKEY_CODE> 	_events;
		vector<int>			joystickButtons;
		int					joystickNumber;
        SEvent::SJoystickEvent JoystickState;
		
		bool 				_trackPressed[NFRETS];		// tell us the state of the tracks on the last
		bool 				_rightPressed[NFRETS];		// frame		
		
		Skill* 				findSkill( buttonType buttonPressed );
		void				generateSkillsTree( vector<Skill> *skills );
		void				initialize();
};

