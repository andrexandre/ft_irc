#include "../../Irc.hpp"

void Irc::kickCmd(istringstream &ss, Client* actualClient)
{
	string channelName;
	string nickName;

	if (!(ss >> channelName) || !(ss >> nickName))
		return serverErrorMsg(actualClient->getSock(), ERR_NEEDMOREPARAMS(actualClient->getNick(), "KICK"));
	Channel* channel = findChannel(channelName);
	if (!channel)
		return (serverErrorMsg(actualClient->getSock(), ERR_NOSUCHCHANNEL(actualClient->getNick(), channelName)));
	if (!channel->isPartOfChannel(actualClient->getNick()))
		return (serverErrorMsg(actualClient->getSock(), ERR_NOTONCHANNEL(actualClient->getNick(), channelName)));
	if (!channel->isOperator(actualClient->getNick()))
		return (serverErrorMsg(actualClient->getSock(), ERR_CHANOPRIVSNEEDED(actualClient->getNick(), channelName)));

	Client* targetClient = findClient(nickName);
	if (!targetClient)
		return (serverErrorMsg(actualClient->getSock(), ERR_NOSUCHNICK(actualClient->getNick(), nickName)));
	if (!channel->isPartOfChannel(targetClient->getNick()))
		return (serverErrorMsg(actualClient->getSock(), ERR_USERNOTINCHANNEL(actualClient->getNick(), channelName)));
	string tmp = channelName + " " + targetClient->getNick(); // temporario
	channel->sendAll(RPL(actualClient->getNick(), actualClient->getUser(), "KICK", tmp, " :", actualClient->getNick()));
	channel->removeClient(targetClient);
}
