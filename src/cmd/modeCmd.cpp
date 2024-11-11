#include "../../Irc.hpp"

bool validateModeFlag(string modeFlag, Channel* targetChannel)
{
	if ((modeFlag[0] == '+' && targetChannel->isFlagSet(modeFlag[1])) || (modeFlag[0] == '-' && !targetChannel->isFlagSet(modeFlag[1])))
		return true;
	return false;
}

void applyMode(std::istringstream &ss, Channel* targetChannel, Client* actualClient, string modeFlag)
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
			if (targetChannel->apllyLimitRestrictionFlag(ss, modeFlag, actualClient))
				return;			
			break;
		
		case 'k': // Set/remove passWord for channel
			if (targetChannel->apllyPasswordFlag(ss, modeFlag, actualClient))
				return;
			break;
		
		case 'o': // Set/remove the operator privilege
			if (targetChannel->apllyOperatorPrivilegeFlag(ss, modeFlag, actualClient))
				return;
			break;
		
		default:
			return (serverErrorMsg(actualClient->getSock(), ERR_UNKNOWNMODE(actualClient->getNick(), modeFlag)));
	}
	targetChannel->sendAll(RPL_MODE(actualClient->getNick(), actualClient->getUser(), targetChannel->getChannelName(), modeFlag));
}

void Irc::modeCmd(istringstream &ss, Client* actualClient)
{
	string channelName;
	string modeFlag;

	ss >> channelName;

	Channel* targetChannel;
	if (!(targetChannel = findChannel(channelName)))
		return (serverErrorMsg(actualClient->getSock(), ERR_NOSUCHCHANNEL(actualClient->getNick(), channelName)));

	if (!targetChannel->isPartOfChannel(actualClient->getNick()))
		return (serverErrorMsg(actualClient->getSock(), ERR_NOTONCHANNEL(actualClient->getNick(), channelName)));
	
	if (ss >> modeFlag)
	{
		if (!targetChannel->isOperator(actualClient->getNick()))
			return (serverErrorMsg(actualClient->getSock(), ERR_CHANOPRIVSNEEDED(actualClient->getNick(), channelName)));
	
		if ((modeFlag[0] != '+' && modeFlag[0] != '-') || modeFlag.size() != 2)
			return (serverErrorMsg(actualClient->getSock(), ERR_UNKNOWNMODE(actualClient->getNick(), modeFlag)));
		
		applyMode(ss, targetChannel, actualClient, modeFlag);
	}
	else
		sendMsg(actualClient->getSock(), RPL_CHANNELMODEIS(actualClient->getNick(), channelName, targetChannel->getChannelModes()));
	//>> :loCALHOST 324 alex #ww +tn 

}
