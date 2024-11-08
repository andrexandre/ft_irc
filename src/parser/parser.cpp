#include "../../Irc.hpp"

void Irc::parsing(int targetFd)
{
	char buffer[30000];
	bzero(buffer, sizeof(buffer));
	istringstream ss;
	Client *client = findClient(targetFd);

	if (recv(targetFd, &buffer, sizeof(buffer), 0) <= 0)
		return quitCmd(ss, client);
	client->_buffer += string(buffer);
	if (client->_buffer.find('\n') == string::npos)
		return;
	requests.insert(std::make_pair(targetFd, client->_buffer));
	epfds->modFd(targetFd, EPOLLOUT);
	client->_buffer.clear();
}
