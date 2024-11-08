#include "../../Irc.hpp"

void Irc::quitCmd(istringstream &ss, Client* actualClient)
{
	(void)ss;
	map<int, Client *>::iterator it = _clients.find(actualClient->getSock());
	cout << YELLOW << "Closing connection, fd: " << it->first << END << endl;
	delete it->second;
	epfds->deleteFd(it->first);
	_clients.erase(it->first);
}
