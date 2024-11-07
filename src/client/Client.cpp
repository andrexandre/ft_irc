
#include "Client.hpp"

Client::Client(int socket) : _connectionSock(socket)
{
	(void) _connectionSock;
	_isAuthenticated = 0;
}

Client::~Client(void) {}

int Client::getSock(void) const {
	return (_connectionSock);
}

int Client::isAuthenticated(void) const {
	return (_isAuthenticated);
}

void Client::authenticate(void) 
{
	_isAuthenticated = 1;
}
