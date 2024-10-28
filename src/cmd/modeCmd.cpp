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
			// if ((modeFlag[0] != '+' && modeFlag[0] != '-') ||)
			// {
			// 	/*ERR_UNKNOWNMODE (472)*/
			// 	//:localhost 472 andre * :is unknown mode char to me
			// }
			// else
			// {

			// }
		}
		else{/*Eviar os modes atuais do canal*/}

	}
	else
	{
		// erro a dizer que nao e operador e que nao faz parte do canal
		return (serverErrorMsg(actualClient->getSock(), ERR_NOSUCHCHANNEL(actualClient->getNick(), channelName)));
	}

}
