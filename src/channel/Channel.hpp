#pragma once

#include "../../Irc.hpp"


class Channel
{
	private:
		size_t _usersNumber;
		string _channelName;
		std::map<Client*, bool> _channelUsers;

	public:
		string getChannelName(void) const;
		size_t getUsersNumber(void) const;
		void setChannelUsers(bool oprt, Client* ptr);
		bool isPartOfChannel(string userName) const;
		void sendMsg(int fd, string& msg) const;

	public:
		Channel(string name);
		~Channel(void);
};