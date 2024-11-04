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

void Irc::userCmd(istringstream &ss, Client* actualClient)
{
	int length = ssLength(ss);
	string user;
	string str;
	
	if (length != 4)
		return serverErrorMsg(actualClient->getSock(), ERR_NEEDMOREPARAMS(actualClient->getNick(), "USER"));
	if (actualClient->getNick().empty())
		return serverErrorMsg(actualClient->getSock(), NOTICE_MSG(actualClient->getNick(), "Empty nick, please set a nick first"));
	if (!actualClient->getUser().empty())
		return serverErrorMsg(actualClient->getSock(), ERR_ALREADYREGISTRED(actualClient->getNick()));
	ss >> user;
	ss >> str;
	if (!(str == "0" && ss >> str && str == "*" && ss >> str && str == ":realname"))
		serverErrorMsg(actualClient->getSock(), NOTICE_MSG(actualClient->getNick(), "Unwanted input, username still changed"));
	if (actualClient->getPassWord() != _serverPassWord) // is the password is not correct, dont let the user use commands
		return serverErrorMsg(actualClient->getSock(), ERR_PASSWDMISMATCH(actualClient->getNick()));
	actualClient->setUser(user);
}
