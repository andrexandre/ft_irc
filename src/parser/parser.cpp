#include "../../Irc.hpp"

void Irc::parsing(int targetFd)
{
	char buffer[30000];
	bzero(buffer, sizeof(buffer));

	if (recv(targetFd, &buffer, sizeof(buffer), 0) <= 0)
	{
		map<int, Client *>::iterator it = _clients.find(targetFd);
		cout << YELLOW << "Closing connection, fd: " << it->first << END << endl;
		delete it->second;
		epfds->deleteFd(it->first);
		_clients.erase(it->first);
		return;
	}
	Client *client = findClient(targetFd);
	client->_buffer += string(buffer);
	if (client->_buffer.find('\n') == string::npos)
		return;
	requests.insert(std::make_pair(targetFd, client->_buffer));
	epfds->modFd(targetFd, EPOLLOUT);
	client->_buffer.clear();
}
