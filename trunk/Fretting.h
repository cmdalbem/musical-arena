// fretting.h
//

#pragma once

#include "Stone.h"
#include "utils.h"
#include "tree.hh"
#include "Skill.h"

#include <irrlicht.h>
using namespace irr;

#define ERROR 0
enum {
	HIT = 1, DO_NOTHING, NO_BUTTON_PRESSED
};


class skillTreeNode {
	
	public:
		skillTreeNode( buttonType b, Skill* s ) : button(b), skill(s) {}
		
		buttonType button;
		Skill *skill;
				
};


class Fretting
{
	public:
		double *musicTime;
		double tolerance;
		
		int acertadas;
		int erradas;
		int neutras;
	
		Fretting();
		Fretting( vector<Skill> *skills );
		Fretting(EKEY_CODE events[NUMBER_OF_FRETS]);

		~Fretting();

		void 	setEvents(EKEY_CODE events[NUMBER_OF_FRETS]);

		int 	verify_events(bool KeyIsDown[KEY_KEY_CODES_COUNT], vector<Stone*> stones[NUMBER_OF_FRETS])
		{
			double noteCreationTime[NUMBER_OF_FRETS];
			double noteDestructionTime[NUMBER_OF_FRETS];
		Skill*				findSkill( buttonType buttonPressed );

			bool pressed[NUMBER_OF_FRETS];
			bool mustPress[NUMBER_OF_FRETS];
			bool mustPressYet[NUMBER_OF_FRETS];
			bool chords[NUMBER_OF_FRETS];
			bool rightNow[NUMBER_OF_FRETS];
			int	 chordCounter = 0;
			
			// verifies what buttons are pressed
			for (unsigned int index = 0; index < events.size(); index++)
			{
				if (stones[index].size() > 0)
				{
					noteCreationTime[index] = stones[index][0]->event.time;
					noteDestructionTime[index] = stones[index][0]->destroyTime;
					if (*musicTime > noteCreationTime[index] - tolerance &&
						*musicTime < noteCreationTime[index] + tolerance)
						mustPress[index] = true;
					else
						mustPress[index] = false;
					
					if (*musicTime > noteDestructionTime[index])
						mustPressYet[index] = false;
					else
						mustPressYet[index] = true;
				}
				else
				{
					noteCreationTime[index] = INT_MAX;
					noteDestructionTime[index] = INT_MIN;
					mustPressYet[index] = false;
					mustPress[index] = false;
				}
				
				rightNow[index] = false;
				chords[index] = false;
				EKEY_CODE code = events[index];
				pressed[index] = KeyIsDown[code];
			}
			
			int chordCreationTime = INT_MAX;
			for (int i = 0; i < NUMBER_OF_FRETS; i++)
				if (noteCreationTime[i] < chordCreationTime)
					chordCreationTime = noteCreationTime[i];
					
			for (int i = 0; i < NUMBER_OF_FRETS; i++)
				for (int j = 0; j < NUMBER_OF_FRETS; j++)
					if (noteCreationTime[i] == noteCreationTime[j]
						&& i != j && chordCreationTime == noteCreationTime[i])
					{
						chords[i] = true;
						chords[j] = true;
					}
			for (int i = 0; i < NUMBER_OF_FRETS; i++)
				if (chords [i])
					chordCounter++;
			
			for (int i = 0; i < NUMBER_OF_FRETS; i++)
			{
				
				if (mustPress[i])
					if (pressed[i])
						if (rightPressed[i])
							if (chords[i])
							{
								chordCounter--;
								if (chordCounter == 0)
								{
									// acertou --> track/right[i] = true
									acertadas++;
//									cout << "acertou" << endl;
									trackPressed[i] = true;
									rightPressed[i] = true;
								}
							}
							else
							{
								// errou -->  trackPressed = true / rightPressed[i] = false
								erradas++;
//								cout << "errou" << endl;
								trackPressed[i] = true;
								rightPressed[i] = false;
							}
						else
						{
							if (trackPressed[i])
							{
								// nao faz nada --> trackPressed[i] = true / rightPressed = false
								neutras++;
	//							cout << "faz nada" << endl;
								trackPressed[i] = true;
								rightPressed[i] = false;
							}
							else
							{
								// acertou
								acertadas++;
								trackPressed[i] = true;
								rightPressed[i] = true;
							}	
						}
					else
					{
						// espera (nao faz nada) --> rightPressed/trackPressed = false;
						//neutras++;
//						cout << "faz nada" << endl;
						trackPressed[i] = false;
						rightPressed[i] = false;
					}
				else if (pressed[i])
					if (trackPressed[i])
					{
						// nao faz nada --> trackPressed[i] = true/ rightPressed = false
//						cout << "faz nada" << endl;
						trackPressed[i] = true;
						rightPressed[i] = false;	
					}
					else
					{
						// errou --> trackPressed[i] = true/ rightPressed = false
						erradas++;
//						cout << "errou" << endl;
						trackPressed[i] = true;
						rightPressed[i] = false;	
					}
				else
				{
					// nao faz nada ("acertou não apertar nada =D")
					// trackPressed = true/ rightPressed = false;
//					cout << "faz nada" << endl;
					trackPressed[i] = true;
					rightPressed[i] = false;	
				}
						
				if (mustPressYet[i])
					if (trackPressed[i])
						if (pressed[i])
							if (rightPressed[i])
							{
								// acertou -> track/right = true
								acertadas++;
//								cout << "acertou" << endl;
								trackPressed[i] = true;
								rightPressed[i] = true;	
							}
							else
							{
								// faz nada -> track = true, right = false
//								cout << "faz nada" << endl;
								trackPressed[i] = true;
								rightPressed[i] = false;	
							}
						else
						{
							// faz nada -> track = false, right = false
//							cout << "faz nada" << endl;
							trackPressed[i] = false;
							rightPressed[i] = false;	
						}
					else 
					if (pressed[i])
					{
						// errei -> track = true, right = false
						erradas++;
//						cout << "errou" << endl;
						trackPressed[i] = true;
						rightPressed[i] = false;	
					}	
					else
					{
						// nao faz nada -> track = false, right = false
//						cout << "faz nada" << endl;
						trackPressed[i] = false;
						rightPressed[i] = false;	
					}
				else
					if (pressed[i])
						if (trackPressed[i])
						{
							// nao faz nada -> track = true, right = false
//							cout << "faz nada" << endl;
							trackPressed[i] = true;
							rightPressed[i] = false;	
						}
						else
						{
							// errei -> track = true, right = false
							erradas++;
//							cout << "errou" << endl;
							trackPressed[i] = true;
							rightPressed[i] = false;	
						}
					else
					{
						// nao faz nada -> track = false, right = false
//						cout << "faz nada" << endl;
						trackPressed[i] = false;
						rightPressed[i] = false;	
					}
							
						
				
			}
			cout << "acertadas: " << acertadas <<
				 "\terradas: " << erradas <<
				 "\tneutras: " << neutras << endl;
			
			return 1;
				// vejo quais botões estão apertados;
				// se algum foi apertado agora;
				// // // se tem alguma nota nova pra ele;
				// // // // se tem uma outra nota com início no mesmo momento;
				// // // // // se tem alguma nota apertada já errada
				// // // // // // errou
				// // // // // se nao tem alguma nota apertada errada
				// // // // // // não faz nada (espera que o jogador ainda pode apertar as outras)
				// // // // se não tem outra nota com o início no mesmo momento;
				// // // // // acertou
				// // // se não tem alguma nota nova pra ele
				// // // // errou
				// se ninguém foi apertado agora
				// // se tem nota tocando agora & acertei (last frame)
				// // // "dá ponto"
				// // se nao tem nota tocando agora
				// // // nao faz nada
				// O caso de o jogador deixar um nota passar será tratado pela track =D

		}

	private:
		tree<skillTreeNode> 			skillsTree;
		tree<skillTreeNode>::iterator 	actualSkillNode;
		
		vector<EKEY_CODE> 	events;
		bool 				trackPressed[NUMBER_OF_FRETS];		// tell us the state of the tracks on the last
		bool 				rightPressed[NUMBER_OF_FRETS];		// frame		
		
		Skill* 				findSkill( buttonType buttonPressed );
		void				generateSkillsTree( vector<Skill> *skills );
		void				initialize();
};

