#include "Cmd2.hpp"

Cmd2::Cmd2(){
	commands["PASS"] = &Cmd2::passCmd;
	/*commands["NICK"] = &Cmd2::nickCmd;
	commands["USER"] = &Cmd2::userCmd;
	commands["QUIT"] = &Cmd2::quitCmd;
	commands["NOTICE"] = &Cmd2::noticeCmd;
	commands["JOIN"] = &Cmd2::joinCmd;
	commands["WHO"] = &Cmd2::whoCmd;
	commands["MODE"] = &Cmd2::modeCmd;
	commands["TOPIC"] = &Cmd2::topicCmd;
	commands["INVITE"] = &Cmd2::inviteCmd;
	commands["KICK"] = &Cmd2::kickCmd;
	commands["PRIVMSG"] = &Cmd2::privmsgCmd;*/
}

Cmd2::~Cmd2(void) 
{}

void Cmd2::passCmd(){
    std::stringstream ss;
    ss << actualClient->getSock();
    std::string message = "estou no pass com o client = " + ss.str() + "\r\n";

	cout << message;
    send(actualClient->getSock(), message.c_str(), message.size(), 0);
}

void Cmd2::setMsg(char buffer[30000]){
    msg += buffer;
}
string Cmd2::getMsg(void){
    return msg;
}

void Cmd2::addArg(string arg){
    args.push_back(arg);
}

void Cmd2::setActualClient(Client* client){
    actualClient = client;
}
