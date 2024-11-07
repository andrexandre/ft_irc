#include "../../Irc.hpp"

// KICK #a analexan
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
	if (channel->isOperator(actualClient->getNick()))
		return (serverErrorMsg(actualClient->getSock(), ERR_CHANOPRIVSNEEDED(actualClient->getNick(), channelName)));
	Client* targetClient = findClient(nickName);
	if (!targetClient)
		return (serverErrorMsg(actualClient->getSock(), ERR_NOSUCHNICK(actualClient->getNick(), nickName)));
	if (channel->isPartOfChannel(actualClient->getNick()))
		return (serverErrorMsg(actualClient->getSock(), ERR_NOTONCHANNEL(actualClient->getNick(), channelName)));
	if (channel->isPartOfChannel(targetClient->getNick()))
		return (serverErrorMsg(actualClient->getSock(), ERR_USERONCHANNEL(actualClient->getNick(), channelName)));
	// send massage
	// TEST ORDER OF TESTS ON PAPER!!!
	channel->removeClient(targetClient);
}
