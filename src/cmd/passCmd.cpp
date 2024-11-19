#include "../../Irc.hpp"

void Irc::passCmd(istringstream &ss, Client* actualClient)
{
	string str;
	
	if (!(ss >> str))
		return serverErrorMsg(actualClient->getSock(), ERR_NEEDMOREPARAMS(actualClient->getNick(), "PASS"));
	if (actualClient->isAuthenticated())
		return serverErrorMsg(actualClient->getSock(), ERR_ALREADYREGISTRED(actualClient->getNick()));
	actualClient->setPassWord(str);
}
