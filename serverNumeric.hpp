#pragma once

// :[prefix] command [params] :[trailing]

#define NOTICE_MSG(message) (string(":localhost NOTICE :") + (message) + "\r\n")

#define ERR_SAMPLE(code, errName, nick) (string(":localhost ") + (code) + ' ' + (nick) + " :" + (errName) + "\r\n")

#define ERR_SAMPLE_2(code, errName, nick, name) (string(":localhost ") + (code) + ' ' + (nick) + ' ' + (name) + " :" + (errName) + "\r\n")

#define ERR_SAMPLE_3(code, errName, nick, name, channelName) (string(":localhost ") + (code) + ' ' + (nick) + ' ' + (name) + ' ' + (channelName) + " :" + (errName) + "\r\n")


#define RPL(nick, user, command, target, character, content) (string(":") + (nick) + '!' + (user) + "@localhost " + (command) + ' ' + (target) + (character) + (content) + "\r\n")

#define RPL_MODE(nick, user, channelName, flags) (RPL(nick, user, "MODE", channelName, ' ', flags))

#define RPL_PRIVMSG(nick, user, target, msg) (RPL(nick, user, "PRIVMSG", target, " :", msg))

#define RPL_PART(nick, user, channelName, reasonToPart) (RPL(nick, user, "PART", channelName, " :", reasonToPart))

#define RPL_JOIN(nick, user, channelName, msg) (RPL(nick, user, "JOIN", channelName, " * :", msg))


// ERROR CODES

#define RPL_CHANNELMODEIS(nick, channelName, modeFlags) (":localhost 324 " + (nick) + ' ' + (channelName) + ' ' + (modeFlags) + "\r\n")

#define RPL_NOTOPIC(nick, channelName) (ERR_SAMPLE_2("331", "No topic is set", nick, channelName))

#define RPL_TOPIC(nick, channelName, channelTopic) (":localhost 332 " + (nick) + ' ' + (channelName) + ' ' + (channelTopic) + "\r\n")

#define RPL_INVITING(nick, targetNick, channelName) (":localhost 341 " + (nick) + ' ' + (targetNick) + ' ' + (channelName)  + "\r\n")

#define ERR_NOSUCHNICK(nick, targetNick) (ERR_SAMPLE_2("401", "No such nick", nick, targetNick))

#define ERR_NOSUCHCHANNEL(nick, channelName) (ERR_SAMPLE_2("403", "No such channel", nick, channelName))

#define ERR_UNKNOWNCOMMAND(nick, targetCommand) (ERR_SAMPLE_2("421", "Unknown command", nick, targetCommand))

#define ERR_NONICKNAMEGIVEN(nick) (ERR_SAMPLE("431", "No nickname given", nick))

#define ERR_ERRONEUSNICKNAME(nick, targetNick) (ERR_SAMPLE_2("432", "Erroneous nickname", nick, targetNick))

#define ERR_NICKNAMEINUSE(targetNick) (ERR_SAMPLE("433", "Nickname is already in use", targetNick))

#define ERR_USERNOTINCHANNEL(nick, targetNick, channelName) (ERR_SAMPLE_3("441", "They aren't on that channel", nick, targetNick, channelName))

#define ERR_NOTONCHANNEL(nick, channelName) (ERR_SAMPLE_2("442", "You're not on that channel", nick, channelName))

#define ERR_USERONCHANNEL(nick, targetNick, channelName) (ERR_SAMPLE_3("443", "is already on channel", nick, targetNick, channelName))

#define ERR_NOTREGISTERED(nick) (ERR_SAMPLE("451", "You have not registered", nick))

#define ERR_NEEDMOREPARAMS(nick, command) (ERR_SAMPLE_2("461", "Not enough parameters", nick, command))

#define ERR_ALREADYREGISTRED(nick) (ERR_SAMPLE("462", "You may not reregister", nick))

#define ERR_PASSWDMISMATCH(nick) (ERR_SAMPLE("464", "Password incorrect", nick))

#define ERR_KEYSET(nick) (ERR_SAMPLE("467", "Channel key already set", nick))

#define ERR_CHANNELISFULL(nick, channelName) (ERR_SAMPLE_2("471", "Cannot join channel (+l)", nick, channelName))

#define ERR_UNKNOWNMODE(nick, mode) (ERR_SAMPLE_2("472", "is unknown mode char to me", nick, mode))

#define ERR_INVITEONLYCHAN(nick, channelName) (ERR_SAMPLE_2("473", "Cannot join channel (+i)", nick, channelName))

#define ERR_BADCHANNELKEY(nick, channelName) (ERR_SAMPLE_2("475", "Cannot join channel (+k)", nick, channelName))

#define ERR_CHANOPRIVSNEEDED(nick, channelName) (ERR_SAMPLE_2("482", "You're not channel operator", nick, channelName))
