#include "Channel.hpp"

Channel::Channel(string name) : _channelName(name), _channelModes("+"), _maxUsersNumber(0) {}


Channel::~Channel(void) {}


void Channel::setChannelUsers(bool oprt, Client* ptr) 
{
	vector<string>::iterator it = std::find(_inviteUsers.begin(), _inviteUsers.end(), ptr->getNick());
	if (it != _inviteUsers.end())
		_inviteUsers.erase(it);
	
	_channelUsers.insert(std::make_pair(ptr,oprt));
}

void Channel::setChannelTopic(string content) {
	_channelTopic = content;
}

void Channel::setChannelPassword(string content) {
	_channelPassword = content;
}

void Channel::setMaxUsersNumber(size_t nb) {
	_maxUsersNumber = nb;
}


string Channel::getChannelName(void) const {
	return (_channelName);
}

size_t Channel::getMaxUsersNumber(void) const {
	return (_maxUsersNumber);
}

string Channel::getChannelTopic(void) const {
	return (_channelTopic);
}

string Channel::getChannelPassword(void) const {
	return (_channelPassword);
}

size_t Channel::getNumberOfUsersOnChannel(void) const {
	return (_channelUsers.size());
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

