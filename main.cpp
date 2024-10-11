#include "webserv.hpp"

int verbose = 0;
int linepos;

int main(int ac, char **av)
{
	static WebServ ws;
	string configFilePath;
	if (av[1] && string(av[1]) == "--verbose")
	{
		ac--;
		av[1] = av[2];
		verbose = 1;
	}
	if (ac == 1)
		configFilePath = DEFAULT_CONFIG_FILE_PATH;
	else
		configFilePath = av[1];
	if (ac > 2 || (av[1] && !av[1][0]))
		return (cerr << "./webserv [configuration file]" << endl), 1;
	if (!parsing(configFilePath, ws))
		return (1);
	ws.run_server();
	return (0);
}
