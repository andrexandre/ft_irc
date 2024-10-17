#pragma once

#include "../../Irc.hpp"


class Channel
{
	private:
		string _channelName;

	public:
		string getChannelName(void) const;

	public:
		Channel(string name);
		~Channel(void);
};