
#include "../../Irc.hpp"

bool Channel::isUserInvited(string nick) {
	return ((std::find(_inviteUsers.begin(), _inviteUsers.end(), nick) != _inviteUsers.end()) ? 1 : 0);
}

bool verifyChannelmodes(Channel* tarChannel, Client* actualClient)
{
	if (tarChannel->isFlagSet('i') && !tarChannel->isUserInvited(actualClient->getNick()))
	{
		// cout << RED << "OLA\n" << END ;
		// >> :luna.AfterNET.Org 473 alex #fft :Cannot join channel (+i)
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
			//ver se o user ja foi bannido do channel
			//ver se o user ja foi convidado para o channel
			tarChannel->setChannelUsers(false, actualClient);
			tarChannel->sendAll(RPL_JOIN(actualClient->getNick(), actualClient->getUser(), channelName, string("realname")));
		}
		return;
	}

	//Caso o canal nao exista ele cria o seu canal
	tarChannel = createChannel(channelName);
	tarChannel->setChannelUsers(true, actualClient);
	msg += ':' + actualClient->getNick() + '!' + actualClient->getUser() + "@localhost JOIN " + channelName + " * :realname\r\n";
	cout << msg << endl;
	tarChannel->sendAll(RPL_JOIN(actualClient->getNick(), actualClient->getUser(), channelName, string("realname")));
}


// ERR_NEEDMOREPARAMS (461)
// ERR_NOSUCHCHANNEL (403)
// ERR_TOOMANYCHANNELS (405)
// ERR_BADCHANNELKEY (475)
// ERR_BANNEDFROMCHAN (474)
// ERR_CHANNELISFULL (471)
// ERR_INVITEONLYCHAN (473)
// ERR_BADCHANMASK (476)
// RPL_TOPIC (332)
// RPL_TOPICWHOTIME (333)
// RPL_NAMREPLY (353)
// RPL_ENDOFNAMES (366)



