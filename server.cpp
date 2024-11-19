#include "Irc.hpp"

bool running = true;

void handler(int signal)
{
	(void)signal;
	running = false;
	cout << endl;
}

void Irc::receiveRequest(int targetFd)
{
	char buffer[30000];
	bzero(buffer, sizeof(buffer));
	istringstream ss;
	Client *client = findClient(targetFd);

	if (recv(targetFd, &buffer, sizeof(buffer), 0) <= 0)
		return quitCmd(ss, client);
	client->_buffer += string(buffer);
	if (client->_buffer.find('\n') == string::npos)
		return;
	requests.insert(std::make_pair(targetFd, client->_buffer));
	epfds->modFd(targetFd, EPOLLOUT);
	client->_buffer.clear();
}

void Irc::sendResponse(int targetFd)
{
	Client* client = findClient(targetFd);
	map<int, string>::iterator it = requests.find(targetFd);
	
	istringstream RequestSs(it->second);
	string tmpLine;
	string cmdName;

	cout << "Received from client fd: " << targetFd << endl;
	cout << WHITE << RequestSs.str() << END << endl;
	while (std::getline(RequestSs, tmpLine))
	{
		istringstream lineSs(tmpLine);
		lineSs >> cmdName;
		if (cmdName == "CAP")
			continue;
		if (!client->isAuthenticated() && cmdName != "PASS" && cmdName != "NICK" &&
			cmdName != "USER" && cmdName != "CAP" && cmdName != "QUIT")
		{
			sendMsg(client->getSock(), ERR_NOTREGISTERED(client->getNick()));
			continue;
		}
		if (this->cmds.find(cmdName) != this->cmds.end())
			(this->*(this->cmds[cmdName]))(lineSs, client);
		else
			sendMsg(client->getSock(), ERR_UNKNOWNCOMMAND(client->getNick(), cmdName));
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
		setPortAndPassword(av);
		initNetWork();

		int event_count = 0;
		int j = 0;
		while (running)
		{
			logger(1, j);
			event_count = epoll_wait(epfds->getEpSock(), evs, MAX_EVENTS, -1);
			if (event_count == -1)
				throw std::runtime_error("epoll_wait");

			logger(2, event_count);
			for (int i = 0; i < event_count; i++)
			{
				logger(3, evs[i].data.fd);
				logger(4, evs[i].events);
				if (isNewClient(evs[i].data.fd) && evs[i].events & EPOLLIN) // new client to the server
					acceptClient(evs[i].data.fd);
				else if (evs[i].events & EPOLLIN) // receive request from client
					receiveRequest(evs[i].data.fd);
				else if (evs[i].events & EPOLLOUT) // send response to client
					sendResponse(evs[i].data.fd);
				else
					break;
			}
			j++;
		}
		logger(5, 0);
	}
	catch(const std::exception& e)
	{
		if (running)
			cerr << "Error: " << e.what() << " 💀" << '\n';
	}
	return 0;
}
