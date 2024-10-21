#include "../../Irc.hpp"
//<< TOPIC #lolipop :candy man
//>> :alex!alex21@9C5B1D.95C97E.C247D8.AE513.IP TOPIC #lolipop :candy man
void Irc::topicCmd(std::istringstream &ss, Client* actualClient)
{
	(void) ss;
	(void) actualClient;
	// string channelName;

	// ss >> channelName;
	// Channel* tarChannel = findChannel(channelName);

	// if (tarChannel)
	// {
	// 	string content;
	// 	ss >> content;
	// 	if (ss >> content)
	// 		tarChannel->setChannelTopic(content);
	// 	else
	// 	{

	// 	}
	// }
}
