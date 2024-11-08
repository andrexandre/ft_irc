#include "EpollManager.hpp"

EpollManager::EpollManager(void) : epSock(epoll_create(1))
{
	if (epSock < 0)
		throw std::runtime_error("creating epoll instance");
}

EpollManager::~EpollManager(void)
{
	for (size_t i = 0; i < listFds.size(); i++)
		deleteFd(listFds[i]);
	close(epSock);
}

void EpollManager::addFd(int targetFd, uint32_t newEvent)
{
	struct epoll_event ev;
	
	bzero(&ev, sizeof(ev));
	ev.events = newEvent;
	ev.data.fd = targetFd;
	if (epoll_ctl(epSock, EPOLL_CTL_ADD, targetFd, &ev) == -1)
		throw std::runtime_error("Cannot add server sock in the epoll instance");
	
	listFds.push_back(targetFd);
}

void EpollManager::modFd(int targetFd, uint32_t newEvent)
{
	struct epoll_event ev;
	
	if (std::find(listFds.begin(), listFds.end(), targetFd) == listFds.end())
		return;
	bzero(&ev, sizeof(ev));
	ev.events = newEvent;
	ev.data.fd = targetFd;
	if (epoll_ctl(epSock, EPOLL_CTL_MOD, targetFd, &ev) == -1)
		throw std::runtime_error("Cannot modify the fd in epoll instace12");
}

void EpollManager::deleteFd(int targetFd)
{
	epoll_ctl(epSock, EPOLL_CTL_DEL, targetFd, NULL);
	close(targetFd);
	vector<int>::iterator it;
	it = std::find(listFds.begin(), listFds.end(), targetFd);
	listFds.erase(it);
}

int EpollManager::getEpSock(void) const {
	return (epSock);
}

