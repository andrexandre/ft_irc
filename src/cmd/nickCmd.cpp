#include "../../Irc.hpp"

void Irc::nickCmd(istringstream &ss, Client* client)
{
	string str;
	ss >> str;

	if (findClient(str))
		sendMsg(client->getSock(), ERR_NICKNAMEINUSE(str));
	else
	{
		if (str.empty())
			return sendMsg(client->getSock(), ERR_NONICKNAMEGIVEN("*"));
		if (str.find_first_of(" ,*?!@.#&:$") != string::npos)
			return sendMsg(client->getSock(), ERR_ERRONEUSNICKNAME("*", str));
		
		string oldNick = client->getNick();
		client->setNick(str);
		str = RPL(oldNick, client->getUser(), "NICK", "", ":", client->getNick());
		send(client->getSock(), str.c_str(), str.size(), 0);
	}
}
