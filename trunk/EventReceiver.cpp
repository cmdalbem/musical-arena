// EventReceiver.cpp
//

#include "EventReceiver.h"

#include <iostream>

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


bool loadedNotes=false, 
	 loadedSong1=false,
	 loadedSong2=false;


EventReceiver::EventReceiver( SAppContext * context )  : context(context)
{
	for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
		KeyIsDown[i] = false;
}
	
bool EventReceiver::OnEvent(const SEvent& _event)
{
	if (_event.EventType == EET_GUI_EVENT) {
		//cout << "Received a GUI Event." << endl;
		/////////////////
		// GUI Events
		/////////////////
		
		s32 id = _event.GUIEvent.Caller->getID();
		IGUIEnvironment* env = context->device->getGUIEnvironment();

		switch(_event.GUIEvent.EventType) {			
			case EGET_FILE_SELECTED:
				switch(id)
				{
					case GUI_ID_LOAD_NOTES_DIALOG:
						{
							IGUIFileOpenDialog* dialog = (IGUIFileOpenDialog*)_event.GUIEvent.Caller;
							context->diffBox->clear();
							bool parsedOk = context->loadNotes( std::string(core::stringc(dialog->getFileName()).c_str()));
							
							if( parsedOk ) {
								context->diffBox->setEnabled(true);
								loadedNotes = true;
							}
							else {
								context->diffBox->setEnabled(false);
								loadedNotes = false;
								env->addMessageBox(L"Error", L"Sorry, I couldn't find any notes on this file. Maybe you picked the wrong one?" );
							}
							
							context->startButton->setEnabled(loadedNotes && loadedSong1);
						}
						break;
					
					case GUI_ID_LOAD_MUSIC1_DIALOG:
						{
							IGUIFileOpenDialog* dialog = (IGUIFileOpenDialog*)_event.GUIEvent.Caller;
							context->loadSong( std::string(core::stringc(dialog->getFileName()).c_str()), 0);
							loadedSong1 = true;
														
							context->startButton->setEnabled(loadedNotes && loadedSong1);
						}					
						break;
						
					case GUI_ID_LOAD_MUSIC2_DIALOG:
						{
							IGUIFileOpenDialog* dialog = (IGUIFileOpenDialog*)_event.GUIEvent.Caller;
							context->loadSong( std::string(core::stringc(dialog->getFileName()).c_str()), 1);
							loadedSong2 = true;
						}					
						break;						
				}
				break;

			case EGET_BUTTON_CLICKED:
				switch(id) {
					case GUI_ID_QUIT_BUTTON:
						context->device->closeDevice();
						return true;

					case GUI_ID_START_BUTTON:
					{
						// get selected difficulty
						int difficulty = context->diffBox->getItemData(context->diffBox->getSelected());					
						
						// get selected control modes
						controlType playerControls[NPLAYERS];
						for(int i=0; i<NPLAYERS; i++)
							playerControls[i] = (controlType) context->control[i]->getItemData(context->control[i]->getSelected());
						
						// get selected instruments
						instrumentType playerInstruments[NPLAYERS];
						for(int i=0; i<NPLAYERS; i++)
							playerInstruments[i] = (instrumentType) context->instrument[i]->getItemData(context->instrument[i]->getSelected());
						
						context->mainWindow->remove();
						context->mainBg->remove();
						
						context->state = GUI_PLAYING;
						context->startGame(difficulty, playerControls, playerInstruments);
						return true;
					}
					
					case GUI_ID_LOAD_NOTES:
						env->addFileOpenDialog(0,true,0,GUI_ID_LOAD_NOTES_DIALOG);
						break;
					
					case GUI_ID_LOAD_MUSIC1:
						env->addFileOpenDialog(0,true,0,GUI_ID_LOAD_MUSIC1_DIALOG);
						break;
					
					case GUI_ID_LOAD_MUSIC2:
						env->addFileOpenDialog(0,true,0,GUI_ID_LOAD_MUSIC2_DIALOG);
						break;
						
					default:
						return false;
				}
			default:
				break;
		}
	}
	else {
		/////////////////
		// MusA Events
		/////////////////
		
		events.push_back(_event);
		refreshArray(_event);
	}
}

bool EventReceiver::refreshArray(const SEvent& _event)
{
	if (_event.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		// Remember whether each key is down or up
		KeyIsDown[_event.KeyInput.Key] = _event.KeyInput.PressedDown;
		//cout << "recebeu evento: " << _event.KeyInput.Key << "\tpressionou: " << _event.KeyInput.PressedDown << endl;
	}
	else
		return false;
	
	return true;
}

int EventReceiver::getEventsSize()
{
	return events.size();
}

SEvent* EventReceiver::getEvent()
{
	if (events.size() > 0)
		return &(events[0]);
	else
		return NULL;
}

SEvent* EventReceiver::getEvent(unsigned int i)
{
	if (events.size() > i)
		return &(events[i]);
	else
		return NULL;
}

void EventReceiver::removeEvent()
{
	if (events.size() > 0)
	{
		//sem_wait(semaphore);
		events.erase(events.begin());
	}
}

void EventReceiver::clearEvents()
{
	events.clear();
}
