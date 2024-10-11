#include "Parsing.hpp"


Locations::Locations(void): Server_block(-1) {
	// _autoIndex = true;
}

Locations::~Locations(void){}

void Locations::setOpertaorType(string optr){
	operatorType = optr;
}

void Locations::setDirName(string name){
	if (name.find('.') == 0)
		name.erase(0, 1);
	dirName = name;
}

const string Locations::getDirName(void) const {
	return (dirName);
}

const string Locations::getOperator(void) const {
	return (operatorType);
}

void Locations::printLoc(void) const
{
	cout << BLUE << "Directory Name: " << GREEN << dirName << END << endl;
	if (!_rootPath.empty())
		cout << BLUE << "Root Path: " << GREEN << _rootPath << END << endl;

	if (!_indexs.empty())
	{
		cout << BLUE << "Index files: " << GREEN;
		for (std::vector<string>::const_iterator it = _indexs.begin(); it != _indexs.end(); it++)
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
		for (std::vector<string>::const_iterator it = _allowedMethods.begin(); it != _allowedMethods.end(); it++)
		{
			cout << *it << " ";
		}
		cout << END << endl;
	}

	if (!_redirections.empty())
	{
		cout << BLUE << "Redirections: " << endl;
		for (std::map<int, string>::const_iterator it = _redirections.begin(); it != _redirections.end(); it++)
		{
			cout  << GREEN  << it->first << ": " << MAGENTA << it->second << endl;
		}
		cout << END << endl;
	}

	if (!_errorPages.empty())
	{
		cout << BLUE << "Error pages: " << GREEN << endl;
		for (std::map<int, string>::const_iterator it = _errorPages.begin(); it != _errorPages.end(); it++)
		{
			cout << GREEN  << it->first << ": " << MAGENTA << it->second << endl;
		}
		cout << END << endl;
	}
}
