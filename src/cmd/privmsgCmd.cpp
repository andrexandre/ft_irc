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


string retrieveContent(std::istringstream &ss)
{
	string content;
	ss >> content;
	bool	colon = (content[0] == ':') ? true : false;

	if (colon)
	{
		content.erase(0,1);
		string tmp;
		std::getline(ss, tmp);
		content += tmp;
	}
	else
		while (ss >> content);
	return (content);
}

//ver se e preciso enviar msg para ele proprio
void Irc::privmsgCmd(std::istringstream &ss, Client* actualClient)
{
	string targetName;
	string msg;
	string conntent;
	ss >> targetName;
	bool isChannel = (targetName[0] == '#') ? 1 : 0;


	if (!isChannel)
	{
		// ss >> conntent;
		conntent = retrieveContent(ss);
		Client* targetClient = findClient(targetName);

		if (targetClient)
		{
			msg += ":" + actualClient->getNick() + '!' + actualClient->getUser() + "@localhost PRIVMSG " + targetClient->getNick() + " :" + conntent + "\r\n";
			cout << msg << endl;
			send(targetClient->getSock(), msg.c_str(), msg.size(), 0);
		}
		else
		{
			msg += ERR_NOSUCHNICK(actualClient->getNick(), targetName);
			cout << msg << endl;
			//apagar os dois acima depois 
			return serverErrorMsg(actualClient->getSock(), ERR_NOSUCHNICK(actualClient->getNick(), targetName));
		}
	}
	else
	{
		// string tmp = ss.str();
		// conntent = tmp.substr((tmp.find(targetName) + targetName.size() + 1));

		conntent = retrieveContent(ss);
		Channel* tarChannel = findChannel(targetName);
		if (!tarChannel)
		{
			//mandar mesangem de erro
			return;
		}
		//send message to channel to all people in channel
		msg += ":" + actualClient->getNick() + '!' + actualClient->getUser() + "@localhost PRIVMSG " + tarChannel->getChannelName() + " :" + conntent + "\r\n";
		cout << msg << endl;
		return tarChannel->sendPrivMsg(actualClient->getSock(), msg);
	}

}




// ERR_NOSUCHNICK (401)
// ERR_NOSUCHSERVER (402)
// ERR_CANNOTSENDTOCHAN (404)
// ERR_TOOMANYTARGETS (407)
// ERR_NORECIPIENT (411)
// ERR_NOTEXTTOSEND (412)
// ERR_NOTOPLEVEL (413)
// ERR_WILDTOPLEVEL (414)
// RPL_AWAY (301)
