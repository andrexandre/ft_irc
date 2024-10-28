#include "../../Irc.hpp"
//<< TOPIC #lolipop :candy man
//>> :alex!alex21@9C5B1D.95C97E.C247D8.AE513.IP TOPIC #lolipop :candy man
//>> :Aurora.AfterNET.Org 331 andre #asd :No topic is set.
//>> :alex!alex21@9C5B1D.95C97E.C247D8.AE513.IP TOPIC #asdwq :i like kamoca

// mudar
// TOPIC #qwe :kamoka  mi e d mundo
// >> :alex!alex21@9C5B1D.95C97E.C247D8.AE513.IP TOPIC #qwe :kamoka  mi e d mundo
//ver
// << TOPIC #qwe

// >> :luna.AfterNET.Org 333 alex #qwe alex!alex21@9C5B1D.95C97E.C247D8.AE513.IP 1729685524
void Irc::topicCmd(std::istringstream &ss, Client* actualClient)
{
	string channelName;
	string msg;

	ss >> channelName;
	Channel* tarChannel = findChannel(channelName);

	if (tarChannel && tarChannel->isPartOfChannel(actualClient->getNick()))
	{
		string content;
		if (ss >> content)
		{
			if (!tarChannel->isOperator(actualClient->getNick()))
				return (serverErrorMsg(actualClient->getSock(), ERR_CHANOPRIVSNEEDED(actualClient->getNick(), channelName)));
			
			string tmp;
			std::getline(ss, tmp);
			content += tmp;
			// content += ss.str().substr(0, ss.str().find(con));
			tarChannel->setChannelTopic(content);
			msg += ':' + actualClient->getNick() + '!' + actualClient->getUser() + "@localhost TOPIC " + channelName + ' ' + tarChannel->getChannelTopic() + "\r\n";
			return (tarChannel->sendAll(msg));
		}
		else
		{
			if (tarChannel->getChannelTopic().empty())
			{
				msg += RPL_NOTOPIC(actualClient->getNick(), tarChannel->getChannelName());
				if (send(actualClient->getSock(), msg.c_str(), msg.size(), 0) == -1)
					throw std::runtime_error("Cannot send response of topic");
			}
			else
			{
				msg += RPL_TOPIC(actualClient->getNick(), tarChannel->getChannelName(), tarChannel->getChannelTopic());
				if (send(actualClient->getSock(), msg.c_str(), msg.size(), 0) == -1)
					throw std::runtime_error("Cannot send response of topic");
			}
		}
	}
	else
	{
		if (!tarChannel)
			serverErrorMsg(actualClient->getSock(), ERR_NOSUCHCHANNEL(actualClient->getNick(), channelName));
		else
			serverErrorMsg(actualClient->getSock(), ERR_NOTONCHANNEL(actualClient->getNick(), channelName));
	}
}
