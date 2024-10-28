#pragma once





#define RPL_TOPIC(nick, channelName, channelTopic) (":localhost 332 " + (nick) + ' ' + (channelName) + ' ' + (channelTopic) + "\r\n")
//>> :localhost 331 alex #channel :No topic is set.
#define RPL_NOTOPIC(nick, channelName) (":localhost 331 " + (nick) + ' ' + (channelName) + ' ' + ":No topic is set." + "\r\n")

//>> :localhost 324 alex #channel +tn 
#define RPL_CHANNELMODEIS(nick, channelName, modeFlags) (":localhost 324 " + (nick) + ' ' + (channelName) + " :" + (modeFlags) + "\r\n")

/// Numeric Replies
#define ERR_SAMPLE(code, errName, nick, name) (string(":localhost ") + (code) + ' ' + (nick) + ' ' + (name) + " :" + (errName) + "\r\n")

// PRIVMSG
#define ERR_NOSUCHNICK(nick, targetNick) (ERR_SAMPLE("401", "No such nick", nick, targetNick))

// JOIN
#define ERR_NOSUCHCHANNEL(nick, channelName) (ERR_SAMPLE("403", "No such channel", nick, channelName))

// PART
#define ERR_NOTONCHANNEL(nick, channelName) (ERR_SAMPLE("442", "You're not on that channel", nick, channelName))

// TOPIC
#define ERR_CHANOPRIVSNEEDED(nick, channelName) (ERR_SAMPLE("482", "You're not channel operator", nick, channelName))

// NICK
#define ERR_NICKNAMEINUSE(nick, targetNick) (ERR_SAMPLE("433", "Nickname is already in use", nick, targetNick))
// :localhost 433 analexan alex :Nickname is already in use