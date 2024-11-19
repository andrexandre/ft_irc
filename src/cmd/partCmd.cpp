#include "../../Irc.hpp"

static void retrieveReason(string content, string& reason)

{
	size_t start = content.find(reason);
	string tmp = content.substr(start + 1);
	reason = tmp;
}

void Irc::partCmd(istringstream &ss, Client* client)
{
	string msg;
	string channelName;
	Channel* tarChannel;
	string reasonToPart;

	if (!(ss >> channelName) || !(ss >> reasonToPart))
		return sendMsg(client->getSock(), ERR_NEEDMOREPARAMS(client->getNick(), "PART"));
	
	if (!(tarChannel = findChannel(channelName)))
		return sendMsg(client->getSock(), ERR_NOSUCHCHANNEL(client->getNick(), channelName));
	
	if (!tarChannel->isPartOfChannel(client->getNick()))
		return sendMsg(client->getSock(), ERR_NOTONCHANNEL(client->getNick(), channelName));

	retrieveReason(ss.str(), reasonToPart);

	cout << endl << RPL_PART(client->getNick(), client->getUser(), channelName, reasonToPart) << endl; // apagar depois

	tarChannel->sendAll(RPL_PART(client->getNick(), client->getUser(), channelName, reasonToPart));
	tarChannel->removeClient(client);
	if (tarChannel->getNumberOfUsersOnChannel() == 0)
	{
		_serverChannels.erase(std::find(_serverChannels.begin(), _serverChannels.end(), tarChannel));
		delete tarChannel;
	}
}
