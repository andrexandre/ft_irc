#pragma once



#define RPL(nick, user, command, target, character, content) (string(":") + (nick) + '!' + (user) + "@localhost " + (command) + " " + (target) + (character) + (content) + "\r\n")

#define ERR_SAMPLE(code, errName, nick, name) (string(":localhost ") + (code) + ' ' + (nick) + ' ' + (name) + " :" + (errName) + "\r\n")

// to remove if useless in the end of the project
#define ERR_SAMPLE_NO_NAME(code, errName, nick) (string(":localhost ") + (code) + ' ' + (nick) + " :" + (errName) + "\r\n")

// TOPIC
#define RPL_TOPIC(nick, channelName, channelTopic) (":localhost 332 " + (nick) + ' ' + (channelName) + ' ' + (channelTopic) + "\r\n")

#define RPL_NOTOPIC(nick, channelName) (":localhost 331 " + (nick) + ' ' + (channelName) + ' ' + ":No topic is set." + "\r\n")

// MODE
//>> :localhost 324 alex #channel +tn 
#define RPL_MODE(nick, user, channelName, flags) (RPL(nick, user, "MODE", channelName, " ", flags))

#define RPL_CHANNELMODEIS(nick, channelName, modeFlags) (":localhost 324 " + (nick) + ' ' + (channelName) + " :" + (modeFlags) + "\r\n")

//ver se nao e prececiso conlocar no canal a mensagem
#define ERR_UNKNOWNMODE(nick, mode) (ERR_SAMPLE("472", "is unknown mode char to me", nick, mode))

// INVITE
#define RPL_INVITING(nick, targetNick, channelName) (":localhost 341 " + (nick) + ' ' + (targetNick) + ' ' + (channelName)  + "\r\n")


/// Numeric Replies
#define ERR_UNKNOWNCOMMAND(nick, targetCommand) (ERR_SAMPLE("421", "Unknown command", nick, targetCommand))

// PRIVMSG
#define RPL_PRIVMSG(nick, user, target, msg) (RPL(nick, user, "PRIVMSG", target, " :", msg))

#define ERR_NOSUCHNICK(nick, targetNick) (ERR_SAMPLE("401", "No such nick", nick, targetNick))

// PART
#define RPL_PART(nick, user, channelName, reasonToPart) (RPL(nick, user, "PART", channelName, " :", reasonToPart))

// JOIN
#define RPL_JOIN(nick, user, channelName, msg) (RPL(nick, user, "JOIN", channelName, " * :", msg))

#define ERR_INVITEONLYCHAN(nick, channelName) (ERR_SAMPLE("473", "Cannot join channel (+i)", nick, channelName))

#define ERR_NOSUCHCHANNEL(nick, channelName) (ERR_SAMPLE("403", "No such channel", nick, channelName))

// PART
#define ERR_NOTONCHANNEL(nick, channelName) (ERR_SAMPLE("442", "You're not on that channel", nick, channelName))

// TOPIC
#define ERR_CHANOPRIVSNEEDED(nick, channelName) (ERR_SAMPLE("482", "You're not channel operator", nick, channelName))

// NICK
#define ERR_NICKNAMEINUSE(nick, targetNick) (ERR_SAMPLE("433", "Nickname is already in use", nick, targetNick))

#define ERR_NONICKNAMEGIVEN(nick) (ERR_SAMPLE_NO_NAME("431", "No nickname given", nick))

