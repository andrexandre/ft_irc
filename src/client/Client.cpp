
#include "Client.hpp"

Client::Client(int socket)
: _connectionSock(socket), _hostname(""), _siteContent(NULL), _closeConn(false), _lastRequestTime(0)
{
	(void) _hostname;
	(void) _connectionSock;
	(void) _siteContent;
	(void) _lastRequestTime;
	(void) _closeConn;
	requestVal = 0;
}

Client::~Client(void)
{}

void Client::setHostName(string host) {
	_hostname = host;
}

void Client::setSiteContent(Server_block* ptr) {
	_siteContent = ptr;
}

void Client::setLastRquestTime(time_t actualTime) {
	_lastRequestTime = actualTime;
}

int Client::getSock(void) const {
	return (_connectionSock);
}

time_t Client::getLastRquestTime(void) const {
	return (_lastRequestTime);
}