#include "../../Irc.hpp"

void Irc::quitCmd(std::istringstream &ss, Client* actualClient)
{
	(void)ss;
	map<int, Client *>::iterator it = _clients.find(actualClient->getSock());
	delete it->second;
	epfds->deleteFd(it->first);
	_clients.erase(it->first);
}
