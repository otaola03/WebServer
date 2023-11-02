#ifndef SERVER_HPP

# define SERVER_HPP

#define C200 "HTTP/1.1 200 OK"
#define C404 "HTTP/1.1 404 Not Found"
#define C405 "HTTP/1.1 405 Method Not Allowed"


#include "Port.hpp"
#include "Location.hpp"
#include "HttpRequest.hpp"

#include "../../includes/templates.h"

#include <algorithm>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <map>
#include <vector>
#include <cstdlib>

extern char** environ;

class Server
{
	private:
		std::string name;
		std::string root;
		intPortMap	fdPortsList;	//map(fd, Port)
		intCharMap errorPages;
		locationVector locations;

		Server();

	public:
		int fdMax;

	public:
		Server(\
				const std::string& name, \
				const std::string& root, \
				const intVector& ports, \
				const intCharMap& errorPages, \
				const locationVector& locations\
		);
		Server(const std::string& name, const std::vector<int> ports);
		Server(const Server& toCopy);
		~Server();

		Port&	getPort(const int fd);

		std::string postImage(std::string path, std::string body);
		std::string	getIndex(std::string code, std::string path);
		std::string getImg(std::string path);
		std::string getPython(std::string path);
		std::string getPhp(std::string path);
		std::string	getMessage(HttpRequest& parser);
		std::string pythonCgiHandler(std::string script, char **av);
		std::string phpCgiHandler(std::string script, char **av);
		bool	isMethodAllowed(HttpRequest& parser, Location& location);
		bool	fileFinder(const std::string& path, std::string& founDir);

		void	addPortsToSet(fd_set& portsList);

		Server& operator=(const Server& toAssign);
};

#endif
