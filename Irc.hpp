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

class Client;
class EpollManager;

#include "src/client/Client.hpp"
#include "src/epoll/EpollManager.hpp"


class Irc
{
	private:
		int _serverSock;
		std::map<int, Client*> _clients; 
		EpollManager* epfds;
	
	private:
		int _port;
		string _passWord;

	private:
		void initNetWork(void);
		bool isNewClient(int targetFd);
		void acceptClient(int serverFd);
		void deleteClient(std::map<int, Client*>::iterator& it);
	
	private:
		void executeAction(int fd);
	
	//See if we really need them?
	private:
		void readRequest(int targetFd);
		void sendResponse(int targetFd);

	public:
		static bool running;
		Irc(void);
		~Irc(void);
		int run_server(char **av);
	public:
		void setPort(string arg);
		void setPassword(string arg);
		// static void handler(int signal);
};
