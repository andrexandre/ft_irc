#include "../../Irc.hpp"

void Irc::passCmd(istringstream &ss, Client* client)
{
	string str;
	
	if (!(ss >> str))
		return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "PASS"));
	if (client->isAuthenticated())
		return sendMsg(client->getSock(), ERR_ALREADYREGISTRED(client->getNick()));
	client->setPassWord(str);
}
