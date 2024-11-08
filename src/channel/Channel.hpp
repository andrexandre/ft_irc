#pragma once

#include "../../Irc.hpp"


class Channel
{
	private:
		string _channelName;
		string _channelTopic;
		string _channelModes;
		std::map<Client*, bool> _channelUsers;

		size_t _maxUsersNumber; // l: Set/remove limit of user on channel
		vector<string> _inviteUsers; // i: Set/remove Invite-only channel

	public:
		string getChannelName(void) const;
		string getChannelTopic(void) const;
		size_t getMaxUsersNumber(void) const;
		size_t getNumberOfUsersOnChannel(void) const;

		void setMaxUsersNumber(size_t nb);
		void setChannelTopic(string content);
		void setChannelUsers(bool oprt, Client* ptr);

		void removeClient(Client* ptr);
		bool isPartOfChannel(string nick) const;
		bool isOperator(string nick) const;

		void sendAll(string msg) const;
		void sendPrivMsg(int fd, string msg) const;

		void apllyInviteOnlyFlag(bool optr);
		void apllyTopicRestrictionFlag(bool optr);
		void setInviteUsers(string nick);
		void setChannelModes(char flag);
		void removeChannelModesFlag(char flag);
		bool isFlagSet(char flag) const;
		bool isChannelFull(void) const;
		bool isUserInvited(string nick) const;
		bool apllyLimitRestrictionFlag(istringstream& ss, string& modeFlag, Client* client);
	public:
		Channel(string name);
		~Channel(void);
};