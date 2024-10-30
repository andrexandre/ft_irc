#include "../../Irc.hpp"


static string retrieveContent(std::istringstream &ss)
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
void Irc::privmsgCmd(std::istringstream &ss, Client* actualClient)
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

		conntent = retrieveContent(ss);
		Channel* tarChannel = findChannel(targetName);
		if (!tarChannel)
			return serverErrorMsg(actualClient->getSock(), ERR_NOSUCHCHANNEL(actualClient->getNick(), targetName));

		//send message to channel to all people in channel
		cout << RPL_PRIVMSG(actualClient->getNick(), actualClient->getUser(), targetName, conntent) << endl;
		return tarChannel->sendPrivMsg(actualClient->getSock(), RPL_PRIVMSG(actualClient->getNick(), actualClient->getUser(), targetName, conntent));
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
