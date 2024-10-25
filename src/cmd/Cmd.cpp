#include "Cmd.hpp"

Cmd::Cmd(Client* ptr) : currentClient(ptr)
{
	cmd["PASS"] = &Cmd::passCmd;
	/*cmd["NICK"] = &Cmd::nickCmd;
	cmd["USER"] = &Cmd::userCmd;
	cmd["QUIT"] = &Cmd::quitCmd;
	cmd["NOTICE"] = &Cmd::noticeCmd;
	cmd["JOIN"] = &Cmd::joinCmd;
	cmd["WHO"] = &Cmd::whoCmd;
	cmd["MODE"] = &Cmd::modeCmd;
	cmd["TOPIC"] = &Cmd::topicCmd;
	cmd["INVITE"] = &Cmd::inviteCmd;
	cmd["KICK"] = &Cmd::kickCmd;
	cmd["PRIVMSG"] = &Cmd::privmsgCmd;*/
	(void) currentClient;
}

Cmd::~Cmd(void) {}

void Cmd::appendBufer(string msg) {
	buffer += msg;
}

void Cmd::setSs(void) {
	ss.str(buffer);
}


void Cmd::passCmd(){
	// (void)iss;
    // std::stringstream ss;
    // ss << actualClient->getSock();
    // std::string message = "estou no pass com o client = " + ss.str() + "\r\n";

	// cout << message;
    // send(actualClient->getSock(), message.c_str(), message.size(), 0);
}
