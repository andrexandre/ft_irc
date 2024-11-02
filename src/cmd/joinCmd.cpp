
#include "../../Irc.hpp"



bool verifyChannelmodes(Channel* tarChannel, Client* actualClient)
{
	if (tarChannel->isFlagSet('i') && !tarChannel->isUserInvited(actualClient->getNick()))
	{
		return (serverErrorMsg(actualClient->getSock(), ERR_INVITEONLYCHAN(actualClient->getNick(), tarChannel->getChannelName())), 1);
	}
	return 0;
}

void Irc::joinCmd(std::istringstream &ss, Client* actualClient)
{
	string channelName;
	string msg;
	ss >> channelName;

	if (channelName[0] != '#')
		return (serverErrorMsg(actualClient->getSock(), ERR_NOSUCHCHANNEL(actualClient->getNick(), channelName)));

	Channel* tarChannel;
	if ((tarChannel = findChannel(channelName)))
	{
		// i k l mode flags to check
		if (!verifyChannelmodes(tarChannel, actualClient))
		{
			//ver se o user ja foi convidado para o channel
			tarChannel->setChannelUsers(false, actualClient);
			tarChannel->sendAll(RPL_JOIN(actualClient->getNick(), actualClient->getUser(), channelName, string("realname")));
		}
		return;
	}

	tarChannel = createChannel(channelName);
	tarChannel->setChannelUsers(true, actualClient);
	//apagar depois
	msg += ':' + actualClient->getNick() + '!' + actualClient->getUser() + "@localhost JOIN " + channelName + " * :realname\r\n";
	cout << msg << endl;
	tarChannel->sendAll(RPL_JOIN(actualClient->getNick(), actualClient->getUser(), channelName, string("realname")));
}

// ERR_CHANNELISFULL (471)
// ERR_NEEDMOREPARAMS (461)
// ERR_TOOMANYCHANNELS (405)
// ERR_BADCHANNELKEY (475)
// RPL_TOPIC (332)



