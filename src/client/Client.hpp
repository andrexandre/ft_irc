#pragma once

#include "../../webserv.hpp"


class Client
{
	private:
		int _connectionSock;
	
	private:
		//Vao ser setados depois no primeiro request
		string _hostname;
		Server_block* _siteContent;
		bool _closeConn;
		time_t _lastRequestTime;
	
	public:
		Client(int connectionSock);
		~Client(void);
		int requestVal;
	
	public:
		int getSock(void) const;
		void setHostName(string host);
		void setSiteContent(Server_block* ptr);
		void setLastRquestTime(time_t actualTime);
		bool shouldCloseConn(void);
		
	public:
		time_t getLastRquestTime(void) const;
	
	public:
		// httpRequest input;
		// httpResponse output;
};