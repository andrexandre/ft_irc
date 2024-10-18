#include "../../Irc.hpp"

// :alex21!alex@9C5B1D.95C97E.C247D8.AE513.IP PRIVMSG loiky :bem?

Client* Irc::findClient(int target)
{
	std::map<int, Client*>::iterator it = _clients.find(target);
	return (it->second);
}

Client* Irc::findClient(string name)
{
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getNick() == name)
			return (it->second);
	}

	return (NULL);
}

void Irc::privmsgCmd(std::istringstream &ss, Client* actualClient)
{
	string targetName;
	string msg;
	string conn;
	ss >> targetName;
	bool isChannel = targetName[0] == '#' ? 1 : 0;


	if (!isChannel)
	{
		ss >> conn;

		Client* targetClient = findClient(targetName);
		if (targetClient)
		{
			msg += ":" + actualClient->getNick() + '!' + actualClient->getUser() + "@localhost PRIVMSG " + targetClient->getNick() + " :" + conn + "\r\n";
			cout << msg << endl;
			send(targetClient->getSock(), msg.c_str(), msg.size(), 0);
		}
		else
		{
			conn = " :No such nick";
			//>> :Aurora.AfterNET.Org 401 alex loiky7 :No such nick
			msg += ":localhost 401 " + actualClient->getNick() + ' ' + targetName  + conn + "\r\n";
			cout << msg << endl;
			send(actualClient->getSock(), msg.c_str(), msg.size(), 0);
			//client dont exist
		}

	}
	else
	{
		ss >> conn;
		Channel* tarChannel = findChannel(targetName);
		if (!tarChannel)
		{
			//mandar mesangem de erro
			return;
		}
		msg += ":" + actualClient->getNick() + '!' + actualClient->getUser() + "@localhost PRIVMSG " + tarChannel->getChannelName() + " :" + conn + "\r\n";

		tarChannel->sendMsg(actualClient->getSock(), msg);
		//send message to channel
	}

}