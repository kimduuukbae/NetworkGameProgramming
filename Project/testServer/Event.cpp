#include "Event.h"


Event::Event(simplePacket& p) : var{ p }{

}

Event::Event(shootPacket& p) : var{ p }{

}

std::tuple<simplePacket*, shootPacket*> Event::getPacket(){
	return std::make_tuple<simplePacket*, shootPacket*>(std::get_if<0>(&var), std::get_if<1>(&var));
}