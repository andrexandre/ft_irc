#pragma once

#include "../../Irc.hpp"

class EpollManager
{
	private:
		int epSock;
		std::vector<int> listFds;

	public:
		EpollManager(void);
		~EpollManager(void);

	public:
		void addFd(int fd, uint32_t newEvent);
		void deleteFd(int fd);
		void modFd(int fd, uint32_t newEvent);
	
	public:
		int getEpSock(void) const;
};
// strace -e epoll_ctl,epoll_wait ./ircserv 8080 123