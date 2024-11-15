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
	if (_inviteUsers.size() == 0 || std::find(_inviteUsers.begin(), _inviteUsers.end(), nick) == _inviteUsers.end())
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

bool Channel::apllyLimitRestrictionFlag(istringstream& ss, string& modeFlag, Client* client)
{
	if (modeFlag[0] == '-')
		return ((isFlagSet('l')) ? (removeChannelModesFlag('l'), 0) : 1);	
	
	int nb;
	char* end;
	string number;
	stringstream converter;

	if (!(ss >> number))
		return (serverErrorMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), string("MODE +l"))), 1);

	errno = 0;
	nb = strtol(number.c_str(), &end, 10);
	size_t tmp = nb;
	if (nb <= 0  || errno == ERANGE || tmp == getMaxUsersNumber())
		return 1;
	
	setMaxUsersNumber(nb);
	if (!isFlagSet('l'))
		setChannelModes('l');
	converter << nb;
	modeFlag += " " + converter.str();
	
	return 0;
}

bool Channel::apllyPasswordFlag(istringstream& ss, string& modeFlag, Client* client)
{
	string pass;
	bool operation = (modeFlag[0] == '-');

	if (!(ss >> pass))
		return (serverErrorMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), string("MODE +k"))), 1);
	
	if (operation)
	{
		if (!isFlagSet('k'))
			return 1;
		
		if (pass != _channelPassword)
			return (serverErrorMsg(client->getSock(), ERR_KEYSET(client->getNick())), 1);
		
		removeChannelModesFlag('k');
		setChannelPassword("");
	}
	else
	{
		if (!_channelPassword.empty())
			return (serverErrorMsg(client->getSock(), ERR_KEYSET(client->getNick())), 1);
		
		setChannelPassword(pass);
		setChannelModes('k');
	}

	modeFlag += " " + pass;
	return 0;
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

bool Channel::apllyOperatorPrivilegeFlag(istringstream& ss, string& modeFlag, Client* client)
{
	// << MODE #aa +o klkl
	// >> :luna.AfterNET.Org 401 alex klkl :No such nick
	string targetNick;
	bool operation = (modeFlag[0] == '-');

	if (!(ss >> targetNick))
		return 1;
	
	//ver se o nick existe no servidor

	if (!isPartOfChannel(targetNick))
		return (serverErrorMsg(client->getSock(), ERR_USERNOTINCHANNEL(client->getNick(), targetNick, _channelName)), 1);
	
	if (operation)
	{
		if (!isOperator(targetNick))
			return 1;
		else
			giveOrTakeOperatorPrivilege(targetNick);		
	}
	else
	{
		if (isOperator(targetNick))
			return 1;
		else
			giveOrTakeOperatorPrivilege(targetNick, true);
	}

	modeFlag += " " + targetNick;
	return 0;
}
