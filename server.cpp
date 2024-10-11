#include "Irc.hpp"

void handler(int signal)
{
	(void)signal;
	throw std::runtime_error("\nServer stopped!");
}

void Irc::initNetWork(void)
{
	epfds = new EpollManager();

	// int port;
	// size_t idx;
	// int addrlen;
	// int enable = 1;
	// string hostTmp;
	// int serverSock; 

	
}


void Irc::acceptClient(int serverFd)
{
	//salvar o addres depois	
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);
	bzero(&address, addrlen);

	int new_sock;
	if ((new_sock = accept(serverFd, (struct sockaddr *)&address, &addrlen)) < 0)
		throw std::runtime_error("Error: Failed to accept connection");
	
	epfds->addFd(new_sock);
	_clients.insert(std::make_pair(new_sock, (new Client(new_sock))));//create new client
	cout << "NEW CLIENT ADDED TO THE SERVER" << "(fd: " << new_sock << ")" << endl;					
}

bool Irc::isNewClient(int targetFd){
	return((targetFd == _serverSock) ? true : false);
}

void Irc::sendResponse(int targetFd)
{
	(void) targetFd;
}

void Irc::readRequest(int targetFd)
{
	(void) targetFd;
}

void Irc::deleteClient(std::map<int, Client*>::iterator& it)
{
	// std::map<int, Client*>::iterator tmp = it;
	// ++it;
	// epfds->deleteFd(tmp->first);
	// delete tmp->second;
	// _clients.erase(tmp);
}


int Irc::run_server(void)
{
	signal(SIGINT, handler);
	struct epoll_event evs[MAX_EVENTS]; //pesquisar coisas
	try
	{
		initNetWork();
		
		int event_count = 0;
		int j = 0;
		while (true)
		{
			cout << "\nPolling for input " << j << "..." << endl;			
			event_count = epoll_wait(epfds->getEpSock(), evs, MAX_EVENTS, 30000);//30seconds
			if (event_count == -1)
				throw std::runtime_error("Error: in epoll_wait");

			cout << "EVENTS READY: " << event_count << '\n' << endl;
			for (int i = 0; i < event_count; i++)
			{
				cout << RED << "Socket that was ready(" << evs[i].data.fd  << ") and the event: " << static_cast<int>(evs[i].events) << END << endl;;
				 if (isNewClient(evs[i].data.fd) && evs[i].events & EPOLLIN)//new client to the server
					acceptClient(evs[i].data.fd);
				else if (evs[i].events & EPOLLIN)//new request from client
					readRequest(evs[i].data.fd);
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

