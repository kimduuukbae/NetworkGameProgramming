#pragma once
#include <deque>
#include "Event.h"
enum E_PUSH_TYPE {
	E_EVENT,
	E_SEND
};

class EventManager {
public:
	EventManager();
	~EventManager() = default;
	void pushEvent(const Event& e, E_PUSH_TYPE eventType);

	size_t eventQSize();
	size_t sendQSize();

	_NODISCARD Event popEventQueue();
	_NODISCARD Event popSendQueue();
private:
	std::deque<Event> eventQueue;
	std::deque<Event> sendQueue;
};