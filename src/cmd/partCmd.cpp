#include "../../Irc.hpp"
//>> :andre!andre21@9C5B1D.95C97E.C247D8.AE513.IP PART #blaus :Leaving
//>> :andre!andre21@9C5B1D.95C97E.C247D8.AE513.IP PART #blaus :not happy
void Irc::partCmd(std::istringstream &ss, Client* actualClient)
{
	string msg;
	string channelName;
	string reasonToPart;

	ss >> channelName;
	Channel* tarChannel = findChannel(channelName);
	if (tarChannel && tarChannel->isPartOfChannel(actualClient->getNick()))
	{
		if (ss >> reasonToPart)
		{
			string tmp;
			std::getline(ss, tmp);
			reasonToPart += tmp;
		}

		msg += ':' + actualClient->getNick() + '!' + actualClient->getUser() + "@localhost PART " + channelName + " " + reasonToPart + "\r\n";
		tarChannel->sendAll(msg);
		tarChannel->removeClient(actualClient);
	}
	else
	{
		if (!tarChannel)
			serverErrorMsg(actualClient->getSock(), ERR_NOSUCHCHANNEL(actualClient->getNick(), channelName));
		else
			serverErrorMsg(actualClient->getSock(), ERR_NOTONCHANNEL(actualClient->getNick(), channelName));
	}
}

// ERR_NEEDMOREPARAMS (461) 
