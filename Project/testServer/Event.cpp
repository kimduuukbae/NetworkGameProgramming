#include "Event.h"


Event::Event(simplePacket& p, E_TARGET_VALUE t) : var{ p }, target{ t }{

}

Event::Event(shootPacket& p, E_TARGET_VALUE t) : var{ p }, target{ t }{

}
Event::Event(posPacket& p, E_TARGET_VALUE t) : var{ p }, target{ t }{

}
Event::Event(allPacket& p, E_TARGET_VALUE t) : var{ p }, target{ t }{

}
Event::Event(itemPacket& p, E_TARGET_VALUE t) : var { p }, target{ t }{

}
std::tuple<simplePacket*, shootPacket*, posPacket*, allPacket*, itemPacket*> Event::getPacket(){
	return std::make_tuple<simplePacket*, shootPacket*, posPacket*, allPacket*>(std::get_if<0>(&var), std::get_if<1>(&var), std::get_if<2>(&var), std::get_if<3>(&var), std::get_if<4>(&var));
}

int Event::getTarget(){
	return static_cast<int>(target);
}
