#include "webserv.hpp"

WebServ::WebServ(void) {}

WebServ::~WebServ(void) {
	for (size_t i = 0; i < _servers.size(); i++)
		delete _servers[i];
	
	_servers.clear();
}

void WebServ::setServers(Server_block* other) {
	_servers.push_back(other);
}


// This method will verify if all the servers as the mandatory directives set
void WebServ::mandatoryConfig(void) const {
	
	if (_servers.size() == 0)
		throw("Invalid file!");
	for (size_t i = 0; i < _servers.size(); i++)
		_servers[i]->validateMandatoryConfig();
}

void WebServ::printServers(void)
{
	for (size_t i = 0; i < _servers.size(); i++)
	{
		_servers[i]->printInfo();
		cout << endl;
	}
}
