#pragma once



#define RPL(nick, user, target, command, operator_, content) (string(":") + (nick) + '!' + (user) + "@localhost " + (command) + " " + (target) + (operator_) + (content) + "\r\n")

#define ERR_SAMPLE(code, errName, nick, name) (string(":localhost ") + (code) + ' ' + (nick) + ' ' + (name) + " :" + (errName) + "\r\n")

// TOPIC
#define RPL_TOPIC(nick, channelName, channelTopic) (":localhost 332 " + (nick) + ' ' + (channelName) + ' ' + (channelTopic) + "\r\n")

#define RPL_NOTOPIC(nick, channelName) (":localhost 331 " + (nick) + ' ' + (channelName) + ' ' + ":No topic is set." + "\r\n")

// MODE
//>> :localhost 324 alex #channel +tn 
#define RPL_CHANNELMODEIS(nick, channelName, modeFlags) (":localhost 324 " + (nick) + ' ' + (channelName) + " :" + (modeFlags) + "\r\n")

/// Numeric Replies

// >> :localhost 421 andre flkjdlfkjlkdfjgkljf :Unknown command	
#define ERR_UNKNOWNCOMMAND(nick, targetCommand) (ERR_SAMPLE("421", "Unknown command", nick, targetCommand))

// PRIVMSG
#define RPL_PRIVMSG(nick, user, target, msg) (RPL(nick, user, target, "PRIVMSG", " :", msg))

#define ERR_NOSUCHNICK(nick, targetNick) (ERR_SAMPLE("401", "No such nick", nick, targetNick))

// PART
#define RPL_PART(nick, user, channelName, reasonToPart) (RPL(nick, user, channelName, "PART", " ", msg))

// JOIN
#define RPL_JOIN(nick, user, channelName, msg) (RPL(nick, user, channelName, "JOIN", " * :", msg))

#define ERR_NOSUCHCHANNEL(nick, channelName) (ERR_SAMPLE("403", "No such channel", nick, channelName))

// PART
#define ERR_NOTONCHANNEL(nick, channelName) (ERR_SAMPLE("442", "You're not on that channel", nick, channelName))

// TOPIC
#define ERR_CHANOPRIVSNEEDED(nick, channelName) (ERR_SAMPLE("482", "You're not channel operator", nick, channelName))

// NICK
#define ERR_NICKNAMEINUSE(nick, targetNick) (ERR_SAMPLE("433", "Nickname is already in use", nick, targetNick))
// :localhost 433 analexan alex :Nickname is already in use

