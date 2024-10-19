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

#include "src/client/Client.hpp"
#include "src/epoll/EpollManager.hpp"

class Irc
{
	private:
		int _serverSock;
		map<int, Client*> _clients; 
		EpollManager* epfds;
		string msg; //Para ter o buffer da msg enviada pelo client
		typedef void (Irc::*Command)(Client* actualClient, vector<string> args); //Recebe os mesmos parâmetros das funções de cada comando, é usado no map
		map<string, Command> cmds; //Map para facilitar a chamada de cada comando. Key == comando Value == função de cada comando
		
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

	private:
		void passCmd(Client* actualClient, vector<string> args);
		/*void nickCmd(Client* actualClient, vector<string> args);
		void userCmd(Client* actualClient, vector<string> args);
		void quitCmd(Client* actualClient, vector<string> args);
		void noticeCmd(Client* actualClient, vector<string> args);
		void joinCmd(Client* actualClient, vector<string> args);
		void whoCmd(Client* actualClient, vector<string> args);
		void partCmd(Client* actualClient, vector<string> args);
		void modeCmd(Client* actualClient, vector<string> args);
		void topicCmd(Client* actualClient, vector<string> args);
		void inviteCmd(Client* actualClient, vector<string> args);
		void kickCmd(Client* actualClient, vector<string> args);
		void privmsgCmd(Client* actualClient, vector<string> args);*/

};
