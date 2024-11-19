#include "../../Irc.hpp"

int ssLength(istringstream &ss)
{
    istringstream::pos_type position = ss.tellg();
	string temp;
	int count = 0;

	while (ss >> temp)
		count++;
	ss.clear();
	ss.seekg(position, std::ios::beg);
	return count;
}

void Irc::userCmd(istringstream &ss, Client* client)
{
	int length = ssLength(ss);
	string user;
	string str;
	
	if (length != 4)
		return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "USER"));
	if (client->getNick().empty())
		return sendMsg(client->getSock(), NOTICE_MSG("Empty nick, please set a nick first"));
	if (client->isAuthenticated())
		return sendMsg(client->getSock(), ERR_ALREADYREGISTRED(client->getNick()));
	ss >> user;
	ss >> str;
	if (!(str == "0" && ss >> str && str == "*" && ss >> str && str == ":realname"))
		sendMsg(client->getSock(), NOTICE_MSG("Unwanted input, username still changed"));
	if (client->getPassWord() != _serverPassWord)
	{
		sendMsg(client->getSock(), ERR_PASSWDMISMATCH(client->getNick()));
		return quitCmd(ss, client);
	}
	client->setUser(user);
	client->authenticate();
	sendMsg(client->getSock(), NOTICE_MSG("\x03" "02Welcome to the server!"));
}
