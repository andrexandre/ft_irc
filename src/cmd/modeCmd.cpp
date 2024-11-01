#include "../../Irc.hpp"

// << MODE #kk3 +i
// >> :andre!_Isto@9C5B1D.95C97E.C247D8.AE513.IP MODE #kk3 +

//  i: Set/remove Invite-only channel
// · t: Set/remove the restrictions of the TOPIC command to channel
// operators
// · k: Set/remove the channel key (password)
// · o: Give/take channel operator privilege
// l: Set/remove the user limit to channel

//  Type B: Modes that change a setting on a channel. These modes MUST always have a parameter.
// Type C: Modes that change a setting on a channel. These modes MUST have a parameter when being set, and MUST NOT have a parameter when being unset.
// Type D: Modes that change a setting on a channel. These modes MUST NOT have a parameter.

void Channel::removeChannelModesFlag(char flag) {
	_channelModes.erase(_channelModes.find(flag), 1);
}

bool Channel::isFlagSet(char flag) {
	return((_channelModes.find(flag) != string::npos) ? 1 : 0);
}

void Channel::apllyInviteOnlyFlag(bool optr)
{
	if (optr)
	{
		setChannelModes('i');
		//ver se tenho que colocar os que ja estao no canal no users logo de inicio
	}
	else
	{
		cout << "aqui\n";
		removeChannelModesFlag('i');
	}
}

// >> :luna.AfterNET.Org 473 andre #eer :Cannot join channel (+i)  entar em um canal invite only
void checkMode(Channel* targetChannel, Client* actualClient,string modeFlag)
{
	char flag = modeFlag[1];

	switch (flag)
	{
		case 'i':
			targetChannel->apllyInviteOnlyFlag((modeFlag[0] == '+'));
			break;
		
		default:
			return (serverErrorMsg(actualClient->getSock(), ERR_UNKNOWNMODE(actualClient->getNick(), modeFlag)));
	}
	sendMsg(actualClient->getSock(), RPL_MODE(actualClient->getNick(), actualClient->getUser(), targetChannel->getChannelName(), modeFlag));
}

void Irc::modeCmd(std::istringstream &ss, Client* actualClient)
{
	string channelName;
	string modeFlag;

	ss >> channelName;

	Channel* targetChannel;
	if ((targetChannel = findChannel(channelName)) && targetChannel->isPartOfChannel(actualClient->getNick()))
	{
		if (ss >> modeFlag)
		{
			if ((modeFlag[0] != '+' && modeFlag[0] != '-') || modeFlag.size() != 2)
			{
				return (serverErrorMsg(actualClient->getSock(), ERR_UNKNOWNMODE(actualClient->getNick(), modeFlag)));
				/*ERR_UNKNOWNMODE (472)*/
				//:localhost 472 andre * :is unknown mode char to me
			}
			
			if ((modeFlag[0] == '+' && targetChannel->isFlagSet(modeFlag[1])) || (modeFlag[0] == '-' && !targetChannel->isFlagSet(modeFlag[1])))
				return;

			checkMode(targetChannel, actualClient, modeFlag);
			//:andre!alex21@9C5B1D.95C97E.C247D8.AE513.IP MODE #ggt +i
			//se aquele mode que foi escolhido ja existir ele nao diz nada

		}
		else{/*Eviar os modes atuais do canal*/}

	}
	else
	{
		// erro a dizer que nao e operador e que nao faz parte do canal
		return (serverErrorMsg(actualClient->getSock(), ERR_NOSUCHCHANNEL(actualClient->getNick(), channelName)));
	}

}
