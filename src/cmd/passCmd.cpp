#include "../../Irc.hpp"

void Irc::passCmd(std::istringstream &ss, Client* actualClient)
{
	string str;
	ss >> str;
	actualClient->setPassWord(str);
}
/*
// This is what should be on the authenticateClient() function
	if (actualClient->getPassWord().empty())
		cout << YELLOW "Password is empty!" END << endl;
	else if (actualClient->getPassWord() == _passWord)
		cout << YELLOW "Correct Password!" END << endl;
	else
		cout << YELLOW "Incorrect Password!" END << endl;
*/