#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <iterator>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <csignal>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <sys/epoll.h>  // Para epoll
#include <fcntl.h>


#define END			"\033[0m"
#define RED			"\033[1;31m"
#define GREEN		"\033[1;32m"
#define YELLOW		"\033[1;33m"
#define BLUE		"\033[1;34m"
#define MAGENTA		"\033[1;35m"
#define CYAN		"\033[1;36m"
#define WHITE		"\033[1;37m"

#define BACKLOG 100
#define MAX_EVENTS 1024

using std::string;
using std::cout;
using std::cerr;
using std::endl;

// apagar depois
extern int linepos;
extern int verbose;

class Client;
class EpollManager;
class Server_block;
class httpRequest;
class httpResponse;

#include "src/parsing/Parsing.hpp"
#include "src/client/Client.hpp"
#include "src/epoll/EpollManager.hpp"
#include "src/httpHandler/httpHandler.hpp"



class WebServ
{
	private:
		std::vector<Server_block*> _serversConfig;
		std::vector<Server_block*> usableServers;
		std::vector<int> _serverFds;
		EpollManager* epfds;

		std::map<int, Client*> _clients; 
		// std::map<string, int> _connaddr; 

	// Servers methods to netWork
	private:
		void initNetWork(void);
		void acceptClient(int serverFd);
		bool isNewClient(int targetFd);
		void readRequest(int targetFd);
		void sendResponse(int targetFd);
		void chooseBlocks(void);
		void timeOut(void);
		void deleteClient(std::map<int, Client*>::iterator& it);

	public:
		WebServ(void);
		~WebServ(void);

	// Servers methods to S. blocks
	public:
		void printServers(void);//temporario
		void mandatoryConfig(void) const;
		void setServers(Server_block* other);
		int run_server(void);
};

