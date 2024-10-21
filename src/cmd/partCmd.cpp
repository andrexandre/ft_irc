#include "../../Irc.hpp"
//>> :andre!andre21@9C5B1D.95C97E.C247D8.AE513.IP PART #blaus :Leaving
//>> :andre!andre21@9C5B1D.95C97E.C247D8.AE513.IP PART #blaus :not happy
void Irc::partCmd(std::istringstream &ss, Client* actualClient)
{
	string msg;
	string channelName;
	string reasonToPart = " :Leaving";

	ss >> channelName;
	Channel* tarChannel = findChannel(channelName);
	if (tarChannel)
	{
		//ver se faz parte do channel
		if (!tarChannel->isPartOfChannel(actualClient->getNick()))
		{
			//erro
			return;
		}

		// mandar msg
		msg += ':' + actualClient->getNick() + '!' + actualClient->getUser() + "@localhost PART " + channelName + reasonToPart + "\r\n";
		tarChannel->sendAll(msg);
		tarChannel->removeClient(actualClient);
	}
	else
	{
		//erro prvavelmente
	}
}



// ERR_NEEDMOREPARAMS (461) 
// ERR_NOSUCHCHANNEL (403) // canal nao existe
// ERR_NOTONCHANNEL (442) // nao esta no canal
