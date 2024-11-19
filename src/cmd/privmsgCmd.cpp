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

void Irc::privmsgCmd(istringstream &ss, Client* actualClient)
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
			return serverErrorMsg(actualClient->getSock(), ERR_NOSUCHNICK(actualClient->getNick(), targetName));

		conntent = retrieveMsg(ss);
		cout << RPL_PRIVMSG(actualClient->getNick(), actualClient->getUser(), targetClient->getNick(), conntent) << endl; //apagar depois
		sendMsg(targetClient->getSock(), RPL_PRIVMSG(actualClient->getNick(), actualClient->getUser(), targetClient->getNick(), conntent));
	}
	else
	{
		Channel* targetChannel;
		if (!(targetChannel = findChannel(targetName)))
			return serverErrorMsg(actualClient->getSock(), ERR_NOSUCHCHANNEL(actualClient->getNick(), targetName));

		if (!targetChannel->isPartOfChannel(actualClient->getNick()))
			return serverErrorMsg(actualClient->getSock(), ERR_NOTONCHANNEL(actualClient->getNick(), targetName));
		
		conntent = retrieveMsg(ss);
		cout << RPL_PRIVMSG(actualClient->getNick(), actualClient->getUser(), targetName, conntent) << endl; //apagar depois
		return targetChannel->sendPrivMsg(actualClient->getSock(), RPL_PRIVMSG(actualClient->getNick(), actualClient->getUser(), targetName, conntent));
	}
}


