#include "webserv.hpp"

void handler(int signal)
{
	(void)signal;
	throw std::runtime_error("\nServer stopped!");
}

// void setNonBloking(int *ptr)
// {
// 	int fd = fcntl(*ptr, F_SETFL, O_NONBLOCK);
// 	if (fd == -1)
// 		throw std::runtime_error("Error: Failed to set the soket to nonBlocking");
// }

void WebServ::chooseBlocks(void)
{
    std::vector<Server_block*> duplicates; // Para armazenar os duplicados encontrados
    // Iterar sobre todos os servidores em _serversConfig
    for (size_t i = 0; i < _serversConfig.size(); i++)
    {
        bool isDuplicate = false;

        // Verificar se o servidor atual já existe em `result` ou em `duplicates`
        for (size_t j = 0; j < usableServers.size(); j++)
        {
            if (*_serversConfig[i] == *usableServers[j]) // Se já estiver em result, é duplicado
            {
                duplicates.push_back(_serversConfig[i]); // Adicionar aos duplicados
                isDuplicate = true;
                break;
            }
        }

        // Verificar se já está nos duplicados
        for (size_t k = 0; k < duplicates.size(); k++)
        {
            if (*_serversConfig[i] == *duplicates[k]) // Já foi encontrado como duplicado
            {
                isDuplicate = true;
                break;
            }
        }

        // Adicionar ao result apenas se não for duplicado
        if (!isDuplicate)
            usableServers.push_back(_serversConfig[i]);
    }
}


void WebServ::initNetWork(void)
{
	epfds = new EpollManager();

	int port;
	size_t idx;
	int addrlen;
	int enable = 1;
	string hostTmp;
	int serverSock; 

	//se os servers bloks ouvirem da mesma porta e ip so vai existir um soket 
	//para o primeiro server block que aparecer no congif file
	chooseBlocks();
	cout << usableServers.size() << endl;
	for (size_t i = 0; i < usableServers.size(); i++)
	{
		idx = usableServers[i]->getHostName().find(':');
		hostTmp = usableServers[i]->getHostName().substr(0, idx);

		struct sockaddr_in address;
		addrlen = sizeof(address);
		bzero(&address, addrlen);
		
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htons(INADDR_ANY);
		port = this->usableServers[i]->getPort()[0];
		address.sin_port = htons(port);

		try
		{
			serverSock = socket(AF_INET, SOCK_STREAM, 0);
			if (serverSock == -1)
				throw std::runtime_error("Error: Failed to create socket");
			
			if (setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
				throw std::runtime_error("Error: Failed to set setsockopt to SO_REUSEADDR");
			
			if (bind(serverSock, (struct sockaddr *)&address, addrlen) < 0)
				throw std::runtime_error("Error: Failed to bind socket to a port");

			if (listen(serverSock, BACKLOG) < 0)
				throw std::runtime_error("Error: Failed to listen to socket");
			
			// setNonBloking(&serverSock);

			_serverFds.push_back(serverSock);
			epfds->addFd(serverSock);//epollet para os evers sockets
		}
		catch(const std::exception& e)
		{
			close(serverSock);
			cerr << e.what() << endl;
			continue;
		}
		cout << "Server connection on: " << this->usableServers[i]->getHostName() << endl;
	}

	if (_serverFds.empty())
		throw std::runtime_error("Error: Failed to listen to socket");
}



void WebServ::acceptClient(int serverFd)
{
	//salvar o addres depois	
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);
	bzero(&address, addrlen);

	int new_sock;
	if ((new_sock = accept(serverFd, (struct sockaddr *)&address, &addrlen)) < 0)
		throw std::runtime_error("Error: Failed to accept connection");
	

	
	//setNonBloking(&new_sock);

	//add the new sock to the mager of epoll
	epfds->addFd(new_sock);

	_clients.insert(std::make_pair(new_sock, (new Client(new_sock))));//create new client
	_clients.at(new_sock)->setLastRquestTime(time(NULL));
	cout << "NEW CLIENT ADDED TO THE SERVER" << "(fd: " << new_sock << ")" << endl;					
}

bool WebServ::isNewClient(int targetFd)
{
	if (std::find(_serverFds.begin(), _serverFds.end(), targetFd) == _serverFds.end())
		return (false);
	
	return (true);
}

void WebServ::sendResponse(int targetFd)
{
	// std::ifstream test("index.html");// apagar depois
	std::map<int, Client*>::iterator it = _clients.find(targetFd);

	std::stringstream ss;
	ss << it->second->requestVal;
	string msg = "<html><h1>Hello, World ";
	msg += ss.str() + "!</h1></html>";
	string resp = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: ";
	ss.str("");
	ss << msg.length();
	resp = resp + ss.str() + "\r\n\r\n" + msg;

	
	if (send(it->second->getSock(), resp.c_str(), resp.length(), 0) == -1)
	{
		deleteClient(it);
		throw std::runtime_error("Error: in sending the response");
	}

	cout << "Response sent" << endl << endl;
	// if (it->second->shouldClose())
	// 	deleteClient(it);
	// else
	epfds->modFd(it->second->getSock(), EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLRDHUP);
	// TODO: CHECK IF NEEDS TO CLOSE CONNECTION AFTER SENDING THE RESPONSE
	it->second->requestVal++;
	// test.close();// apagar depois
}

