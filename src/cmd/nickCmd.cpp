#include "../../Irc.hpp"

// msg += ':' + actualClient->getNick() + '!' + actualClient->getUser() + "@localhost TOPIC " + channelName + ' ' + tarChannel->getChannelTopic() + "\r\n";

void Irc::nickCmd(std::istringstream &ss, Client* actualClient)
{
	string str;
	ss >> str;
	if (findClient(str))
		serverErrorMsg(actualClient->getSock(), ERR_NICKNAMEINUSE(actualClient->getNick(), str));
	else
	{
		if (str.empty())
			return; // error empty nick
		if (actualClient->getNick().empty())
			actualClient->setNick(str);
		string oldNick = actualClient->getNick();
		actualClient->setNick(str);
		str = ':' + oldNick + '!' + actualClient->getUser() + "@localhost NICK :" + actualClient->getNick() + "\r\n";
		send(actualClient->getSock(), str.c_str(), str.size(), 0);
		// :velhoNick!analexan@localhost NICK :novoNick
	}
}
