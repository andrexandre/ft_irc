#include "../../Irc.hpp"

void Irc::userCmd(std::istringstream &ss, Client* actualClient)
{
	string str;
	ss >> str;
	// if (str == "0")
	// 	return serverErrorMsg(actualClient->getSock(), ERR_NEEDMOREPARAMS(actualClient->getNick()));
	actualClient->setUser(str);
}
