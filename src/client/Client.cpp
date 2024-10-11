
#include "Client.hpp"

Client::Client(int socket) : _connectionSock(socket)
{
	(void) _connectionSock;
}

Client::~Client(void)
{}

int Client::getSock(void) const {
	return (_connectionSock);
}
