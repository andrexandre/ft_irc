#include "../../Irc.hpp"
//>> :andre!andre21@9C5B1D.95C97E.C247D8.AE513.IP PART #blaus :Leaving
//>> :andre!andre21@9C5B1D.95C97E.C247D8.AE513.IP PART #blaus :not happy
static void retrieveContent(std::istringstream &ss, string& reason)
{
	string content = ss.str();
	size_t idx = content.find(reason) + reason.size();
	reason += content.erase(0,idx);
}
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
			// resolver depois
			retrieveContent(ss ,reasonToPart);
		}
		// msg += ':' + actualClient->getNick() + '!' + actualClient->getUser() + "@localhost JOIN " + channelName + " * :realname\r\n";
		// msg += ':' + actualClient->getNick() + '!' + actualClient->getUser() + "@localhost TOPIC " + channelName + ' ' + tarChannel->getChannelTopic() + "\r\n";
		// msg += ':' + actualClient->getNick() + '!' + actualClient->getUser() + "@localhost PART " + channelName + ' ' + reasonToPart + "\r\n";
		// msg += ":" + actualClient->getNick() + '!' + actualClient->getUser() + "@localhost PRIVMSG " + targetClient->getNick() + " :" + conntent + "\r\n";
		// msg += ':' + actualClient->getNick() + '!' + actualClient->getUser() + "@localhost JOIN " + channelName + " * :realname\r\n";
		msg += ':' + actualClient->getNick() + '!' + actualClient->getUser() + "@localhost PART " + channelName + ' ' + reasonToPart + "\r\n";
		cout << endl << RPL_PART(actualClient->getNick(), actualClient->getUser(), channelName, reasonToPart) << endl;
		cout << "REASON before send: " << reasonToPart << endl;
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
