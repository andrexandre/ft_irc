#pragma once

#include "../../Irc.hpp"


class Channel
{
	private:
		size_t _usersNumber;
		string _channelName;
		string _channelTopic;
		string _channelModes;
		std::map<Client*, bool> _channelUsers;

		vector<string> _inviteUsers; //i: Set/remove Invite-only channel

	public:
		string getChannelName(void) const;
		size_t getUsersNumber(void) const;
		string getChannelTopic(void) const;

		void setChannelUsers(bool oprt, Client* ptr);
		void setChannelTopic(string content);

		void removeClient(Client* ptr);
		bool isPartOfChannel(string userName) const;
		bool isOperator(string userName) const;

		void sendAll(string msg) const;
		void sendPrivMsg(int fd, string msg) const;

		void apllyInviteOnlyFlag(bool optr);
		void setChannelModes(char flag);
		void removeChannelModesFlag(char flag);
		bool isFlagSet(char flag);
	public:
		Channel(string name);
		~Channel(void);
};