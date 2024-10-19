#include "Irc.hpp"

//Comentado por não ter nenhum comando criado ( o PASS é só para teste )
Irc::Irc(void) {
	cmds["PASS"] = &Irc::passCmd;
	/*cmds["NICK"] = &Irc::nickCmd;
	cmds["USER"] = &Irc::userCmd;
	cmds["QUIT"] = &Irc::quitCmd;
	cmds["NOTICE"] = &Irc::noticeCmd;
	cmds["JOIN"] = &Irc::joinCmd;
	cmds["WHO"] = &Irc::whoCmd;
	cmds["MODE"] = &Irc::modeCmd;
	cmds["TOPIC"] = &Irc::topicCmd;
	cmds["INVITE"] = &Irc::inviteCmd;
	cmds["KICK"] = &Irc::kickCmd;
	cmds["PRIVMSG"] = &Irc::privmsgCmd;*/
}

Irc::~Irc(void) 
{
	
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end();)
		deleteClient(it);
	if (epfds)
		delete epfds;
}

void Irc::setPassword(string arg) {
	_passWord = arg;
}

void Irc::setPort(string arg)
{
	char *end;
	int num = strtol(arg.c_str(), &end, 10);

	if (*end || num <= 0  || num >= 65535)
		throw std::runtime_error("Error: Invalid port!");;

	_port = num;	
}
void Irc::passCmd(Client* actualClient, vector<string> args){
	(void)actualClient;
	(void)args;
	cout << "estou no pass\n";
}
