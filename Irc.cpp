#include "Irc.hpp"

Irc::Irc(void)
{
	cmds["JOIN"] = &Irc::joinCmd;
	cmds["TOPIC"] = &Irc::topicCmd;
	cmds["PRIVMSG"] = &Irc::privmsgCmd;
	cmds["PASS"] = &Irc::passCmd;
	cmds["NICK"] = &Irc::nickCmd;
	cmds["USER"] = &Irc::userCmd;
	cmds["PART"] = &Irc::partCmd;
	cmds["MODE"] = &Irc::modeCmd;
	cmds["INVITE"] = &Irc::inviteCmd;
	cmds["QUIT"] = &Irc::quitCmd;
	cmds["KICK"] = &Irc::kickCmd;
	cout << CYAN "Server started (Ctrl+C to quit)" END << endl;
}

Irc::~Irc(void) 
{
	for (map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
		deleteClient(it);
	for (vector<Channel*>::iterator it = _serverChannels.begin(); it != _serverChannels.end(); it++)
		delete *it;
	if (epfds)
		delete epfds;
	cout << CYAN "Server terminated" END << endl;
}

void Irc::setPortAndPassword(char **av)
{
	char *end;
	int num = strtol(av[1], &end, 10);

	if (*end || num <= 0 || num >= 65535)
		throw std::runtime_error("Invalid port!");

	_port = num;
	_serverPassWord = av[2];
}