void WebServ::readRequest(int targetFd)
{
	std::map<int, Client*>::iterator it = _clients.find(targetFd);

	cout << "NEW REQUEST FROM CLIENTE TO THE SERVER" << endl;
	char buffer[30000];
	bzero(&buffer, sizeof(buffer));
	long bytes = read(it->second->getSock(), buffer, 30000);
	if (bytes <= 0)
	{
		deleteClient(it);
		cout << "Client removed" << endl;
		return;
		// throw std::runtime_error("Error: in readind request");
	}
	
	cout << "Request received" << endl;
	cout << buffer << endl;

	it->second->setLastRquestTime(time(NULL));

	epfds->modFd(it->second->getSock(), EPOLLOUT | EPOLLHUP | EPOLLERR | EPOLLRDHUP);
}

void WebServ::deleteClient(std::map<int, Client*>::iterator& it)
{
	std::map<int, Client*>::iterator tmp = it;
	++it;
	epfds->deleteFd(tmp->first);
	delete tmp->second;
	_clients.erase(tmp);
}

void WebServ::timeOut(void)
{
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end();)
	{
		time_t actualTime = time(NULL);
		if (difftime(actualTime, it->second->getLastRquestTime()) >= 90)
		{
			cout << "Client(" << it->first << ") was desconnected" << endl;
			deleteClient(it);
		}
		else
			it++;
	}
}


int WebServ::run_server(void)
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
				if (evs[i].events & EPOLLRDHUP)
				{
					std::map<int, Client*>::iterator it = _clients.find(evs[i].data.fd);
					deleteClient(it);
					cout << "CLIENT REMOVED" << endl;
					continue;
				}
				else if (isNewClient(evs[i].data.fd) && evs[i].events & EPOLLIN)//new client to the server
					acceptClient(evs[i].data.fd);
				else if (evs[i].events & EPOLLIN)//new request from client
					readRequest(evs[i].data.fd);
				else if (evs[i].events & EPOLLOUT)//send response to client
					sendResponse(evs[i].data.fd);
				else if (evs[i].events & EPOLLERR)
				{
					int err = 0;
					socklen_t len = sizeof(err);
					if (getsockopt(evs[i].data.fd, SOL_SOCKET, SO_ERROR, &err, &len) == 0) 
						printf("Error code: %d\n", err);  // Imprimir o código de erro.
					
					throw std::runtime_error("Server stoped with EPOLLERR");
				}
				else if (evs[i].events & EPOLLHUP)
				{
					// epoll_ctl(epfds->getEpSock(), EPOLL_CTL_DEL, evs[i].data.fd, NULL);
					// close(evs[i].data.fd);
					throw std::runtime_error("Server stoped with EPOLLHUP");
				}
				else
					break;
			}
			timeOut();
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









































// v1
// int WebServ::server_core(void)
// {
// 	signal(SIGINT, handler);
// 	try
// 	{
// 		struct sockaddr_in address;
// 		int addrlen = sizeof(address);
// 		bzero(&address, addrlen);
// 		address.sin_family = AF_INET;
// 		address.sin_addr.s_addr = htons(INADDR_ANY); // INADDR_ANY: any ip address
// 		int port = this->_serversConfig[0]->getPort()[0];
// 		address.sin_port = htons(port);
// 		cout << "Server opened to any address on port: " << port << endl;
//
// 		// domain IPv4, service TCP, default protocol, port, interface, bklog
// 		// AF_INET,     SOCK_STREAM, 0,                8080, INADDR_ANY, 10
// 		serverSock = socket(AF_INET, SOCK_STREAM, 0);
// 		if (serverSock == -1)
// 			throw std::runtime_error("Error: Failed to create socket");
// 		int enable = 1;
// 		if (setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
// 			throw std::runtime_error("Error: Failed to set setsockopt to SO_REUSEADDR");
// 		if (bind(serverSock, (struct sockaddr *)&address, addrlen) < 0)
// 			throw std::runtime_error("Error: Failed to bind socket or socket already open");
// 		int bklog = 10;
// 		if (listen(serverSock, bklog) < 0)
// 			throw std::runtime_error("Error: Failed to listen to socket");
//
// 		int i = 1;
// 		while (true)
// 		{
// 			cout << "Server waiting for request..." << endl;
// 			int new_sock;
// 			if ((new_sock = accept(serverSock, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
// 				throw std::runtime_error("Error: Failed to accept connection");
// 			char buffer[30000];
// 			bzero(&buffer, sizeof(buffer));
// 			read(new_sock, buffer, 30000);
// 			string msg = "Hi!";
// 			string resp = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: ";
// 			// msg = string(buffer); // Uncomment to send what we recieved instead of msg
// 			std::stringstream ss;
// 			ss << msg.length();
// 			resp = resp + ss.str() + "\n\n" + msg;
// 			write(new_sock, resp.c_str(), resp.length());
// 			close(new_sock);
// 			cout << buffer << endl;
// 			cout << "Request received!" << " Requests: " << i << endl;
// 			i++;
// 		}
// 		cout << RED "Reached uncommon place" END << endl;
// 	}
// 	catch(const std::exception& e)
// 	{
// 		cerr << e.what() << " 💀" << '\n';
// 	}
// 	close(serverSock);
// 	return 0;
// }