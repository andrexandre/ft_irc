#include "../../Irc.hpp"

// << MODE #kk3 +i
// >> :andre!_Isto@9C5B1D.95C97E.C247D8.AE513.IP MODE #kk3 +

// Feito
//  i: Set/remove Invite-only channel
// t: Set/remove the restrictions of the TOPIC command to channel operators

// Não feito
// · k: Set/remove the channel key (password)
// · o: Give/take channel operator privilege
// . l: Set/remove the user limit to channel

//  Type B: Modes that change a setting on a channel. These modes MUST always have a parameter.
// Type C: Modes that change a setting on a channel. These modes MUST have a parameter when being set, and MUST NOT have a parameter when being unset.
// Type D: Modes that change a setting on a channel. These modes MUST NOT have a parameter.

bool validateModeFlag(string modeFlag, Channel* targetChannel)
{
	if ((modeFlag[0] == '+' && targetChannel->isFlagSet(modeFlag[1])) || (modeFlag[0] == '-' && !targetChannel->isFlagSet(modeFlag[1])))
		return true;
	return false;
}

void checkMode(istringstream &ss, Channel* targetChannel, Client* actualClient, string modeFlag)
{
	
	// << MODE #dd +l 9v9
	// >> :alex!alexsandro@B751E6:CB7426:C6EC48:482FF6:IP MODE #dd +l 9
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
		{
			// Agora falta fazer com que o operador tire o limite(-l)
			int nb;
			char* end;
			string number;
			stringstream converter;

			if (!(ss >> number))
			{
				// >> :localhost 461 alex MODE +l :Not enough parameters
				// >> :Aurora.AfterNET.Org 461 alex MODE +l :Not enough parameters
				return (serverErrorMsg(actualClient->getSock(), ERR_NEEDMOREPARAMS(actualClient->getNick(), string("MODE +l"))));

			}
			errno = 0;
			nb = strtol(number.c_str(), &end, 10);
			size_t tmp = nb;
			if (nb <= 0  || errno == ERANGE || tmp == targetChannel->getMaxUsersNumber())
				return;
			
			targetChannel->setMaxUsersNumber(nb);
			targetChannel->setChannelModes('l');
			converter << nb;
			modeFlag += " " + converter.str();

			// targetChannel->apllyTopicRestrictionFlag((modeFlag[0] == '+'));
			break;
		}
		
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
			
			// if ((modeFlag[0] == '+' && targetChannel->isFlagSet(modeFlag[1])) || (modeFlag[0] == '-' && !targetChannel->isFlagSet(modeFlag[1])))
			// 	return;

			checkMode(ss, targetChannel, actualClient, modeFlag);
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
