
#include "../../Irc.hpp"



Channel* Irc::createChannel(string name)
{
	Channel* newChannel = new Channel(name);
	_serverChannels.push_back(newChannel);
	return (newChannel);
}

Channel* Irc::findChannel(string name)
{
	std::vector<Channel*>::iterator it;
	for (it = _serverChannels.begin(); it != _serverChannels.end(); it++)
	{
		if ((*it)->getChannelName() == name)
			return (*it);
	}
	
	return (NULL);
}

void Irc::joinCmd(std::istringstream &ss, Client* actualClient)
{
	string channelName;
	string msg;
	ss >> channelName;

	if (channelName[0] != '#')
	{
		//>> :localhost 403 "actualClient nick" channelName :No such channel
		//mensagem de erro
		return;
	}

	// cout << channelName << endl;
	Channel* tarChannel = findChannel(channelName);
	if ((tarChannel = findChannel(channelName)))
	{
		// tarChannel->sendMsg(actualClient->getSock(), )
		// if (tarChannel->isPartOfChannel(actualClient->getNick()))
		// {
		// 	//ver se o user faz parte do channel
		// }
		//ver se o user ja foi bannido do channel

		msg += ':' + actualClient->getNick() + '!' + actualClient->getUser() + "@localhost JOIN " + channelName + " * :realname\r\n";
		tarChannel->setChannelUsers(false, actualClient);
		//enviar a mensagem
		tarChannel->sendAll(msg);
		return;
	}

	//Caso o canal nao exista ele cria o seu canal
	tarChannel = createChannel(channelName);
	tarChannel->setChannelUsers(true, actualClient);
	msg += ':' + actualClient->getNick() + '!' + actualClient->getUser() + "@localhost JOIN " + channelName + " * :realname\r\n";
	cout << msg << endl;
	tarChannel->sendAll(msg);
	// send(actualClient->getSock(), msg.c_str(), msg.size(), 0);

}


// ERR_NEEDMOREPARAMS (461)
// ERR_NOSUCHCHANNEL (403)
// ERR_TOOMANYCHANNELS (405)
// ERR_BADCHANNELKEY (475)
// ERR_BANNEDFROMCHAN (474)
// ERR_CHANNELISFULL (471)
// ERR_INVITEONLYCHAN (473)
// ERR_BADCHANMASK (476)
// RPL_TOPIC (332)
// RPL_TOPICWHOTIME (333)
// RPL_NAMREPLY (353)
// RPL_ENDOFNAMES (366)



