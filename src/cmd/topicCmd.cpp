#include "../../Irc.hpp"

void Irc::topicCmd(istringstream &ss, Client* client)
{
	string msg;
	string content;
	string channelName;

	if (ssLength(ss) < 2)
		return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "TOPIC"));

	ss >> channelName;
	Channel* tarChannel;
	if (!(tarChannel = findChannel(channelName)))
		return sendMsg(client->getSock(), ERR_NOSUCHCHANNEL(client->getNick(), channelName));
	if (!tarChannel->isPartOfChannel(client->getNick()))
		return sendMsg(client->getSock(), ERR_NOTONCHANNEL(client->getNick(), channelName));
	
	if (ss >> content)
	{
		if (tarChannel->isFlagSet('t') && !tarChannel->isOperator(client->getNick()))
			return sendMsg(client->getSock(), ERR_CHANOPRIVSNEEDED(client->getNick(), channelName));
		
		string tmp;
		std::getline(ss, tmp);
		content += tmp;
		
		tarChannel->setChannelTopic(content.substr(1));

		cout << YELLOW << msg << GREEN << RPL(client->getNick(), client->getUser(), "TOPIC", channelName, " :", tarChannel->getChannelTopic()) << END << endl;
		
		return (tarChannel->sendAll(RPL(client->getNick(), client->getUser(), "TOPIC", channelName, " ", tarChannel->getChannelTopic())));
	}
	else
	{
		if (tarChannel->getChannelTopic().empty())
			return sendMsg(client->getSock(), RPL_NOTOPIC(client->getNick(), tarChannel->getChannelName()));
		else
			return sendMsg(client->getSock(), RPL_TOPIC(client->getNick(), tarChannel->getChannelName(), tarChannel->getChannelTopic()));
	}
}
