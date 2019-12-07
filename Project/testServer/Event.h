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
	Event(const simplePacket& p, E_TARGET_VALUE t = E_EVERYONE);
	Event(const shootPacket& p, E_TARGET_VALUE t = E_EVERYONE);
	Event(const posPacket& p, E_TARGET_VALUE t = E_EVERYONE);
	Event(const allPacket& p, E_TARGET_VALUE t = E_EVERYONE);
	Event(const itemPacket& p, E_TARGET_VALUE t = E_EVERYONE);
	std::tuple<simplePacket*, shootPacket*, posPacket*, allPacket*, itemPacket*> getPacket();
	int getTarget();
private:
	std::variant<simplePacket, shootPacket, posPacket, allPacket, itemPacket> var;
	E_TARGET_VALUE target;
};

