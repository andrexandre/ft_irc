#include "../../Irc.hpp"

void Irc::leaveAllChannels(Client* ptr)
{
	vector<Channel*>::iterator it;
	for (it = _serverChannels.begin(); it != _serverChannels.end(); it++)
	{
		if ((*it)->isPartOfChannel(ptr->getNick()))
		{
			(*it)->sendPrivMsg(ptr->getSock(), RPL_PART(ptr->getNick(), ptr->getUser(), (*it)->getChannelName(), "Leaving"));
			(*it)->removeClient(ptr);
			
			if ((*it)->getNumberOfUsersOnChannel() == 0)
			{
				delete *it;
				it = _serverChannels.erase(it);
				it--;
			}
		}
	}
}

void Irc::quitCmd(istringstream &ss, Client* client)
{
	(void)ss;
	map<int, Client *>::iterator it = _clients.find(client->getSock());
	cout << YELLOW << "Closing connection, fd: " << it->first << END << endl;
	leaveAllChannels(client);
	delete it->second;
	epfds->deleteFd(it->first);
	_clients.erase(it->first);
}
