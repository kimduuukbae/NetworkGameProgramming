#pragma once
#include <variant>
#include <tuple>
#include "packetDataStructure.h"
enum E_TARGET_VALUE {
	E_ONE,
	E_EVERYONE
};
class Event {
public:
	Event(simplePacket& p, E_TARGET_VALUE t = E_EVERYONE);
	Event(shootPacket& p, E_TARGET_VALUE t = E_EVERYONE);
	Event(posPacket& p, E_TARGET_VALUE t = E_EVERYONE);
	Event(allPacket& p, E_TARGET_VALUE t = E_EVERYONE);
	std::tuple<simplePacket*, shootPacket*, posPacket*, allPacket*> getPacket();
	int getTarget();
private:
	std::variant<simplePacket, shootPacket, posPacket, allPacket> var;
	E_TARGET_VALUE target;
};

