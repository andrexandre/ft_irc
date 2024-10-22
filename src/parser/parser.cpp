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
	
	istringstream lines((string(buffer)));
	string strLine;
	Cmd command = Cmd();

	while (std::getline(lines,strLine))
	{
		std::istringstream line(strLine);
		string cmd;
		line >> cmd;

		if(command.cmd.find(trim(toUpper(cmd))) != command.cmd.end()){
			(command.*(command.cmd[trim(toUpper(cmd))]))(line, _clients[targetFd]);
			requests[_clients[targetFd]] = command;
		}else{
			send(targetFd, "Command not found.\r\n", 21, 0);
			//Aqui poderia ser exibido uma lista com todos os comandos possíveis
		}
		cout << "Client Msg: "<< buffer << endl;
	}
}
