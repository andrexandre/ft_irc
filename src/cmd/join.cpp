
#include "../../Irc.hpp"

// JOIN #a

Channel* Irc::findChannel(string name)
{
	std::vector<Channel*>::iterator it;
	for (it = _serverChannels.begin(); it != _serverChannels.end(); it++)
	{
		if ((*it)->getChannelName() == name)
			return (*it);
	}
	
	Channel* newChannel = new Channel(name);
	_serverChannels.push_back(newChannel);
	return (newChannel);
}


void Irc::joinCmd(std::istringstream &ss, Client* actualClient)
{
	string channelName;
	string msg;
	ss >> channelName;

	if (channelName[0] != '#')
	{
		//>> :localhost 403 "actualClient nick" channelName :No such channel
		//mensagem de erro
		return;
	}

	// cout << channelName << endl;
	Channel* tarChannel = findChannel(channelName);

	if (tarChannel->getUsersNumber() == 0)
		tarChannel->setChannelUsers(true, actualClient);

	// if (tarChannel->isPartOfChannel(actualClient->getNick()))
	// >> :alex!alex21@9C5B1D.95C97E.C247D8.AE513.IP JOIN #a * :realname
	msg += ':' + actualClient->getNick() + '!' + actualClient->getUser() + "@localhost JOIN " + channelName + " * :realname\r\n";
	cout << msg << endl;
	send(actualClient->getSock(), msg.c_str(), msg.size(), 0);

}

// Criar um canal
// << JOIN #lol
// >> :alex!alex21@9C5B1D.95C97E.C247D8.AE513.IP JOIN #lol * :realname
// << MODE #lol
// << WHO #lol %chtsunfra,152
// >> :luna.AfterNET.Org 353 alex = #lol :@alex!alex21@9C5B1D.95C97E.C247D8.AE513.IP
// >> :luna.AfterNET.Org 366 alex #lol :End of /NAMES list.
// >> :luna.AfterNET.Org 324 alex #lol +tn 
// >> :luna.AfterNET.Org 329 alex #lol 1729253221
// >> :luna.AfterNET.Org 354 alex 152 #lol alex21 9C5B1D.95C97E.C247D8.AE513.IP *.afternet.org alex H@xz 0 :realname
// >> :luna.AfterNET.Org 315 alex #lol :End of /WHO list



// Juntar a um canal ja criado
//  JOIN #lol
// >> :andre!andre21@9C5B1D.95C97E.C247D8.AE513.IP JOIN #lol * :realname
// << MODE #lol
// << WHO #lol %chtsunfra,152
// >> :luna.AfterNET.Org 353 andre = #lol :andre!andre21@9C5B1D.95C97E.C247D8.AE513.IP @alex!alex21@9C5B1D.95C97E.C247D8.AE513.IP
// >> :luna.AfterNET.Org 366 andre #lol :End of /NAMES list.
// >> :luna.AfterNET.Org 324 andre #lol +tn 
// >> :luna.AfterNET.Org 329 andre #lol 1729253221
// >> :luna.AfterNET.Org 354 andre 152 #lol andre21 9C5B1D.95C97E.C247D8.AE513.IP *.afternet.org andre Hxz 0 :realname
// >> :luna.AfterNET.Org 354 andre 152 #lol alex21 9C5B1D.95C97E.C247D8.AE513.IP *.afternet.org alex H@xz 0 :realname
// >> :luna.AfterNET.Org 315 andre #lol :End of /WHO list


