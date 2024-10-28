#include "Irc.hpp"

Irc::Irc(void)
{
	cmds["JOIN"] = &Irc::joinCmd;
	cmds["TOPIC"] = &Irc::topicCmd;
	cmds["PRIVMSG"] = &Irc::privmsgCmd;
	cmds["PASS"] = &Irc::passCmd;
	cmds["NICK"] = &Irc::nickCmd;
	cmds["USER"] = &Irc::userCmd;
	// cmds["QUIT"] = &Irc::quitCmd;
	// cmds["NOTICE"] = &Irc::noticeCmd;
	// cmds["WHO"] = &Irc::whoCmd;
	// cmds["MODE"] = &Irc::modeCmd;
	// cmds["INVITE"] = &Irc::inviteCmd;
	// cmds["KICK"] = &Irc::kickCmd;
}

Irc::~Irc(void) 
{
	
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end();)
		deleteClient(it);
	if (epfds)
		delete epfds;
}

void Irc::setServerPassword(string arg) {
	_serverPassWord = arg;
}

void Irc::setPort(string arg)
{
	char *end;
	int num = strtol(arg.c_str(), &end, 10);

	if (*end || num <= 0  || num >= 65535)
		throw std::runtime_error("Error: Invalid port!");;

	_port = num;	
}

void Irc::serverErrorMsg(int fd, string errMsg)
{
	if (send(fd, errMsg.c_str(), errMsg.size(), 0) == -1)
		throw std::runtime_error("Error: in sending the response");
}
