
#include "Client.hpp"

Client::Client(int socket) : _connectionSock(socket), _isAuthenticated(0) {}

Client::~Client(void) {}

int Client::getSock(void) const {
	return (_connectionSock);
}

int Client::isAuthenticated(void) const {
	return (_isAuthenticated);
}

string Client::getNick(void) const { 
	return (_nick);
}

string Client::getUser(void) const { 
	return (_user);
}

string Client::getPassWord(void) const { 
	return (_passWord);
}

void Client::authenticate(void) {
	_isAuthenticated = 1;
}

void Client::setNick(string name) { 
	_nick = name;
}

void Client::setUser(string name) { 
	_user = name;
}

void Client::setPassWord(string name) { 
	_passWord = name;
}
