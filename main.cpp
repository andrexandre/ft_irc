#include "Irc.hpp"


// int main(void)
int main(int ac, char **av)
{
	static Irc irc;

	if (ac != 3)
	{
		cerr << "Error: wrong number of arguments!" << endl;
		return (1);
	}
	irc.run_server(av);
	return (0);
}
