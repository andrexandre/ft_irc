#include "../../Irc.hpp"

void Irc::placeholder(istringstream &ss, Client* actualClient)
{
	string cmdName;
	istringstream sss(ss.str());
	sss >> cmdName;
	serverErrorMsg(actualClient->getSock(), ERR_SAMPLE("421", "Command not implemented", actualClient->getNick(), cmdName));
}

void Irc::nickCmd(std::istringstream &ss, Client* actualClient)
{
	string str;
	ss >> str;
	if (findClient(str))
		serverErrorMsg(actualClient->getSock(), ERR_NICKNAMEINUSE(actualClient->getNick(), str));
	else
	{
		if (str.empty())
			return serverErrorMsg(actualClient->getSock(), ERR_NONICKNAMEGIVEN(actualClient->getNick()));
		// verificar invalid characters (#, &, :)
		string oldNick = actualClient->getNick();
		actualClient->setNick(str);
		str = RPL(oldNick, actualClient->getUser(), "NICK", "", ":", actualClient->getNick());
		send(actualClient->getSock(), str.c_str(), str.size(), 0);
	}
}
