
#include "../../Irc.hpp"

static bool verifyChannelmodes(Channel* tarChannel, Client* actualClient, istringstream& ss)
{
	string pass;
	ss >> pass;

	if (tarChannel->isFlagSet('i') && !tarChannel->isUserInvited(actualClient->getNick()))
		return (serverErrorMsg(actualClient->getSock(), ERR_INVITEONLYCHAN(actualClient->getNick(), tarChannel->getChannelName())), 1);
	else if (tarChannel->isFlagSet('l') && tarChannel->isChannelFull())
		return (serverErrorMsg(actualClient->getSock(), ERR_CHANNELISFULL(actualClient->getNick(), tarChannel->getChannelName())), 1);
	else if (tarChannel->isFlagSet('k') && (pass != tarChannel->getChannelPassword()))
		return (serverErrorMsg(actualClient->getSock(), ERR_BADCHANNELKEY(actualClient->getNick(), tarChannel->getChannelName())), 1);

	return 0;
}

void Irc::joinCmd(istringstream &ss, Client* actualClient)
{
	string msg;
	string channelName;

	if (!(ss >> channelName))
		return serverErrorMsg(actualClient->getSock(), ERR_NEEDMOREPARAMS(actualClient->getNick(), "JOIN"));

	if (channelName[0] != '#')
		return serverErrorMsg(actualClient->getSock(), ERR_NOSUCHCHANNEL(actualClient->getNick(), channelName));

	Channel* tarChannel;
	if ((tarChannel = findChannel(channelName)))
	{
		if (!verifyChannelmodes(tarChannel, actualClient, ss))
		{
			tarChannel->setChannelUsers(false, actualClient);
			tarChannel->sendAll(RPL_JOIN(actualClient->getNick(), actualClient->getUser(), channelName, string("realname")));
		}
		return;
	}
	tarChannel = createChannel(channelName);
	tarChannel->setChannelUsers(true, actualClient);

	cout << RPL_JOIN(actualClient->getNick(), actualClient->getUser(), channelName, string("realname")) << endl; // apagar depois
	
	tarChannel->sendAll(RPL_JOIN(actualClient->getNick(), actualClient->getUser(), channelName, string("realname")));
}