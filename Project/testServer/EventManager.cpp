#include "EventManager.h"
#include <mutex>
std::mutex m2;
void EventManager::pushEvent(const Event & e, E_PUSH_TYPE eventType){
	m2.lock();
	if (eventType == E_EVENT)
		eventQueue.push(e);
	else
		sendQueue.push(e);
	m2.unlock();
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
