#include "Cmd.hpp"

Cmd::Cmd(){
	commands["PASS"] = &Cmd::passCmd;
	/*commands["NICK"] = &Cmd::nickCmd;
	commands["USER"] = &Cmd::userCmd;
	commands["QUIT"] = &Cmd::quitCmd;
	commands["NOTICE"] = &Cmd::noticeCmd;
	commands["JOIN"] = &Cmd::joinCmd;
	commands["WHO"] = &Cmd::whoCmd;
	commands["MODE"] = &Cmd::modeCmd;
	commands["TOPIC"] = &Cmd::topicCmd;
	commands["INVITE"] = &Cmd::inviteCmd;
	commands["KICK"] = &Cmd::kickCmd;
	commands["PRIVMSG"] = &Cmd::privmsgCmd;*/
}

Cmd::~Cmd(void) 
{}

void Cmd::passCmd(){
    std::stringstream ss;
    ss << actualClient->getSock();
    std::string message = "estou no pass com o client = " + ss.str() + "\r\n";

	cout << message;
    send(actualClient->getSock(), message.c_str(), message.size(), 0);
}

void Cmd::setMsg(char buffer[30000]){
    msg += buffer;
}
string Cmd::getMsg(void){
    return msg;
}

void Cmd::addArg(string arg){
    args.push_back(arg);
}

void Cmd::setActualClient(Client* client){
    actualClient = client;
}
