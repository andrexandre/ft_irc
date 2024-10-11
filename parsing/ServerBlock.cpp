#include "Parsing.hpp"
// int linepos;

Server_block::Server_block(int id) : serverId(id)
{
	ptrSet[0] = &Server_block::setHostName;
	ptrSet[1] = &Server_block::setServerName;
	ptrSet[2] = &Server_block::setRootPath;
	ptrSet[3] = &Server_block::setIndexs;
	ptrSet[4] = &Server_block::setMaxBodySize;
	ptrSet[5] = &Server_block::setAutoindex;
	ptrSet[6] = &Server_block::setRedirections;
	ptrSet[7] = &Server_block::setAllowedMethods;
	ptrSet[8] = &Server_block::setErrorPages;

	_autoIndex = false;
	_maxBodySize = 0;

	ptrSetSpecial[0] = &Server_block::setLocation;
	ptrSetSpecial[1] = &Server_block::setCGI;
}

void Server_block::validateMandatoryConfig(void)
{
	std::stringstream ss;

	if (!isPortSet())
		_port.push_back(DEFAULT_PORT);
	
	if (!isHostNameSet())
	{
		ss << this->_port[0];
		_hostName = LOCAL_HOST;
		_hostName += ":" + ss.str();
		ss.str("");
		ss.clear();
	}

	if (!isServerNameSet())
		_serverName.push_back(DEFAULT_SERVER_NAME);
	
	if (!isBodySizeSet())
		_maxBodySize = DEFAULT_BODY_SIZE;
	
	if (!isErrorPageSet())
	{
		ss << DEFAULT_ERROR_PAGES;
		setErrorPages(ss);
	}
		
	if (!isIndexSet())
		_indexs.push_back(DEFAULT_INDEX_PAGE);
	
	if (!isRootSet())
		_rootPath = DEFAULT_ROOT;
}

Server_block::~Server_block(void)
{
	for (size_t i = 0; i < _locs.size(); i++)
		delete _locs[i];
	
}

void isDirectory(string name)
{
	if (name.find_first_of('/') == 0)
		name.insert(name.begin(), '.');
	
	struct stat info;
	if (access(name.c_str(), F_OK) != -1) 
	{
		if (!(!stat(name.c_str(), &info) && S_ISDIR(info.st_mode)))
			throw("Is not a directory");
	}
	else
		throw("File/Directory dont exist");
}

int isFileOk(string path)
{
	if (path.find('/') == 0)
		path.insert(path.begin(), '.');
	
	if (access(path.c_str(), F_OK | R_OK) == -1)
		return(-1);

	return (0);
}

void Server_block::setHostName(std::stringstream& ss) {
	
	string arg;
	string tmp;
	string host;
	char* end = NULL;

	if ((ss >> arg))
	{
		if (std::count(arg.begin(), arg.end(), ':') == 1)
		{
			if (!_hostName.empty())
				throw("Host already exist");

			string name = arg.substr(0, arg.find(':'));
			if (name.empty())
				throw("Invalid host!");
			
			if (name == "localhost")
			{
				name = LOCAL_HOST;
				std::stringstream box(arg.substr(arg.find(':')));
				name += box.str();
				_hostName = name;
			}
			else
			{
				if (std::count(name.begin(), name.end(), '.') == 3)
				{
					long value;
					for (size_t i = 0; i < 4; i++)
					{
						host = name.substr(0, name.find('.'));
						if (host.size() == 0)
							throw("Invalid host");
						name.erase(0, name.find('.') + 1);
						value = strtol(host.c_str(), &end, 10);
						if (*end || errno == ERANGE)
							throw("invalid host");
						else if (value < 0 || value > 255)
							throw("Invalid host!");
					}
					name = arg;
				}
				else
					throw("Invalid host!");
			}
			_hostName = name;
			arg.erase(0, arg.find(':') + 1);
		}

		long nb = strtol(arg.c_str(), &end, 10);
		if (*end || errno == ERANGE || nb <= 0 || nb >= 65535)
			throw("Invalid port to listen!");
		else
			_port.push_back(nb);
		
		if (ss >> arg)
			throw("Too much argumets");
	}
	else
		throw("Empty directive!");
}

