#include "Irc.hpp"

Channel* Irc::createChannel(string name)
{
	Channel* newChannel = new Channel(name);
	_serverChannels.push_back(newChannel);
	return (newChannel);
}

Channel* Irc::findChannel(string name)
{
	std::vector<Channel*>::iterator it;
	for (it = _serverChannels.begin(); it != _serverChannels.end(); it++)
	{
		if ((*it)->getChannelName() == name)
			return (*it);
	}
	
	return (NULL);
}


Client* Irc::findClient(int target)
{
	std::map<int, Client*>::iterator it = _clients.find(target);
	return (it->second);
}

Client* Irc::findClient(string name)
{
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getNick() == name)
			return (it->second);
	}

	return (NULL);
}

void Irc::deleteClient(std::map<int, Client*>::iterator& it)
{
	(void)it;
	// std::map<int, Client*>::iterator tmp = it;
	// ++it;
	// epfds->deleteFd(tmp->first);
	// delete tmp->second;
	// _clients.erase(tmp);
}
