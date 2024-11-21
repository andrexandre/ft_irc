#include "Channel.hpp"

Channel::Channel(string name) : _channelName(name), _channelModes("+t"), _maxUsersNumber(0) {}

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

void Channel::setInviteUsers(string nick) 
{
	// Para não colocar o nome duas vezes na lista
	if (_inviteUsers.size() == 0 || std::find(_inviteUsers.begin(), _inviteUsers.end(), nick) == _inviteUsers.end())
		_inviteUsers.push_back(nick);
}

void Channel::setChannelModes(char flag) {
	_channelModes.push_back(flag);
}



string Channel::getChannelName(void) const {
	return (_channelName);
}

string Channel::getChannelTopic(void) const {
	return (_channelTopic);
}

string Channel::getChannelModes(void) const {
	return (_channelModes);
}

size_t Channel::getMaxUsersNumber(void) const {
	return (_maxUsersNumber);
}

string Channel::getChannelPassword(void) const {
	return (_channelPassword);
}

size_t Channel::getNumberOfUsersOnChannel(void) const {
	return (_channelUsers.size());
}


void Channel::removeClient(Client* ptr)
{
	map<Client*, bool>::iterator it;
	for (it = _channelUsers.begin(); it != _channelUsers.end(); it++)
	{
		if (it->first == ptr)
			break;
	}
	_channelUsers.erase(it);
}

void Channel::removeChannelModesFlag(char flag) {
	_channelModes.erase(_channelModes.find(flag), 1);
}


bool Channel::isPartOfChannel(string nick) const
{
	map<Client*, bool>::const_iterator it;
	for (it = _channelUsers.begin(); it != _channelUsers.end(); it++)
	{
		if (it->first->getNick() == nick)
			return (true);
	}

	return (false);
}

bool Channel::isFlagSet(char flag) const {
	return((_channelModes.find(flag) != string::npos) ? 1 : 0);
}

bool Channel::isChannelFull(void) const {
	return((getNumberOfUsersOnChannel() >= getMaxUsersNumber()) ? 1 : 0);
}

bool Channel::isOperator(string nick) const
{
	map<Client*, bool>::const_iterator it;
	for (it = _channelUsers.begin(); it != _channelUsers.end(); it++)
	{
		if (it->first->getNick() == nick)
			return ((it->second) ? true : false);
	}

	return (false);
}

bool Channel::isUserInvited(string nick) const {
	return ((std::find(_inviteUsers.begin(), _inviteUsers.end(), nick) != _inviteUsers.end()) ? 1 : 0);
}


void Channel::sendPrivMsg(int fd, string msg) const 
{
	map<Client*, bool>::const_iterator it;
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
	map<Client*, bool>::const_iterator it;
	cout << WHITE << msg << END << endl;
	for (it = _channelUsers.begin(); it != _channelUsers.end(); it++)
	{
		if (send(it->first->getSock(), msg.c_str(), msg.size(), 0) == -1)
			throw std::runtime_error("Cannot send response");
	}
}


void Channel::giveOrTakeOperatorPrivilege(string targetNick, bool privilege)
{
	std::map<Client*, bool>::iterator it;
	for (it = _channelUsers.begin(); it != _channelUsers.end(); it++)
	{
		if (it->first->getNick() == targetNick)
			break;
	}
	it->second = privilege;
}