#include "../../Irc.hpp"

string trim(const string &str) {
    const string whitespace = " \t\n\r\f\v";
    size_t start = str.find_first_not_of(whitespace);
    if (start == string::npos) {
        return "";
    }
    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

std::string toUpper(const std::string &cmd) {
    std::string upperCmd;
    for (size_t i = 0; i < cmd.size(); i++) {
        upperCmd += std::toupper(cmd[i]);
    }
    return upperCmd;
}

void Irc::parsing(int targetFd){
    char buffer[30000];
	bzero(buffer, sizeof(buffer));
	if (read(targetFd, &buffer, 30000) < 0)
		throw std::runtime_error("Error: in readind the fd");
	
	msg += buffer;

	vector<string> args;
	Client *client = _clients[targetFd];
	istringstream ss(msg);
	string cmd;
	string arg;

	ss >> cmd;
	while (ss >> arg) {
        args.push_back(trim(arg));
    }

	if(cmds.find(trim(toUpper(cmd))) != cmds.end()){
		(this->*(this->cmds[trim(toUpper(cmd))]))(client,args);
	}else{
		send(client->getSock(), "Command not found.\r\n", 21, 0);
		//Aqui poderia ser exibido uma lista com todos os comandos possíveis
	}

	cout << "Client Msg: "<< msg << endl;
	msg.clear();
}
