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
    std::string uppercmd;
    for (size_t i = 0; i < cmd.size(); i++) {
        uppercmd += std::toupper(cmd[i]);
    }
    return uppercmd;
}

void Irc::parsing(int targetFd){
    char buffer[30000];
	bzero(buffer, sizeof(buffer));
	if (read(targetFd, &buffer, 30000) < 0)
		throw std::runtime_error("Error: in readind the fd");
	
	Cmd *cmd = new Cmd();

	cmd->setMsg(buffer);
	cmd->setActualClient(_clients[targetFd]);

	Client *client = _clients[targetFd];
	istringstream ss(cmd->getMsg());
	string parsecmd;
	string arg;

	ss >> parsecmd;
	while (ss >> arg) {
        cmd->addArg(trim(arg));
    }

	if(cmd->commands.find(trim(toUpper(parsecmd))) != cmd->commands.end()){
		(cmd->*(cmd->commands[trim(toUpper(parsecmd))]))();
		requests[_clients[targetFd]] = cmd;
	}else{
		send(client->getSock(), "Command not found.\r\n", 21, 0);
		//Aqui poderia ser exibido uma lista com todos os comandos possíveis
	}

	cout << "Client Msg: "<< cmd->getMsg() << endl;
	cmd->getMsg().clear();
}
