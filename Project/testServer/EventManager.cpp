#include "EventManager.h"
#include <mutex>
std::mutex m2;

static EventManager* _instance = nullptr;
EventManager* EventManager::instance(){
	if (_instance == nullptr)
		_instance = new EventManager();
	return _instance;
}
EventManager::EventManager(){
	delete _instance;
}
void EventManager::pushEvent(const Event& e, E_PUSH_TYPE eventType){
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
	m2.lock();
	auto t = eventQueue.front();
	eventQueue.pop_front();
	m2.unlock();
	return t;
}

Event EventManager::popSendQueue(){
	m2.lock();
	auto t = sendQueue.front();
	sendQueue.pop_front();
	m2.unlock();
	return t;
}
