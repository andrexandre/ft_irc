#include "webserv.hpp"

WebServ::WebServ(void) {}

WebServ::~WebServ(void) 
{

	for (size_t i = 0; i < _serversConfig.size(); i++)
		delete _serversConfig[i];
	
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end();)
		deleteClient(it);
	
	delete epfds;

	// _clients.clear();
	_serversConfig.clear();
}

void WebServ::setServers(Server_block* other) {
	_serversConfig.push_back(other);
}


// This method will verify if all the servers as the mandatory directives set
void WebServ::mandatoryConfig(void) const
{
	if (_serversConfig.size() == 0)
		throw("Invalid file!");
	for (size_t i = 0; i < _serversConfig.size(); i++)
		_serversConfig[i]->validateMandatoryConfig();
}

void WebServ::printServers(void)
{
	for (size_t i = 0; i < _serversConfig.size(); i++)
	{
		_serversConfig[i]->printInfo();
		cout << endl;
	}
}
