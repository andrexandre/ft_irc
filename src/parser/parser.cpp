#include "../../Irc.hpp"

void Irc::parsing(int targetFd)
{
	char buffer[30000];
	bzero(buffer, sizeof(buffer));
	if (read(targetFd, &buffer, 30000) < 0)
		throw std::runtime_error("reading the fd");
	
	// authenticateClient() will be here

	requests.insert(std::make_pair(targetFd, string(buffer)));
	epfds->modFd(targetFd, EPOLLOUT);
}
