#include "../../Irc.hpp"
//>> :andre!andre21@9C5B1D.95C97E.C247D8.AE513.IP PART #blaus :Leaving
//>> :andre!andre21@9C5B1D.95C97E.C247D8.AE513.IP PART #blaus :not happy
static void retrieveContent(string content, string& reason)
{
	size_t start = content.find(reason);
	string tmp = content.substr(start + 1);
	reason = tmp;
}
	
void Irc::partCmd(istringstream &ss, Client* actualClient)
{
	string msg;
	string channelName;
	string reasonToPart;

	ss >> channelName;
	Channel* tarChannel = findChannel(channelName);
	if (tarChannel && tarChannel->isPartOfChannel(actualClient->getNick()))
	{
		if (ss >> reasonToPart)
			retrieveContent(ss.str(), reasonToPart);
		
		cout << endl << RPL_PART(actualClient->getNick(), actualClient->getUser(), channelName, reasonToPart) << endl;
		tarChannel->sendAll(RPL_PART(actualClient->getNick(), actualClient->getUser(), channelName, reasonToPart));
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