void Server_block::setRootPath(std::stringstream& ss) 
{
	string path;

	if (ss >> path)
	{
		isDirectory(path);
		_rootPath = path;

		if (ss >> path)
			throw("Too much arguments");
	}
	else
		throw("Empty directive!");
}

void Server_block::setMaxBodySize(std::stringstream& ss) {
	
	char *end;
	string arg;

	if (ss >> arg)
	{
		long val = strtol(arg.c_str(), &end, 10);
		if (*end || errno == ERANGE || val <= 0)
			throw("Invalid argument");
		
		_maxBodySize = val;

		if (ss >> arg)
			throw("Too much arguments");
	}
	else
		throw("Empty directive!");
}

void Server_block::setRedirections(std::stringstream& ss) {
	
	char *end;
	string arg;

	if (ss >> arg)
	{
		long val = strtol(arg.c_str(), &end, 10);
		if (*end || errno == ERANGE)
			throw("Invalid code for redirection!");
		
		if (ss >> arg)
			_redirections.insert(std::make_pair(val, arg));
		else
			throw("Not enough arguemnts!");
		
		if (ss >> arg)
			throw("Too much arguments!");
	}
	else
		throw("Empty directive!");

}

void Server_block::setAutoindex(std::stringstream& ss) {
	
	string arg;

	ss >> arg;
	if (ss.fail())
		throw("Empty directive!");
	else if (arg == "on")
		_autoIndex = true;
	else if (arg == "off")
		_autoIndex = false;
	else
		throw("Invalid argument!");
	
	if (ss >> arg)
		throw("Too much arguments");
}

void Server_block::setErrorPages(std::stringstream& ss) {
	
	char *end; 
	long code;
	string arg;
	string fileName;
	std::vector<int> val;

	errno = 0;
	while (ss >> arg)
	{
		code = strtol(arg.c_str(), &end, 10);
		if (*end)
		{
			fileName = arg;
			if (isFileOk(fileName) == -1)
				throw("Invalid code status or file doesnt exist!");
			else
			{
				if (ss >> arg)
					throw("Too much arguments!");
				else
					break;
			}
		}
		else if (errno == ERANGE)
			throw("Invalid status code!");
		
		val.push_back(code);
	}
	
	if (val.size() >= 1 && !fileName.empty())
	{
		for (size_t i = 0; i < val.size(); i++)
			_errorPages.insert(std::make_pair(val[i], arg));
	}
	else
		throw("Need more arguments!");
}

void Server_block::setAllowedMethods(std::stringstream& ss) {
	serverId += 0;
	string methods[] = {"GET", "POST", "DELETE"};
	string arg;

	while (ss >> arg)
	{
		if (arg != methods[0] && arg != methods[1] && arg != methods[2])
			throw("Invalid method!");
		else
		{
			if (std::find(_allowedMethods.begin(), _allowedMethods.end(), arg) != _allowedMethods.end())
				continue;
			else
				_allowedMethods.push_back(arg);
		}	
	}

	if (arg.empty())
		throw("Empty directive!");
}

void Server_block::setIndexs(std::stringstream& ss) {
	
	if(!_indexs.empty())
		_indexs.clear();
	
	string path;
	struct stat info;//ver se funciona da mesma forma do nginx 

	while (ss >> path)
	{
		if (isFileOk(path) == -1 || ((!stat(path.c_str(), &info) && S_ISDIR(info.st_mode))))
			continue;
		else
			_indexs.push_back(path);
	}
	
	if (path.empty())
		throw("Empty directive!");
}

void Server_block::setServerName(std::stringstream& ss) {
	
	string name;
	while (ss >> name)
	{
		_serverName.push_back(name);
	}
	
	if (name.empty())
		throw("Empty directive!");
}

