#include "EventManager.h"

void EventManager::pushEvent(const Event & e, E_PUSH_TYPE eventType){
	if (eventType == E_EVENT)
		eventQueue.push(e);
	else
		sendQueue.push(e);
}

size_t EventManager::eventQSize(){
	return eventQueue.size();
}

size_t EventManager::sendQSize(){
	return sendQueue.size();
}

Event EventManager::popEventQueue(){
	auto t = eventQueue.front();
	eventQueue.pop();
	return t;
}

Event EventManager::popSendQueue(){
	auto t = sendQueue.front();
	sendQueue.pop();
	return t;
}
