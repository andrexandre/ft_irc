#include "Irc.hpp"

/**
 * @param type of line the logger will print, type 0 will change mode
 * @param data to be printed
 * 
 * if mode is 0, it will show default logs
 * 
 * if mode is 1, it will show all logs
 * 
 * uncomment the logger(0, 0) in the main function to change mode
 */
void logger(int type, int data)
{
	static int mode;
	if (!type)
		mode = !mode;

	if (type == 1 && mode)
		cout << BLUE << data << " Inputs received, Waiting for event..." END << endl;
	else if (type == 2 && mode)
		cout << "Fds received: " << data << endl;
	else if (type == 3 && mode)
		cout << GREEN "Received socket n: " << data << " with event ";
	else if (type == 4 && mode)
	{
		string eventString;
		if (data & EPOLLIN)
			eventString = "EPOLLIN";
		else if (data & EPOLLOUT)
			eventString = "EPOLLOUT";
		else if (data & EPOLLERR || data & EPOLLRDHUP || data & EPOLLHUP)
			eventString = "EPOLLERR || EPOLLRDHUP || EPOLLHUP";
		else
			eventString = "INVALID";
		cout << eventString << "\n" END << endl;
	}
	else if (type == 5 && mode)
		cout << RED "Reached uncommon place" END << endl;
}

int main(int ac, char **av)
{
	static Irc irc;

	if (ac != 3 || !av[1][0] || !av[2][0])
		return (cerr << "./ircserv <port> <password>" << endl), 1;
	cout << CYAN "Server started (Ctrl+C to quit)" END << endl;
	logger(0, 0);
	irc.run_server(av);
	return (0);
}
