#include "Irc.hpp"

Channel* Irc::createChannel(string name)
{
	Channel* newChannel = new Channel(name);
	_serverChannels.push_back(newChannel);
	return (newChannel);
}

Channel* Irc::findChannel(string name)
{
	vector<Channel*>::iterator it;
	for (it = _serverChannels.begin(); it != _serverChannels.end(); it++)
		if ((*it)->getChannelName() == name)
			return (*it);
	return (NULL);
}


Client* Irc::findClient(int target)
{
	map<int, Client*>::iterator it = _clients.find(target);
	return (it->second);
}

Client* Irc::findClient(string name)
{
	map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
		if (it->second->getNick() == name)
			return (it->second);
	return (NULL);
}

void Irc::deleteClient(map<int, Client*>::iterator& it)
{
	cout << YELLOW << "Closing connection, fd: " << it->first << END << endl;
	delete it->second;
	epfds->deleteFd(it->first);
}

void sendMsg(int fd, string msg)
{
	if (send(fd, msg.c_str(), msg.size(), 0) == -1)
		throw std::runtime_error("Cannot send the response");
}

int ssLength(istringstream &ss)
{
	istringstream::pos_type position = ss.tellg();
	string temp;
	int count = 0;

	while (ss >> temp)
		count++;
	ss.clear();
	ss.seekg(position, std::ios::beg);
	return count;
}
