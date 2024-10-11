#include "Irc.hpp"

Irc::Irc(void) {}

Irc::~Irc(void) 
{
	
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end();)
		deleteClient(it);
	if (epfds)
		delete epfds;
}


void Irc::setPassword(string arg) {
	_passWord = arg;
}

void Irc::setPort(string arg)
{
	char *end;
	int num = strtol(arg.c_str(), &end, 10);

	if (*end || num <= 0  || num >= 65535)
		throw std::runtime_error("Error: Invalid port!");;

	_port = num;	
}