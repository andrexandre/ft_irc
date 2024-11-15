#include "../../Irc.hpp"

static void retrieveReason(string content, string& reason)

{
	size_t start = content.find(reason);
	string tmp = content.substr(start + 1);
	reason = tmp;
}

void Irc::partCmd(istringstream &ss, Client* actualClient)
{
	string msg;
	string channelName;
	string reasonToPart;

	if (!(ss >> channelName) || !(ss >> reasonToPart))
		return serverErrorMsg(actualClient->getSock(), ERR_NEEDMOREPARAMS(actualClient->getNick(), "PART"));
	Channel* tarChannel;
	if (!(tarChannel = findChannel(channelName)))
		return serverErrorMsg(actualClient->getSock(), ERR_NOSUCHCHANNEL(actualClient->getNick(), channelName));
	
	if (!tarChannel->isPartOfChannel(actualClient->getNick()))
		return serverErrorMsg(actualClient->getSock(), ERR_NOTONCHANNEL(actualClient->getNick(), channelName));

	
	retrieveReason(ss.str(), reasonToPart);

	cout << endl << RPL_PART(actualClient->getNick(), actualClient->getUser(), channelName, reasonToPart) << endl;

	tarChannel->sendAll(RPL_PART(actualClient->getNick(), actualClient->getUser(), channelName, reasonToPart));
	tarChannel->removeClient(actualClient);
	if (tarChannel->getNumberOfUsersOnChannel() == 0)
	{
		_serverChannels.erase(std::find(_serverChannels.begin(), _serverChannels.end(), tarChannel));
		delete tarChannel;
	}
	
}
