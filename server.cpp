#include "Irc.hpp"

bool running = true;

void handler(int signal)
{
	(void)signal;
	
	running = false;	
	cout << RED << running << END << endl;
	// exit(1);
}

void Irc::initNetWork(void)
{
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	bzero(&address, addrlen);
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htons(INADDR_ANY);
	// address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(_port);

	_serverSock = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSock == -1)
		throw std::runtime_error("Error: Failed to create socket");
	
	int enable = 1;
	if (setsockopt(_serverSock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		throw std::runtime_error("Error: Failed to set setsockopt to SO_REUSEADDR");
	
	if (bind(_serverSock, (struct sockaddr *)&address, addrlen) < 0)
		throw std::runtime_error("Error: Failed to bind socket to a port");

	if (listen(_serverSock, BACKLOG) < 0)
		throw std::runtime_error("Error: Failed to listen to socket");

	epfds = new EpollManager();
	epfds->addFd(_serverSock, EPOLLIN | EPOLLET);
}


void setNonBloking(int *ptr)
{
	int fd = fcntl(*ptr, F_SETFL, O_NONBLOCK);
	if (fd == -1)
		throw std::runtime_error("Error: Failed to set the soket to nonBlocking");
}


void Irc::acceptClient(int serverFd)
{
	//salvar o addres depois	
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);
	bzero(&address, addrlen);

	int newSock;
	if ((newSock = accept(serverFd, (struct sockaddr *)&address, &addrlen)) < 0)
		throw std::runtime_error("Error: Failed to accept connection");

	epfds->addFd(newSock, EPOLLIN | EPOLLERR | EPOLLRDHUP | EPOLLHUP);
	// setNonBloking(&newSock);
	_clients.insert(std::make_pair(newSock, (new Client(newSock))));//create new client
	cout << "NEW CLIENT ADDED TO THE SERVER" << "(fd: " << newSock << ")" << endl;
}

bool Irc::isNewClient(int targetFd){
	return((targetFd == _serverSock) ? true : false);
}

void Irc::sendResponse(int targetFd)
{
	string nick = "aleperei";
	string msg = nick + " has logged in" + "\r\n";
	send(targetFd, msg.c_str(), msg.size(), 0);
	epfds->modFd(targetFd, EPOLLIN);
	// (void) targetFd;
}

void Irc::readRequest(int targetFd)
{
	char buffer[30001];
	bzero(buffer, sizeof(buffer));
	if (read(targetFd, &buffer, 30000) < 0)
		throw std::runtime_error("Error: in readind the fd");
	
	cout << buffer << endl;

	std::istringstream conn((string(buffer)));
	string buf;
	Client* actualClient = findClient(targetFd);
	
	while (std::getline(conn,buf))
	{

		std::istringstream line(buf);
		
		string cmd;
		string content;
		line >> cmd;

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
		else if (cmd == "privmsg" || cmd == "PRIVMSG")
			privmsgCmd(line, actualClient);
		else if (cmd == "join" || cmd == "JOIN")
			joinCmd(line, actualClient);
		else if (cmd == "PART" || cmd == "part")
			partCmd(line, actualClient);

		
	}
	
	// epfds->modFd(targetFd, EPOLLOUT); //depois
}

void Irc::deleteClient(std::map<int, Client*>::iterator& it)
{
	(void)it;
	// std::map<int, Client*>::iterator tmp = it;
	// ++it;
	// epfds->deleteFd(tmp->first);
	// delete tmp->second;
	// _clients.erase(tmp);
}


int Irc::run_server(char **av)
{
	struct epoll_event evs[MAX_EVENTS]; //pesquisar coisas
	try
	{
		setPort(av[1]);
		setPassword(av[2]);
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
					// return 1;
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
	cout << "alex" << endl;
	return 0;
}
