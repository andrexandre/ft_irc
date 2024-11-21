#include "../../Irc.hpp"

void Irc::kickCmd(istringstream &ss, Client* client)
{
	string channelName;
	string nickName;

	if (!(ss >> channelName) || !(ss >> nickName))
		return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "KICK"));
	Channel* channel = findChannel(channelName);
	if (!channel)
		return sendMsg(client->getSock(), ERR_NOSUCHCHANNEL(client->getNick(), channelName));
	if (!channel->isPartOfChannel(client->getNick()))
		return sendMsg(client->getSock(), ERR_NOTONCHANNEL(client->getNick(), channelName));
	if (!channel->isOperator(client->getNick()))
		return sendMsg(client->getSock(), ERR_CHANOPRIVSNEEDED(client->getNick(), channelName));

	Client* targetClient = findClient(nickName);
	if (!targetClient)
		return sendMsg(client->getSock(), ERR_NOSUCHNICK(client->getNick(), nickName));
	if (!channel->isPartOfChannel(targetClient->getNick()))
		return sendMsg(client->getSock(), ERR_USERNOTINCHANNEL(client->getNick(), targetClient->getNick(), channelName));
	
	cout << "Send to client fd: " << client->getSock() << endl;
	channel->sendAll(RPL(client->getNick(), client->getUser(), "KICK", (channelName + " " + targetClient->getNick()), " :", client->getNick()));
	channel->removeClient(targetClient);
}
