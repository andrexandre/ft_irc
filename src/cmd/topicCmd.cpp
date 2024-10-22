#include "../../Irc.hpp"
//<< TOPIC #lolipop :candy man
//>> :alex!alex21@9C5B1D.95C97E.C247D8.AE513.IP TOPIC #lolipop :candy man
//>> :Aurora.AfterNET.Org 331 andre #asd :No topic is set.
void Irc::topicCmd(std::istringstream &ss, Client* actualClient)
{
	(void) ss;
	(void) actualClient;
	string channelName;
	string msg;

	ss >> channelName;
	Channel* tarChannel = findChannel(channelName);

	if (tarChannel)
	{
		string content;
		if (ss >> content)
		{
			tarChannel->setChannelTopic(content);
			//sned mesage to all
		}
		else
		{
			if (tarChannel->getChannelTopic().empty())
			{
				msg += ":localhost 331 " + actualClient->getNick() + ' ' + channelName + " :No topic is set."  + "\r\n";
				serverErrorMsg(actualClient->getSock(), msg);
			}
			else
			{
				msg += ':' + actualClient->getNick() + '!' + actualClient->getUser() + "@localhost TOPIC " + channelName + ' ' + tarChannel->getChannelTopic() + "\r\n";
				tarChannel->sendAll(msg);
			}
		}
	}
	else
	{
		// mensagem de erro
	}
}
