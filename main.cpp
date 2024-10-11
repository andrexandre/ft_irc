#include "Irc.hpp"

// int main(void)
int main(int ac, char **av)
{
	static Irc irc;

	if (ac != 3 || !av[1][0] || !av[2][0])
		return (cerr << "./ircserv <port> <password>" << endl), 1;
	irc.run_server(av);
	return (0);
}
