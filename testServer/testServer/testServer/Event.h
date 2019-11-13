#pragma once
#include <variant>
#include <tuple>
#include "packetDataStructure.h"

class Event {
public:
	Event(simplePacket p);
	Event(shootPacket p);
	std::tuple<simplePacket*, shootPacket*> getPacket();
private:
	std::variant<simplePacket, shootPacket> var;
};