bool Server_block::setCGI(std::stringstream& ss, std::ifstream& configFile, string& line) {
	
	string arg;

	if (ss >> arg)
		throw("Too much arguments");
	
	ss.clear();
	line.clear();
	while (std::getline(configFile, line))
	{
		cout << MAGENTA <<"line " << linepos++ << " (CGI block):" << line << END << endl;
		if (line.empty())
			continue;
		
		if (line.find_first_not_of('	') == 2)
		{
			ss.str(line);
			if (ss >> line)
			{
				if (line.find('#') == 0)
					continue;
				
				if (line == ".py" || line == ".php")
				{
					if (!(ss >> arg))
						throw("Not enougth arguments");
					
					if (arg == "/usr/bin/python3" || arg == "/usr/bin/php")
						_cgiPath.insert(std::make_pair(line, arg));
					else
						throw("Invalid arguments!");
				}
				else
					throw("Invalid CGI extension");
			}
		}
		else
		{
			for (int i = line.size(); i >= 0; i--)
				configFile.putback(line[i]);
			break;
		}
		ss.clear();
		line.clear();
	}
	return(true);
}

bool Server_block::setLocation(std::stringstream& ss, std::ifstream& configFile, string& line) {
	Locations *newLoc = NULL;
	string arg;
	string optr;
	int pos;

	if (ss >> arg)
	{
		if (arg == "=")
		{
			optr = arg;
			if (ss >> arg)
			{
				isDirectory(arg);
				if (ss >> arg)
					throw("Too much arguments!");
			}
			else
				throw("Need more argumnets!");
		}
		else if (arg != "/")
		{
			isDirectory(arg);
			if (ss >> arg)
				throw("Too much arguments!");
		}
		else
			throw("Invalid operator!");

	}
	else
		throw("Empty directive!");


	newLoc = new Locations();
	newLoc->setOpertaorType(optr);
	newLoc->setDirName(arg);
	_locs.push_back(newLoc);
	ss.clear();
	line.clear();
	arg.clear();

	//isto pode ir para uma outra funcao
	while (std::getline(configFile, line))
	{
		cout << CYAN <<"line " << linepos++ << " (Location block):" << line << END << endl;
		if (line.empty())
			continue;
		
		if (line.find_first_not_of('	') == 2)
		{
			ss.str(line);
			if (ss >> arg)
			{
				if (arg.find('#') == 0)
					continue;
				
				pos = checkField(arg, 2);
				if (pos == -1)
					throw("Invalid directive in location block!");
				else
					(newLoc->*(newLoc->ptrSet[pos]))(ss);
			}
			else
				throw("Invalid line!");
		}
		else
		{
			for (int i = line.size(); i >= 0; i--)
				configFile.putback(line[i]);
			break;
		}
		ss.clear();
	}

	if (arg.empty())
		throw("Location cannot be empty!");
	return (true);
}

