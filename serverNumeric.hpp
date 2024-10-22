#pragma once


#define ERR_NOSUCHNICK(nick, targetNick) (":localhost 401 " + (nick) + ' ' + (targetNick) + " :No such nick" + "\r\n")

// #define ERR_NOSUCHSERVER()