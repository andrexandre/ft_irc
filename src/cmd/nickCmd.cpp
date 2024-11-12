#include "../../Irc.hpp"

void Irc::placeholder(istringstream &ss, Client* actualClient)
{
	string cmdName;
	istringstream sss(ss.str());

	sss >> cmdName;
	if (cmdName == "CAP")
		return; // capabilites are presumed to be the same as the server
	cout << "Unimplemented Command: " << cmdName << endl;
	if (actualClient->getNick().empty())
		serverErrorMsg(actualClient->getSock(), ERR_SAMPLE("0", "Command not implemented", cmdName));
	else
		serverErrorMsg(actualClient->getSock(), ERR_SAMPLE_2("0", "Command not implemented", actualClient->getNick(), cmdName));
}

void Irc::nickCmd(istringstream &ss, Client* actualClient)
{
	string str;
	ss >> str;

	if (findClient(str))
		serverErrorMsg(actualClient->getSock(), ERR_NICKNAMEINUSE(actualClient->getNick(), str));
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
