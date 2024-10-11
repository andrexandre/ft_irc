#pragma once

#include "../webserv.hpp"

#define LOCAL_HOST			"127.0.0.1"
#define DEFAULT_BODY_SIZE	1024
#define DEFAULT_SERVER_NAME "localhost"
#define DEFAULT_PORT 8080
#define DEFAULT_ROOT "./"
#define DEFAULT_CONFIG_FILE_PATH "config_files/default.conf"

// Mudar depois para uma verdadeira
#define DEFAULT_ERROR_PAGES "404 index.html"
#define DEFAULT_INDEX_PAGE "index.html"

class Locations;

class Server_block
{
	// Server Block exclusive directives
	private:
		int serverId;
		string _hostName;
		std::vector<int> _port;
		std::vector<string> _serverName;
		std::vector<Locations*> _locs;
		std::map<string, string> _cgiPath;
	
	// Server Block directives that can be shared with Location
	protected:
		bool _autoIndex;
		string _rootPath;
		size_t _maxBodySize;
		std::vector<string> _indexs;
		std::map<int, string> _errorPages;
		std::vector<string> _allowedMethods;
		std::map<int, string> _redirections;
	
	public:
		Server_block(int i);
		~Server_block(void);
		void (Server_block::*ptrSet[9])(std::stringstream& ss);
		bool (Server_block::*ptrSetSpecial[2])(std::stringstream& ss, std::ifstream& configFile, string& line);		

	public:
		// void setHost(std::stringstream& ss);
		// void setPort(std::stringstream& ss);
		void setIndexs(std::stringstream& ss);
		void setHostName(std::stringstream& ss);
		void setRootPath(std::stringstream& ss);
		void setAutoindex(std::stringstream& ss);
		void setErrorPages(std::stringstream& ss);
		void setServerName(std::stringstream& ss);
		void setMaxBodySize(std::stringstream& ss);
		void setRedirections(std::stringstream& ss);
		void setAllowedMethods(std::stringstream& ss);
		bool setCGI(std::stringstream& ss, std::ifstream& configFile, string& line);
		bool setLocation(std::stringstream& ss, std::ifstream& configFile, string& line);

	public:
		// print attributes
		void printInfo(void);
		void validateMandatoryConfig(void);
		bool isHostNameSet(void) const;
		bool isPortSet(void) const;
		bool isIndexSet(void) const;
		bool isRootSet(void) const;
		bool isServerNameSet(void) const;
		bool isBodySizeSet(void) const;
		bool isRedirectionsSet(void) const;
		bool isMethodsSet(void) const;
		bool isErrorPageSet(void) const; 
		bool isLocationSet(void) const;	
	
	public:
		bool getAutoIndex(void) const;
		size_t getMaxBodySize(void) const;

		const string& getHostName(void) const;
		const string& getRoot(void) const;

		const std::vector<int>& getPort(void) const;

		const std::vector<string>& getServerName(void) const;
		const std::vector<string>& getMethods(void) const;
		const std::vector<string>& getIndexPages(void) const;

		const std::map<int, string>& getErrorPages(void) const;
		const std::map<int, string>& getRedirections(void) const;

		const std::vector<Locations*>& getLocations(void) const;


};

class Locations : public Server_block
{
	private:
		string operatorType;
		string dirName;
	
	public:
		Locations(void);
		~Locations(void);
		void setOpertaorType(string optr);
		void setDirName(string name);
	
	public:
		void printLoc(void) const;
		const string getDirName(void) const;
		const string getOperator(void) const;
};
