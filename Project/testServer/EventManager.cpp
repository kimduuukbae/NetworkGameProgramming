#include "EventManager.h"
#include <mutex>
std::mutex m2;
EventManager::EventManager(){
	
}
void EventManager::pushEvent(const Event & e, E_PUSH_TYPE eventType){
	m2.lock();
	if (eventType == E_EVENT)
		eventQueue.push_back(e);
	else
		sendQueue.push_back(e);
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
	eventQueue.pop_front();
	return t;
}

Event EventManager::popSendQueue(){
	auto t = sendQueue.front();
	sendQueue.pop_front();
	return t;
}
