#include "../../Irc.hpp"


static string retrieveMsg(istringstream &ss)
{
	string content;
	ss >> content;
	bool	colon = (content[0] == ':') ? true : false;

	if (colon)
	{
		content.erase(content.begin());
		string tmp;
		std::getline(ss, tmp);
		content += tmp;
	}
	else
		while (ss >> content);
	return (content);
}

void Irc::privmsgCmd(istringstream &ss, Client* client)
{
	string msg;
	string conntent;
	string targetName;
	ss >> targetName;
	bool isChannel = (targetName[0] == '#') ? 1 : 0;

	if (!isChannel)
	{
		Client* targetClient;
		if (!(targetClient = findClient(targetName)))
			return sendMsg(client->getSock(), ERR_NOSUCHNICK(client->getNick(), targetName));

		conntent = retrieveMsg(ss);
		cout << "Send to client fd: " << client->getSock() << endl;
		cout << WHITE << RPL_PRIVMSG(client->getNick(), client->getUser(), targetClient->getNick(), conntent) << END << endl;
		sendMsg(targetClient->getSock(), RPL_PRIVMSG(client->getNick(), client->getUser(), targetClient->getNick(), conntent));
	}
	else
	{
		Channel* targetChannel;
		if (!(targetChannel = findChannel(targetName)))
			return sendMsg(client->getSock(), ERR_NOSUCHCHANNEL(client->getNick(), targetName));

		if (!targetChannel->isPartOfChannel(client->getNick()))
			return sendMsg(client->getSock(), ERR_NOTONCHANNEL(client->getNick(), targetName));
		
		conntent = retrieveMsg(ss);
		cout << "Send to client fd: " << client->getSock() << endl;
		cout << WHITE << RPL_PRIVMSG(client->getNick(), client->getUser(), targetName, conntent) << END << endl;
		return targetChannel->sendPrivMsg(client->getSock(), RPL_PRIVMSG(client->getNick(), client->getUser(), targetName, conntent));
	}
}


