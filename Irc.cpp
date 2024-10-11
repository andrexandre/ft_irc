#include "Irc.hpp"

Irc::Irc(void) {}

Irc::~Irc(void) 
{
	
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end();)
		deleteClient(it);
	
	delete epfds;
}
