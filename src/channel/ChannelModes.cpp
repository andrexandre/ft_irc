#include "Channel.hpp"

bool Channel::isFlagSet(char flag) const {
	return((_channelModes.find(flag) != string::npos) ? 1 : 0);
}

bool Channel::isChannelFull(void) const {
	return((getNumberOfUsersOnChannel() >= getMaxUsersNumber()) ? 1 : 0);
}

bool Channel::isUserInvited(string nick) const {
	return ((std::find(_inviteUsers.begin(), _inviteUsers.end(), nick) != _inviteUsers.end()) ? 1 : 0);
}


void Channel::setChannelModes(char flag) {
	_channelModes.push_back(flag);
}

void Channel::removeChannelModesFlag(char flag) {
	_channelModes.erase(_channelModes.find(flag), 1);
}

void Channel::setInviteUsers(string nick) 
{
	// Para não colocar o nome duas vezes na lista
	if (!_inviteUsers.size() || std::find(_inviteUsers.begin(), _inviteUsers.end(), nick) != _inviteUsers.end())
		_inviteUsers.push_back(nick);
}


void Channel::apllyTopicRestrictionFlag(bool optr)
{
	if (optr)
		setChannelModes('t');
	else
		removeChannelModesFlag('t');
}

void Channel::apllyInviteOnlyFlag(bool optr)
{
	//ver se tenho que colocar os que ja estao no canal no users logo de inicio
	if (optr)
		setChannelModes('i');
	else
		removeChannelModesFlag('i');
}

bool Channel::apllyLimitRestrictionFlag(istringstream& ss, string& modeFlag, bool optr, Client* client)
{
	if (!optr)
		return ((isFlagSet('l')) ? (removeChannelModesFlag('l'), 0) : 1);	
	
	int nb;
	char* end;
	string number;
	stringstream converter;

	if (!(ss >> number))
	{
		// >> :localhost 461 alex MODE +l :Not enough parameters
		// >> :Aurora.AfterNET.Org 461 alex MODE +l :Not enough parameters
		return (serverErrorMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), string("MODE +l"))), 1);
	}

	errno = 0;
	nb = strtol(number.c_str(), &end, 10);
	size_t tmp = nb;
	if (nb <= 0  || errno == ERANGE || tmp == getMaxUsersNumber())
		return 1;
	
	setMaxUsersNumber(nb);
	setChannelModes('l');
	converter << nb;
	modeFlag += " " + converter.str();
	
	return 0;
}


