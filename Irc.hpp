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
using std::vector;
using std::map;
using std::istringstream;

class Client;
class EpollManager;
class Cmd;

#include "src/client/Client.hpp"
#include "src/epoll/EpollManager.hpp"
#include "src/cmd/Cmd.hpp"

class Irc
{
	private:
		int _serverSock;
		map<int, Client*> _clients; 
		EpollManager* epfds;
		map<Client*, Cmd*> requests;
		
		
	private:
		int _port;
		string _passWord;

	private:
		void initNetWork(void);
		bool isNewClient(int targetFd);
		void acceptClient(int serverFd);
		void deleteClient(map<int, Client*>::iterator& it);
	
	private:
		void parsing(int targetFd);
		void sendResponse(int targetFd);

	public:
		Irc(void);
		~Irc(void);
		int run_server(char **av);
		
	public:
		void setPort(string arg);
		void setPassword(string arg);

};
