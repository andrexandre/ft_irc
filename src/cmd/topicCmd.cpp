#include "../../Irc.hpp"

void Irc::topicCmd(istringstream &ss, Client* actualClient)
{
	string msg;
	string content;
	string channelName;

	ss >> channelName;
	Channel* tarChannel;

	if (ssLength(ss) != 2)
		return serverErrorMsg(actualClient->getSock(), ERR_NEEDMOREPARAMS(actualClient->getNick(), "TOPIC"));
	if (!(tarChannel = findChannel(channelName)))
		return serverErrorMsg(actualClient->getSock(), ERR_NOSUCHCHANNEL(actualClient->getNick(), channelName));
	if (!tarChannel->isPartOfChannel(actualClient->getNick()))
		return serverErrorMsg(actualClient->getSock(), ERR_NOTONCHANNEL(actualClient->getNick(), channelName));
	
	if (ss >> content)
	{
		if (tarChannel->isFlagSet('t') && !tarChannel->isOperator(actualClient->getNick()))
			return (serverErrorMsg(actualClient->getSock(), ERR_CHANOPRIVSNEEDED(actualClient->getNick(), channelName)));
		
		string tmp;
		std::getline(ss, tmp);
		content += tmp;
		
		tarChannel->setChannelTopic(content.substr(1));

		cout << YELLOW << msg << GREEN << RPL(actualClient->getNick(), actualClient->getUser(), "TOPIC", channelName, " :", tarChannel->getChannelTopic()) << END << endl;
		
		return (tarChannel->sendAll(RPL(actualClient->getNick(), actualClient->getUser(), "TOPIC", channelName, " ", tarChannel->getChannelTopic())));
	}
	else
	{
		if (tarChannel->getChannelTopic().empty())
			return serverErrorMsg(actualClient->getSock(), RPL_NOTOPIC(actualClient->getNick(), tarChannel->getChannelName()));
		else
			return serverErrorMsg(actualClient->getSock(), RPL_TOPIC(actualClient->getNick(), tarChannel->getChannelName(), tarChannel->getChannelTopic()));
	}
}
