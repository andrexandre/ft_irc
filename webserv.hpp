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

#define END			"\033[0m"
#define RED			"\033[1;31m"
#define GREEN		"\033[1;32m"
#define YELLOW		"\033[1;33m"
#define BLUE		"\033[1;34m"
#define MAGENTA		"\033[1;35m"
#define CYAN		"\033[1;36m"
#define WHITE		"\033[1;37m"

using std::string;
using std::cout;
using std::cerr;
using std::endl;

// apagar depois
extern int linepos;
extern int verbose;

#include "parsing/Parsing.hpp"

class Server_block;

class WebServ
{
	private:
		std::vector<Server_block*> _servers;
		int sock;
	
	public:
		WebServ(void);
		~WebServ(void);

		int server_core(void);

	// Servers methods
	public:
		void printServers(void);
		void mandatoryConfig(void) const;
		void setServers(Server_block* other);
};

int server_core(void);
int checkField(string& name, size_t srt);
