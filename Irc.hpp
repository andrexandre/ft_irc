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
#include <sys/epoll.h>
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
using std::stringstream;
using std::istringstream;
using std::stringstream;

class Client;
class Channel;
class EpollManager;

#include "serverNumeric.hpp"
#include "src/client/Client.hpp"
#include "src/channel/Channel.hpp"
#include "src/epoll/EpollManager.hpp"

extern bool running;

int ssLength(istringstream &ss);
void sendMsg(int fd, string msg);
void serverErrorMsg(int fd, string errMsg);


class Irc
{
	private:
		int _port;
		string _serverPassWord;

		int _serverSock;
		EpollManager* epfds;
		map<int, Client*> _clients; // fd e o respetivo cliente
		// requests é o conjunto de todas as request que tem de ser feitas
		map<int, string> requests; // fd e a respetiva string que contem o comando a ser feito
		vector<Channel*> _serverChannels; // Contem todos os canais do server

	private:
		void initNetWork(void);
		void receiveRequest(int targetFd);
		void setNonBloking(int *ptr);
		bool isNewClient(int targetFd);
		void acceptClient(int serverFd);
		void sendResponse(int targetFd);

	private:
		Client* findClient(int target);
		Client* findClient(string name);
		void deleteClient(map<int, Client*>::iterator& it);
		void leaveAllChannels(Client* ptr);

		Channel* findChannel(string name);
		Channel* createChannel(string name);

	public:
		Irc(void);
		~Irc(void);
		int run_server(char **av);
		void setPort(string arg);
		void setServerPassword(string arg);

	private:
		typedef void (Irc::*CommandPtr)(istringstream& line,  Client* actualClient);
		map<string, CommandPtr> cmds; // Nome to comando e o pointer para a respetiva funcao

		void privmsgCmd(istringstream &ss, Client* actualClient);
		void joinCmd(istringstream &ss, Client* actualClient);
		void partCmd(istringstream &ss, Client* actualClient);
		void topicCmd(istringstream &ss, Client* actualClient);
		void modeCmd(istringstream &ss, Client* actualClient);
		void passCmd(istringstream &ss, Client* actualClient);
		void nickCmd(istringstream &ss, Client* actualClient);
		void userCmd(istringstream &ss, Client* actualClient);
		void inviteCmd(istringstream &ss, Client* actualClient);
		void quitCmd(istringstream &ss, Client* actualClient);
		void kickCmd(istringstream &ss, Client* actualClient);
};
