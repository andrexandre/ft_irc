#pragma once

#include "../../Irc.hpp"


class Client
{
	private:
		int _connectionSock;
		string nick;
		string user;
	
	public:
		Client(int connectionSock);
		~Client(void);
	
	public:
		int getSock(void) const;

};