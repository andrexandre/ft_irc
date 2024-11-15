#pragma once

#include "../../Irc.hpp"

class Client
{
	private:
		int _connectionSock;
		int _isAuthenticated;

		// change <var> to _<var>
		string nick;
		string user;
		string passWord;
	
	public:
		string _buffer;
		Client(int connectionSock);
		~Client(void);
	
	public:
		int getSock(void) const;
		int isAuthenticated(void) const;
		string getNick(void) const { return (nick);}
		string getUser(void) const { return (user);}
		string getPassWord(void) const { return (passWord);}

		void setNick(string name) { nick = name;}
		void setUser(string name) { user = name;}
		void setPassWord(string name) { passWord = name;}
		void authenticate(void);

};