void Server_block::printInfo(void)
{
	cout << YELLOW << "-----------------------------------------------" << END << endl;
	cout << YELLOW << "                 SERVER " << serverId << END << endl;
	cout << YELLOW << "-----------------------------------------------" << END << endl;

	if (!_hostName.empty())
		cout << BLUE << "HOSTNAME: " << GREEN << getHostName() << END << endl;

	if (!_port.empty())
	{
		cout << BLUE << "PORTS: " << GREEN;
		for (std::vector<int>::iterator it = _port.begin(); it != _port.end(); it++)
		{
			cout << *it << " ";
		}
		cout << END << endl;
	}

	if (!_serverName.empty())
	{
		cout << BLUE << "ServerNames: " << GREEN;
		for (std::vector<string>::iterator it = _serverName.begin(); it != _serverName.end(); it++)
		{
			cout << *it << " ";
		}
		cout << END << endl;
	}

	if (!_rootPath.empty())
		cout << BLUE << "Root Path: " << GREEN << _rootPath << END << endl;

	if (!_indexs.empty())
	{
		cout << BLUE << "Index files: " << GREEN;
		for (std::vector<string>::iterator it = _indexs.begin(); it != _indexs.end(); it++)
		{
			cout << *it << " ";
		}
		cout << END << endl;
	}

	cout << BLUE << "Autoindex: " << GREEN << _autoIndex << END << endl;

	cout << BLUE << "Client Max Body Size: " << GREEN << _maxBodySize << END << endl;

	if (!_allowedMethods.empty())
	{
		cout << BLUE << "HTTP Methods: " << GREEN;
		for (std::vector<string>::iterator it = _allowedMethods.begin(); it != _allowedMethods.end(); it++)
		{
			cout << *it << " ";
		}
		cout << END << endl;
	}

	if (!_redirections.empty())
	{
		cout << BLUE << "Redirections: " << endl;
		for (std::map<int, string>::iterator it = _redirections.begin(); it != _redirections.end(); it++)
		{
			cout  << GREEN  << it->first << ": " << MAGENTA << it->second << endl;
		}
		cout << END << endl;
	}

	if (!_errorPages.empty())
	{
		cout << BLUE << "Error pages: " << GREEN << endl;
		for (std::map<int, string>::iterator it = _errorPages.begin(); it != _errorPages.end(); it++)
		{
			cout << GREEN  << it->first << ": " << MAGENTA << it->second << endl;
		}
		cout << END << endl;
	}

	if (!_cgiPath.empty())
	{
		cout << BLUE << "CGI: " << endl;
		for (std::map<string, string>::iterator it = _cgiPath.begin(); it != _cgiPath.end(); it++)
		{
			cout  << GREEN  << it->first << ": " << MAGENTA << it->second << endl;
		}
		cout << END << endl;
	}

	if (!_locs.empty())
	{
		cout << RED << "-----------------------------------------------" << END << endl;
		cout << RED << "                 Location Blocks "  << END << endl;
		cout << RED << "-----------------------------------------------" << END << endl;
		
		for (size_t i = 0; i < _locs.size(); i++)
		{
			cout << RED << "Location " << i << ":" << END << endl;
			_locs[i]->printLoc();
		}
	}
	cout << CYAN << "-----------------------------------------------" << END << endl;
	cout << CYAN << "-----------------------------------------------" << END << endl << endl;
}



/**
 * All functions below check whether the essential directives are fulfilled
 * @return (1) if the directive is set 
 * @return (0) if the directive is not set
*/
bool Server_block::isHostNameSet(void) const{
	return(!_hostName.empty());
}

bool Server_block::isPortSet(void) const{
	return(!_port.empty());
}

bool Server_block::isIndexSet(void) const{
	return(!_indexs.empty());
}

bool Server_block::isRootSet(void) const{
	return(!_rootPath.empty());
}

bool Server_block::isServerNameSet(void) const{
	return(!_serverName.empty());
}

bool Server_block::isBodySizeSet(void) const{
	return (_maxBodySize == 0 ? false : true);
}

bool Server_block::isMethodsSet(void) const{
	return(!_allowedMethods.empty());
}

bool Server_block::isRedirectionsSet(void) const{
	return(!_redirections.empty());
}

bool Server_block::isErrorPageSet(void) const{
	return(!_errorPages.empty());
}

bool Server_block::isLocationSet(void) const{
	return(!_locs.empty());
}




/**
 * All the functions below will be the getrs to the 
 * private/protected attributes from the class
*/


bool Server_block::getAutoIndex(void) const{
	return (_autoIndex);
}

size_t Server_block::getMaxBodySize(void) const{
	return (_maxBodySize);
}



const string& Server_block::getHostName(void) const{
	return (_hostName);
}

const string& Server_block::getRoot(void) const{
	return (_rootPath);
}


const std::vector<int>& Server_block::getPort(void) const{
	return (_port);
}


const std::vector<string>& Server_block::getServerName(void) const{
	return (_serverName);
}

const std::vector<string>& Server_block::getMethods(void) const{
	return (_allowedMethods);
}

const std::vector<string>& Server_block::getIndexPages(void) const{
	return (_indexs);
}


const std::map<int, string>& Server_block::getErrorPages(void) const{
	return (_errorPages);
}

const std::map<int, string>& Server_block::getRedirections(void) const{
	return (_redirections);
}


const std::vector<Locations*>& Server_block::getLocations(void) const{
	return (_locs);
}






