#pragma once

#include "../../webserv.hpp"

class EpollManager
{
	private:
		int epSock;
		std::vector<int> listFds;

	public:
		EpollManager(void);
		~EpollManager(void);

	public:
		void addFd(int fd);
		void deleteFd(int fd);
		void modFd(int fd, uint32_t newEvent);
	
	public:
		int getEpSock(void) const;
};


// struct Sock
// {
// 	enum TYPE
// 	{
// 		SERVER,
// 		CONN,
// 	};


// };




// class fileManager
// {
// 	public:
// 		fileManager(void);
// 		~fileManager(void);
	
// 	private:
// 		std::vector<int> connFds;
// 		std::vector<int> serverFds;




// };