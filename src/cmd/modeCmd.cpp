#include "../../Irc.hpp"

bool validateModeFlag(string modeFlag, Channel* targetChannel)
{
	if ((modeFlag[0] == '+' && targetChannel->isFlagSet(modeFlag[1])) || (modeFlag[0] == '-' && !targetChannel->isFlagSet(modeFlag[1])))
		return true;
	return false;
}

void applyMode(std::istringstream &ss, Channel* targetChannel, Client* client, string modeFlag)
{
	char flag = modeFlag[1];

	switch (flag)
	{
		case 'i': // Invite only channel
			if (validateModeFlag(modeFlag, targetChannel))
				return;
			else
				targetChannel->apllyInviteOnlyFlag((modeFlag[0] == '+'));
			break;
		
		case 't': // Set/remove restriction to change the channel topic
			if (validateModeFlag(modeFlag, targetChannel))
				return;
			else
				targetChannel->apllyTopicRestrictionFlag((modeFlag[0] == '+'));
			break;
		
		case 'l': // Set/remove the user limit to channel
			if (targetChannel->apllyLimitRestrictionFlag(ss, modeFlag, client))
				return;			
			break;
		
		case 'k': // Set/remove passWord for channel
			if (targetChannel->apllyPasswordFlag(ss, modeFlag, client))
				return;
			break;
		
		case 'o': // Set/remove the operator privilege
			if (targetChannel->apllyOperatorPrivilegeFlag(ss, modeFlag, client))
				return;
			break;
		
		default:
			return sendMsg(client->getSock(), ERR_UNKNOWNMODE(client->getNick(), modeFlag));
	}
	targetChannel->sendAll(RPL_MODE(client->getNick(), client->getUser(), targetChannel->getChannelName(), modeFlag));
}

void Irc::modeCmd(istringstream &ss, Client* client)
{
	string channelName;
	string modeFlag;

	ss >> channelName;

	Channel* targetChannel;
	if (!(targetChannel = findChannel(channelName)))
		return sendMsg(client->getSock(), ERR_NOSUCHCHANNEL(client->getNick(), channelName));

	if (!targetChannel->isPartOfChannel(client->getNick()))
		return sendMsg(client->getSock(), ERR_NOTONCHANNEL(client->getNick(), channelName));
	
	if (ss >> modeFlag)
	{
		if (!targetChannel->isOperator(client->getNick()))
			return sendMsg(client->getSock(), ERR_CHANOPRIVSNEEDED(client->getNick(), channelName));
	
		if ((modeFlag[0] != '+' && modeFlag[0] != '-') || modeFlag.size() != 2)
			return sendMsg(client->getSock(), ERR_UNKNOWNMODE(client->getNick(), modeFlag));
		
		applyMode(ss, targetChannel, client, modeFlag);
	}
	else
		sendMsg(client->getSock(), RPL_CHANNELMODEIS(client->getNick(), channelName, targetChannel->getChannelModes()));

}
