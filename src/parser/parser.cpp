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

std::string toUpper(const std::string &cmd2) {
    std::string uppercmd2;
    for (size_t i = 0; i < cmd2.size(); i++) {
        uppercmd2 += std::toupper(cmd2[i]);
    }
    return uppercmd2;
}

void Irc::parsing(int targetFd){
    char buffer[30000];
	bzero(buffer, sizeof(buffer));
	if (read(targetFd, &buffer, 30000) < 0)
		throw std::runtime_error("Error: in readind the fd");
	
	Cmd2 *cmd2 = new Cmd2();

	cmd2->setMsg(buffer);
	cmd2->setActualClient(_clients[targetFd]);

	Client *client = _clients[targetFd];
	istringstream ss(cmd2->getMsg());
	string parsecmd2;
	string arg;

	ss >> parsecmd2;
	while (ss >> arg) {
        cmd2->addArg(trim(arg));
    }

	if(cmd2->commands.find(trim(toUpper(parsecmd2))) != cmd2->commands.end()){
		(cmd2->*(cmd2->commands[trim(toUpper(parsecmd2))]))();
		requests[_clients[targetFd]] = cmd2;
	}else{
		send(client->getSock(), "Command not found.\r\n", 21, 0);
		//Aqui poderia ser exibido uma lista com todos os comandos possíveis
	}

	cout << "Client Msg: "<< cmd2->getMsg() << endl;
	cmd2->getMsg().clear();
}
