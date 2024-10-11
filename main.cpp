#include "webserv.hpp"

int verbose = 0;
int linepos;

int checkField(string& name, size_t srt)
{
	string fields[] = {"listen", "server_name", "root", "index", 
	"client_max_body_size", "autoindex", "return", "allowedMethods", "error_page"};
	
	size_t numFields = sizeof(fields) / sizeof(fields[0]);
	for (size_t i = srt; i < numFields; i++)
	{
		if (name == fields[i]){
			return (i);
		}
	}
	return (-1);
}

/**
 * This funtion will verify if inside a server block the identation is right
 * @param line the line we are trying to validate
 * @param newBlock points to the actual server block we are reading
 * @param data class that will save all the server blocks we create
*/
bool checkIndentation(string& line, WebServ& data, Server_block** newBlock)
{
	static int nb; 

	size_t idx = line.find_first_not_of('	');
	
	if (idx > 1 && *newBlock)
		throw("Wrong format of identation(too much tabs)!");
	else if (idx == 0)
	{
		if (line.compare("server"))
			throw("Wrong format of identation(no tab with wrong field)!");
		
		nb++;
		if (verbose)
			cout << YELLOW << "creating server class id: " << nb << END << endl;
		*newBlock = new Server_block(nb);
		data.setServers(*newBlock);
		return(true);
	}
	return(false);
}

/**
 * Verify before finding a server block if we have any wrong argument
 * @param newBlock points to the actual server block we are reading
 * @return (true) if the is only a line has only these characters "\a\b\t\n\v\f\r"
 * @return (false) if we already are inside a server block
 * @throw ("Invalid line!") if there is any character different from these
*/
bool validSpace(Server_block** newBlock, string& line)
{
	if (!(*newBlock))
	{
		size_t idx = line.find_first_not_of("\a\b\t\n\v\f\r");
		if (idx == string::npos)
			return (true);
		else if (line.compare("server"))
			throw("Invalid line!");
	}

	return (false);
}


bool endsWithConf(const string& fileName)
{
	const std::string suffix = ".conf";
    return fileName.size() >= suffix.size() &&
		fileName.rfind(suffix) == (fileName.size() - suffix.size());
}

/**
 * This function will read the file and do the parsing of each line
 * @return(true) if the file has everything right
 * @return(false) if the file doesn't follow our syntax
*/
bool parsing(string configFilePath, WebServ& data)
{
	int idx = 0;
	string line;
	std::ifstream configFile;
	Server_block* newBlock = NULL;
	linepos++; // apagar depois
	try
	{
		if (!endsWithConf(configFilePath))
			throw("file doesn't end with .conf");
		
		configFile.open(configFilePath.c_str());
		if (!configFile.is_open())
			throw("could not open config file");
		
		while (std::getline(configFile, line))
		{
			// apagar depois
			if (verbose)
				cout << GREEN "line " << linepos++ << " (Server block): " << line << END << endl;

			if (line.empty() || line[0] == '#' || validSpace(&newBlock, line) || checkIndentation(line, data, &newBlock))
				continue;
			
			std::stringstream ss(line);
			if (ss >> line)
			{
				if (line[0] == '#')
					continue;
				
				if ((line == "location" && !(idx = 0)) || (line == "cgi" && (idx = 1)))
				{
					(newBlock->*(newBlock->ptrSetSpecial[idx]))(ss, configFile, line);
					continue;
				}
				idx = checkField(line, 0);
				if (idx == -1)
					throw("Invalid directive in the server block!");
				else
					(newBlock->*(newBlock->ptrSet[idx]))(ss);
			}
			else
				throw("Invalid line!");
		}
		data.mandatoryConfig();
		if (verbose)
			data.printServers(); // apagar depois
	}
	catch(const char *err)
	{
		cerr << RED << "Parse error: " << err << END << endl;
		configFile.close();
		return (false);
	}
	configFile.close();
	return (true);
}

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
	ws.server_core();
	return (0);
}
