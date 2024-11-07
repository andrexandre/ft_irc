#include "Irc.hpp"

bool running = true;

void handler(int signal)
{
	(void)signal;
	running = false;
	cout << CYAN "\nTerminating server" END << endl;
}

void Irc::sendResponse(int targetFd)
{
	Client* actualClient = findClient(targetFd);
	map<int, string>::iterator it = requests.find(targetFd);
	
	// Mini-parser
	istringstream RequestSs(it->second);
	string tmpLine;
	string cmdName;

	cout << RequestSs.str() << endl;
	while (std::getline(RequestSs, tmpLine))
	{
		istringstream lineSs(tmpLine);
		lineSs >> cmdName;
		// Executor
		if (!actualClient->isAuthenticated() && cmdName != "PASS" && cmdName != "NICK" &&
			cmdName != "USER" && cmdName != "CAP" && cmdName != "QUIT")
		{
			serverErrorMsg(actualClient->getSock(), ERR_NOTREGISTERED(actualClient->getNick()));
			continue;
		}
		if (this->cmds.find(cmdName) != this->cmds.end())
			(this->*(this->cmds[cmdName]))(lineSs, actualClient);
		else
			serverErrorMsg(actualClient->getSock(), ERR_UNKNOWNCOMMAND(actualClient->getNick(), cmdName));
	}
	
	requests.erase(it);
	epfds->modFd(targetFd, EPOLLIN);
}

int Irc::run_server(char **av)
{
	struct epoll_event evs[MAX_EVENTS];
	try
	{
		signal(SIGINT, handler);
		setPort(av[1]);
		setServerPassword(av[2]);
		initNetWork();

		int event_count = 0;
		int j = 0;
		while (running)
		{
			cout << BLUE "\n" << j << " Inputs received, Waiting for event..." END << endl;
			event_count = epoll_wait(epfds->getEpSock(), evs, MAX_EVENTS, -1);
			if (event_count == -1)
				throw std::runtime_error("epoll_wait");

			cout << "Fds received: " << event_count << endl;
			for (int i = 0; i < event_count; i++)
			{
				cout << GREEN "Received socket n: " << evs[i].data.fd  << " with event ";
				string eventString;
				if (evs[i].events & EPOLLIN)
					eventString = "EPOLLIN";
				else if (evs[i].events & EPOLLOUT)
					eventString = "EPOLLOUT";
				else if (evs[i].events & (EPOLLIN | EPOLLRDHUP))
					eventString = "EPOLLIN | EPOLLRDHUP";
				else if (evs[i].events & EPOLLERR || evs[i].events & EPOLLRDHUP || evs[i].events & EPOLLHUP)
					eventString = "EPOLLERR || EPOLLRDHUP || EPOLLHUP";
				else
					eventString = "INVALID";
				cout << eventString << END << endl;
				if (isNewClient(evs[i].data.fd) && evs[i].events & EPOLLIN)//new client to the server
					acceptClient(evs[i].data.fd);
				else if (evs[i].events & EPOLLIN)//new request from client
					parsing(evs[i].data.fd);
				else if (evs[i].events & EPOLLOUT)//send response to client
					sendResponse(evs[i].data.fd);
				else
					break;
			}
			j++;
			if (j == 1000)
			{
				cout << RED "Infinite loop detected, terminating server" END << endl;
				break;
			}
		}
		cout << RED "Reached uncommon place" END << endl;
	}
	catch(const std::exception& e)
	{
		if (running)
			cerr << "Error: " << e.what() << " 💀" << '\n';
	}
	return 0;
}
