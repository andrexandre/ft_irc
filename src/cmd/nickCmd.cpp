#include "../../Irc.hpp"

void Irc::nickCmd(istringstream &ss, Client* actualClient)
{
	string str;
	ss >> str;

	if (findClient(str))
		serverErrorMsg(actualClient->getSock(), ERR_NICKNAMEINUSE(str));
	else
	{
		if (str.empty())
			return serverErrorMsg(actualClient->getSock(), ERR_NONICKNAMEGIVEN(actualClient->getNick()));
		if (str.find_first_of(" ,*?!@.#&:$") != string::npos)
			return serverErrorMsg(actualClient->getSock(), ERR_ERRONEUSNICKNAME(actualClient->getNick(), str));
		
		string oldNick = actualClient->getNick();
		actualClient->setNick(str);
		str = RPL(oldNick, actualClient->getUser(), "NICK", "", ":", actualClient->getNick());
		send(actualClient->getSock(), str.c_str(), str.size(), 0);
	}
}
