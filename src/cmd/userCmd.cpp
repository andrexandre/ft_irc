#include "../../Irc.hpp"

void Irc::userCmd(std::istringstream &ss, Client* actualClient)
{
	// map<int, Client*>::iterator it;
	// for (it = _clients.begin(); it != _clients.end(); it++)
	// {
	// }
	string str;
	ss >> str;
	actualClient->setUser(str);
}
