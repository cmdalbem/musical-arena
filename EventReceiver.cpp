// EventReceiver.cpp
//

#include "EventReceiver.h"

#include <iostream>

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


EventReceiver::EventReceiver( SAppContext & context )  : Context(context)
{
	for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
		KeyIsDown[i] = false;
	
	enabled = false;
}
	
bool EventReceiver::OnEvent(const SEvent& _event)
{
	if (_event.EventType == EET_GUI_EVENT) {
		cout << "GUI EVENT MOTHERFUCKEEEEEEERRRRRRRRRRR" << endl;
		
		// GUI Events
		s32 id = _event.GUIEvent.Caller->getID();
		//IGUIEnvironment* env = Context.device->getGUIEnvironment();

		if(_event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			cout << "clicou em algum botao" << endl;
			switch(id) {
				case GUI_ID_QUIT_BUTTON:
					cout << "Quitou o fdp" << endl;
					Context.device->closeDevice();
					return true;

				case GUI_ID_NEW_WINDOW_BUTTON:
					/*{
					Context.listbox->addItem(L"Window created");
					Context.counter += 30;
					if (Context.counter > 200)
							Context.counter = 0;

					IGUIWindow* window = env->addWindow(
							rect<s32>(100 + Context.counter, 100 + Context.counter, 300 + Context.counter, 200 + Context.counter),
							false, // modal?
							L"Test window");

					env->addStaticText(L"Please close me",
							rect<s32>(35,35,140,50),
							true, // border?
							false, // wordwrap?
							window);
					}*/
					return true;

				case GUI_ID_FILE_OPEN_BUTTON:
					//Context.listbox->addItem(L"File open");
					//env->addFileOpenDialog(L"Please choose a file.");
					return true;

				default:
					return false;
			}
		}
	}
	else {
		// MusA Event
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
