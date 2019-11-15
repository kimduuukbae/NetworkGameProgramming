#include "Event.h"

Event::Event(simplePacket p) : var{ p }
{
}

Event::Event(shootPacket p) : var{ p }
{
}

std::tuple<simplePacket*, shootPacket*> Event::getPacket()
{
	return std::tuple<simplePacket*, shootPacket*>();
}
