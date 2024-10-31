#include "../../Irc.hpp"

// INVITE <nick> [<channel>]

// enviar para o sock de que faz o invite
// << INVITE andre #ddr
// >> :Aurora.AfterNET.Org 341 alex andre #ddr

// provavelmente o hex envia isto por default
// >> :alex!alex21@9C5B1D.95C97E.C247D8.AE513.IP INVITE andre #ddr
void Irc::inviteCmd(std::istringstream &ss, Client* actualClient)
{
	string targetNick;
	string channelName;
	Channel* targetChannel;
	
	
	if ((ss >> targetNick) && !findClient(targetNick))
		return serverErrorMsg(actualClient->getSock(), ERR_NOSUCHNICK(actualClient->getNick(), targetNick));
	
	if ((ss >> channelName) && !(targetChannel = findChannel(channelName)))
		return serverErrorMsg(actualClient->getSock(), ERR_NOSUCHCHANNEL(actualClient->getNick(), channelName));
	
	if (targetChannel->isPartOfChannel(actualClient->getNick()))
		return serverErrorMsg(actualClient->getSock(), ERR_NOTONCHANNEL(actualClient->getNick(), channelName));

	if (!targetChannel->isOperator(actualClient->getNick()))
		return serverErrorMsg(actualClient->getSock(), ERR_CHANOPRIVSNEEDED(actualClient->getNick(), channelName));
	
	targetChannel->setInviteUsers(targetNick);

}