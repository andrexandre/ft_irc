#include "Irc.hpp"

void Irc::apllyInviteOnlyFlag(bool optr, Channel* targetChannel)
{
	//ver se tenho que colocar os que ja estao no canal no users logo de inicio
	if (optr)
		targetChannel->setChannelModes('i');
	else
		targetChannel->removeChannelModesFlag('i');
}

void Irc::apllyTopicRestrictionFlag(bool optr, Channel* targetChannel)
{
	if (optr)
		targetChannel->setChannelModes('t');
	else
		targetChannel->removeChannelModesFlag('t');
}

bool Irc::apllyPasswordFlag(istringstream& ss, string& modeFlag, Client* client, Channel* targetChannel)
{
	string pass;
	bool operation = (modeFlag[0] == '-');

	if (!(ss >> pass))
		return (sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), string("MODE +k"))), 1);
	
	if (operation)
	{
		if (!targetChannel->isFlagSet('k'))
			return 1;
		
		if (pass != targetChannel->getChannelPassword())
			return (sendMsg(client->getSock(), ERR_KEYSET(client->getNick())), 1);
		
		targetChannel->removeChannelModesFlag('k');
		targetChannel->setChannelPassword("");
	}
	else
	{
		if (!targetChannel->getChannelPassword().empty())
			return (sendMsg(client->getSock(), ERR_KEYSET(client->getNick())), 1);
		
		targetChannel->setChannelPassword(pass);
		targetChannel->setChannelModes('k');
	}

	modeFlag += " " + pass;
	return 0;
}

bool Irc::apllyLimitRestrictionFlag(istringstream& ss, string& modeFlag, Client* client, Channel* targetChannel)
{
	if (modeFlag[0] == '-')
		return ((targetChannel->isFlagSet('l')) ? (targetChannel->removeChannelModesFlag('l'), 0) : 1);	
	
	int nb;
	char* end;
	string number;
	stringstream converter;

	if (!(ss >> number))
		return (sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), string("MODE +l"))), 1);

	errno = 0;
	nb = strtol(number.c_str(), &end, 10);
	size_t tmp = nb;
	if (nb <= 0 || errno == ERANGE || tmp == targetChannel->getMaxUsersNumber())
		return 1;
	
	targetChannel->setMaxUsersNumber(nb);
	if (!targetChannel->isFlagSet('l'))
		targetChannel->setChannelModes('l');
	converter << nb;
	modeFlag += " " + converter.str();
	
	return 0;
}

bool Irc::apllyOperatorPrivilegeFlag(istringstream& ss, string& modeFlag, Client* client, Channel* targetChannel)
{
	string targetNick;
	bool operation = (modeFlag[0] == '-');

	if (!(ss >> targetNick))
		return 1;
	
	if (!findClient(targetNick))
		return (sendMsg(client->getSock(), ERR_NOSUCHNICK(client->getNick(), targetNick)), 1);

	if (!targetChannel->isPartOfChannel(targetNick))
		return (sendMsg(client->getSock(), ERR_USERNOTINCHANNEL(client->getNick(), targetNick, targetChannel->getChannelName())), 1);
	
	if (operation)
	{
		if (!targetChannel->isOperator(targetNick))
			return 1;
		else
			targetChannel->giveOrTakeOperatorPrivilege(targetNick);		
	}
	else
	{
		if (targetChannel->isOperator(targetNick))
			return 1;
		else
			targetChannel->giveOrTakeOperatorPrivilege(targetNick, true);
	}

	modeFlag += " " + targetNick;
	return 0;
}
