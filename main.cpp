#include "Irc.hpp"


int main(int ac, char **av)
{
	static Irc irc;
	irc.run_server();
	return (0);
}
