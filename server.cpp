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
	
	// Mini-parser
	istringstream RequestSs(it->second);
	string tmpLine;
	string cmdName;

	while (std::getline(RequestSs, tmpLine))
	{
		istringstream lineSs(tmpLine);
		lineSs >> cmdName;
		cout << cmdName << endl; // Logger
		// Executor
		if (this->cmds.find(cmdName) != this->cmds.end())
			(this->*(this->cmds[cmdName]))(lineSs, actualClient);
		else {/*erro ou pode ser feito no parser*/}
	}
	
	requests.erase(it);
	epfds->modFd(targetFd, EPOLLIN);
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
