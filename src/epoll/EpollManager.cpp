#include "EpollManager.hpp"

EpollManager::EpollManager(void) : epSock(epoll_create(1))
{
	if (epSock < 0)
		throw std::runtime_error("Error: creating epoll instance");
}

EpollManager::~EpollManager(void)
{
	for (size_t i = 0; i < listFds.size(); i++)
		deleteFd(listFds[i]);
	
	cout << GREEN << "closing epoll sock" << END << endl;
	close(epSock);
}

void EpollManager::addFd(int targetFd)
{
	struct epoll_event ev;
	
	bzero(&ev, sizeof(ev));
	ev.events = EPOLLIN | EPOLLERR | EPOLLRDHUP | EPOLLHUP;
	ev.data.fd = targetFd;
	if (epoll_ctl(epSock, EPOLL_CTL_ADD, targetFd, &ev) == -1)
		throw std::runtime_error("Error: in adding server sock in the epoll instance");
	
	listFds.push_back(targetFd);
}

void EpollManager::modFd(int targetFd, uint32_t newEvent)
{
	struct epoll_event ev;
	
	bzero(&ev, sizeof(ev));
	ev.events = newEvent;
	ev.data.fd = targetFd;
	if (epoll_ctl(epSock, EPOLL_CTL_MOD, targetFd, &ev) == -1)
		throw std::runtime_error("Error: in modifyng the fd in epoll instace");
}

void EpollManager::deleteFd(int targetFd)
{
	epoll_ctl(epSock, EPOLL_CTL_DEL, targetFd, NULL);
	cout << GREEN << "closing fd number: " << targetFd << END << endl;
	close(targetFd);
	std::vector<int>::iterator it;
	it = std::find(listFds.begin(), listFds.end(), targetFd);
	listFds.erase(it, it + 1);
}

int EpollManager::getEpSock(void) const {
	return (epSock);
}

