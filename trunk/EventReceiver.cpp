// EventReceiver.cpp
//

#include "EventReceiver.h"

#include <iostream>


EventReceiver::EventReceiver()
{
	for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
		KeyIsDown[i] = false;
	
	enabled = false;
}
	
bool EventReceiver::OnEvent(const SEvent& _event)
{

	if (enabled)
	{
		
		events.push_back(_event);
		refreshArray(_event);
		//sem_post(semaphore);
	}
	return false;
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
