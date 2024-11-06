#include "Irc.hpp"

int main(int ac, char **av)
{
	static Irc irc;

	if (ac != 3 || !av[1][0] || !av[2][0])
		return (cerr << "./ircserv <port> <password>" << endl), 1;
	cout << CYAN "\nServer started (Ctrl+C to quit)" END;
	irc.run_server(av);
	return (0);
}
