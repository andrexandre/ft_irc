#pragma once

#include "../../Irc.hpp"


class Client
{
	private:
		int _connectionSock;
	
	public:
		Client(int connectionSock);
		~Client(void);
	
	public:
		int getSock(void) const;

};