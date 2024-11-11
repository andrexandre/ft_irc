#include "../../Irc.hpp"


static string retrieveContent(istringstream &ss)
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

//ver se e preciso enviar msg para ele proprio
void Irc::privmsgCmd(istringstream &ss, Client* actualClient)
{
	string targetName;
	string msg;
	string conntent;
	ss >> targetName;
	bool isChannel = (targetName[0] == '#') ? 1 : 0;

	if (!isChannel)
	{
		conntent = retrieveContent(ss);
		Client* targetClient = findClient(targetName);

		if (targetClient)
		{	
			cout << RPL_PRIVMSG(actualClient->getNick(), actualClient->getUser(), targetClient->getNick(), conntent) << endl;
			sendMsg(targetClient->getSock(), RPL_PRIVMSG(actualClient->getNick(), actualClient->getUser(), targetClient->getNick(), conntent));
		}
		else
			return serverErrorMsg(actualClient->getSock(), ERR_NOSUCHNICK(actualClient->getNick(), targetName));
	}
	else
	{
		Channel* targetChannel = findChannel(targetName);
		if (!targetChannel)
			return serverErrorMsg(actualClient->getSock(), ERR_NOSUCHCHANNEL(actualClient->getNick(), targetName));

		if (!targetChannel->isPartOfChannel(actualClient->getNick()))
			return serverErrorMsg(actualClient->getSock(), ERR_NOTONCHANNEL(actualClient->getNick(), targetName));
		
		conntent = retrieveContent(ss);
		cout << RPL_PRIVMSG(actualClient->getNick(), actualClient->getUser(), targetName, conntent) << endl;
		return targetChannel->sendPrivMsg(actualClient->getSock(), RPL_PRIVMSG(actualClient->getNick(), actualClient->getUser(), targetName, conntent));
	}
}

// ERR_CANNOTSENDTOCHAN (404)
