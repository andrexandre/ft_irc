#include "Channel.hpp"

Channel::Channel(string name) : _usersNumber(0), _channelName(name), _channelModes("+") {}


Channel::~Channel(void) {}


string Channel::getChannelName(void) const {
	return (_channelName);
}


void Channel::setChannelUsers(bool oprt, Client* ptr) 
{
	vector<string>::iterator it = std::find(_inviteUsers.begin(), _inviteUsers.end(), ptr->getNick());
	if (it != _inviteUsers.end())
		_inviteUsers.erase(it);
	
	_channelUsers.insert(std::make_pair(ptr,oprt));
	_usersNumber++;
}

void Channel::setChannelTopic(string content) {
	_channelTopic = content;
}


size_t Channel::getUsersNumber(void) const {
	return (_usersNumber);
}

string Channel::getChannelTopic(void) const {
	return (_channelTopic);
}


void Channel::removeClient(Client* ptr)
{
	std::map<Client*, bool>::iterator it;
	for (it = _channelUsers.begin(); it != _channelUsers.end(); it++)
	{
		if (it->first == ptr)
			break;
	}
	_channelUsers.erase(it);
	_usersNumber--;
}

bool Channel::isPartOfChannel(string nick) const
{
	std::map<Client*, bool>::const_iterator it;
	for (it = _channelUsers.begin(); it != _channelUsers.end(); it++)
	{
		if (it->first->getNick() == nick)
			return (true);
	}

	return (false);
}

bool Channel::isOperator(string nick) const
{
	std::map<Client*, bool>::const_iterator it;
	for (it = _channelUsers.begin(); it != _channelUsers.end(); it++)
	{
		if (it->first->getNick() == nick)
			return ((it->second) ? true : false);
	}

	return (false);
}


void Channel::sendPrivMsg(int fd, string msg) const 
{
	std::map<Client*, bool>::const_iterator it;
	for (it = _channelUsers.begin(); it != _channelUsers.end(); it++)
	{
		if (it->first->getSock() != fd)
		{
			if (send(it->first->getSock(), msg.c_str(), msg.size(), 0) == -1)
				throw std::runtime_error("Cannot send response");
		}
	}
}

void Channel::sendAll(string msg) const 
{
	std::map<Client*, bool>::const_iterator it;
	for (it = _channelUsers.begin(); it != _channelUsers.end(); it++)
	{
		if (send(it->first->getSock(), msg.c_str(), msg.size(), 0) == -1)
			throw std::runtime_error("Cannot send response");
	}
}

