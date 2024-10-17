#include "Channel.hpp"

Channel::Channel(string name) : _channelName(name) {}


Channel::~Channel(void) {}


string Channel::getChannelName(void) const {
	return (_channelName);
}