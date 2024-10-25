#pragma once





#define RPL_TOPIC(nick, channelName, channelTopic) (":localhost 332 " + (nick) + ' ' + (channelName) + ' ' + (channelTopic) + "\r\n")
//>> :luna.AfterNET.Org 331 alex #a32 :No topic is set.
#define RPL_NOTOPIC(nick, channelName) (":localhost 331 " + (nick) + ' ' + (channelName) + ' ' + ":No topic is set." + "\r\n")

//>> :Aurora.AfterNET.Org 324 alex #kk3 +tn 
#define RPL_CHANNELMODEIS(nick, channelName, modeFlags) (":localhost 324 " + (nick) + ' ' + (channelName) + " :" + (modeFlags) + "\r\n")

//ERROS
#define ERR_SAMPLE(code, errName, nick, targetNick) (string(":localhost ") + (code) + ' ' + (nick) + ' ' + (targetNick) + " :" + (errName) + "\r\n")

#define ERR_NOSUCHNICK(nick, targetNick) (ERR_SAMPLE("401", "No such nick", nick, targetNick))

#define ERR_NOSUCHCHANNEL(nick, channelName) (ERR_SAMPLE("403", "No such channel", nick, channelName))

#define ERR_NOTONCHANNEL(nick, channelName) (ERR_SAMPLE("442", "You're not on that channel", nick, channelName))

#define ERR_CHANOPRIVSNEEDED(nick, channelName) (ERR_SAMPLE("482", "You're not channel operator", nick, channelName))