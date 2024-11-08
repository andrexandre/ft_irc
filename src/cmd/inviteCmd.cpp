#include "../../Irc.hpp"

// Talvez vamos ter de fazer os sample do tripo 1 2 3 que vao 
// significar o numero de argumentos antesa da mensagem especifica
void Irc::inviteCmd(istringstream &ss, Client* actualClient)
{
	string targetNick;
	string channelName;
	Client* targetClient;
	Channel* targetChannel;
	
	// Verificar se o  comando esta com todos os parametros ERR_NEEDMOREPARAMS 
	if ((ss >> targetNick) && !(targetClient = findClient(targetNick)))
		return serverErrorMsg(actualClient->getSock(), ERR_NOSUCHNICK(actualClient->getNick(), targetNick));
	
	if ((ss >> channelName) && !(targetChannel = findChannel(channelName)))
		return serverErrorMsg(actualClient->getSock(), ERR_NOSUCHCHANNEL(actualClient->getNick(), channelName));
	
	if (!targetChannel->isPartOfChannel(actualClient->getNick()))
		return serverErrorMsg(actualClient->getSock(), ERR_NOTONCHANNEL(actualClient->getNick(), channelName));

	if (!targetChannel->isOperator(actualClient->getNick()))
		return serverErrorMsg(actualClient->getSock(), ERR_CHANOPRIVSNEEDED(actualClient->getNick(), channelName));
	
	string tmp = targetClient->getNick() + " " + channelName; // temporario
	if (targetChannel->isPartOfChannel(targetClient->getNick()))
		return serverErrorMsg(actualClient->getSock(), ERR_USERONCHANNEL(actualClient->getNick(), tmp));

	targetChannel->setInviteUsers(targetNick);
	// Mensagem a avisar que ele convidou  alguem para o canal
	sendMsg(actualClient->getSock(), RPL_INVITING(actualClient->getNick(), targetNick, channelName));
	// Mensagem para a pessoa que foi convidada a dizer quem convidou e para que canal
	sendMsg(targetClient->getSock(), RPL(actualClient->getNick(), actualClient->getNick(), "INVITE", targetNick, " ", channelName));
}
