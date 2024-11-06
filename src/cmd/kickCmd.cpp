#include "../../Irc.hpp"

// KICK #a analexan
// ERR_CHANOPRIVSNEEDED
void Irc::kickCmd(istringstream &ss, Client* actualClient)
{
	string channelName;
	string nickName;

	ss >> channelName;
	if (channelName.empty() || !(ss >> nickName))
		return serverErrorMsg(actualClient->getSock(), ERR_NEEDMOREPARAMS(actualClient->getNick(), "KICK"));
	Channel* channel = findChannel(channelName);
	if (!channel)
		return (serverErrorMsg(actualClient->getSock(), ERR_NOSUCHCHANNEL(actualClient->getNick(), channelName)));
}
