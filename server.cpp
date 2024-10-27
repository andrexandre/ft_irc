#include "Irc.hpp"

bool running = true;

void handler(int signal)
{
	(void)signal;
	
	running = false;	
	cout << RED << running << END << endl;
	// exit(1);
}

void Irc::sendResponse(int targetFd)
{
	Client* actualClient = findClient(targetFd);
	map<int, string>::iterator it = requests.find(targetFd);
	
	istringstream ss(it->second);
	string tmp;
	string cmdName;
	string content;

	while (std::getline(ss, tmp))
	{
		istringstream line(tmp);
		line >> cmdName;
		cout << cmdName << endl;
		if (cmdName == "NICK")
		{
			line >> content;
			actualClient->setNick(content);
		}
		else if (cmdName == "USER")
		{
			line >> content;
			actualClient->setUser(content);
		}
		if (this->cmds.find(cmdName) != this->cmds.end())
			(this->*(this->cmds[cmdName]))(line, actualClient);
		else {/*erro ou pode ser feito no parser*/}
	}
	
	requests.erase(it);
	epfds->modFd(targetFd, EPOLLIN);
}

void Irc::readRequest(int targetFd)
{
	char buffer[30001];
	bzero(buffer, sizeof(buffer));
	if (read(targetFd, &buffer, 30000) < 0)
		throw std::runtime_error("Error: in readind the fd");
	
	cout << buffer << endl;

	std::istringstream fullContent((string(buffer)));
	string tmp;
	Client* actualClient = findClient(targetFd);
	// Order of commands must be checked !!!
	while (std::getline(fullContent, tmp))
	{
		std::istringstream line(tmp);
		string cmd;
		string content; // temp
		line >> cmd;
		// std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
		if (cmd == "NICK")
		{
			line >> content;
			actualClient->setNick(content);
		}
		else if (cmd == "USER")
		{
			line >> content;
			actualClient->setUser(content);
		}
		else if (cmd == "PASS")
			passCmd(line, actualClient);
		else if (cmd == "privmsg" || cmd == "PRIVMSG")
			privmsgCmd(line, actualClient);
		else if (cmd == "join" || cmd == "JOIN")
			joinCmd(line, actualClient);
		else if (cmd == "PART" || cmd == "part")
			partCmd(line, actualClient);
		else if (cmd == "TOPIC" || cmd == "topic")
			topicCmd(line, actualClient);
		else if (cmd == "MODE" || cmd == "mode")
			modeCmd(line, actualClient);
	}
	
	// epfds->modFd(targetFd, EPOLLOUT); //depois
}

int Irc::run_server(char **av)
{
	struct epoll_event evs[MAX_EVENTS]; //pesquisar coisas
	try
	{
		setPort(av[1]);
		setServerPassword(av[2]);
		initNetWork();

		int event_count = 0;
		int j = 0;	
		while (running)
		{
			cout << "\nPolling for input " << j << "..." << endl;	
			event_count = epoll_wait(epfds->getEpSock(), evs, MAX_EVENTS, -1);
			if (event_count == -1)
				throw std::runtime_error("Error: in epoll_wait");

			cout << "EVENTS READY: " << event_count << '\n' << endl;
			for (int i = 0; i < event_count; i++)
			{
				cout << RED << "Socket that was ready(" << evs[i].data.fd  << ") and the event: " << static_cast<int>(evs[i].events) << END << endl;
				if (isNewClient(evs[i].data.fd) && evs[i].events & EPOLLIN)//new client to the server
					acceptClient(evs[i].data.fd);
				else if (evs[i].events & EPOLLIN)//new request from client
					parsing(evs[i].data.fd);
				else if (evs[i].events & EPOLLOUT)//send response to client
					sendResponse(evs[i].data.fd);
				else if (evs[i].events & EPOLLRDHUP || evs[i].events & EPOLLERR || evs[i].events & EPOLLHUP)
					throw std::runtime_error("Server stoped with EPOLLERR || EPOLLRDHUP || EPOLLHUP");
				else
					break;
			}
			j++;
		}
		cout << RED "Reached uncommon place" END << endl;
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << " 💀" << '\n';
	}
	return 0;
}
